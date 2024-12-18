#include <stdio.h>
#include <stdlib.h>
#include <omp.h> // Burada OpenMP zaman ölçümü için gereken başlık
#include <time.h>

void read_matrix(const char *filename, int ***matrix, int *rows, int *cols) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    fscanf(file, "%d %d", rows, cols);
    *matrix = (int **)malloc(*rows * sizeof(int *));
    for (int i = 0; i < *rows; i++) {
        (*matrix)[i] = (int *)malloc(*cols * sizeof(int));
        for (int j = 0; j < *cols; j++) {
            fscanf(file, "%d", &(*matrix)[i][j]);
        }
    }

    fclose(file);
}

void write_matrix_to_file(const char *filename, int **matrix, int rows, int cols) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file for writing");
        exit(1);
    }

    fprintf(file, "%d %d\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%d ", matrix[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void matrix_multiply_serial(int **A, int **B, int **C, int A_rows, int A_cols, int B_cols) {
    for (int i = 0; i < A_rows; i++) {
        for (int j = 0; j < B_cols; j++) {
            C[i][j] = 0;
            for (int k = 0; k < A_cols; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void matrix_multiply_parallel(int **A, int **B, int **C, int A_rows, int A_cols, int B_cols) {
    #pragma omp parallel for
    for (int i = 0; i < A_rows; i++) {
        for (int j = 0; j < B_cols; j++) {
            C[i][j] = 0;
            for (int k = 0; k < A_cols; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    int **A, **B, **C_serial, **C_parallel;
    int A_rows, A_cols, B_rows, B_cols;

    // Matrisleri dosyalardan oku
    read_matrix("matrix_a.txt", &A, &A_rows, &A_cols);
    read_matrix("matrix_b.txt", &B, &B_rows, &B_cols);

    if (A_cols != B_rows) {
        printf("Matrislerin çarpılabilmesi için boyutlar uyumsuz!\n");
        exit(1);
    }

    // Sonuç matrislerini oluştur
    C_serial = (int **)malloc(A_rows * sizeof(int *));
    C_parallel = (int **)malloc(A_rows * sizeof(int *));
    for (int i = 0; i < A_rows; i++) {
        C_serial[i] = (int *)malloc(B_cols * sizeof(int));
        C_parallel[i] = (int *)malloc(B_cols * sizeof(int));
    }

    // Seri çarpım
    double start = omp_get_wtime();
    matrix_multiply_serial(A, B, C_serial, A_rows, A_cols, B_cols);
    double end = omp_get_wtime();
    double serial_time = end - start;

    printf("Seri matris çarpımı süresi: %f saniye\n", serial_time);

    // Paralel çarpım
    start = omp_get_wtime();
    matrix_multiply_parallel(A, B, C_parallel, A_rows, A_cols, B_cols);
    end = omp_get_wtime();
    double parallel_time = end - start;

    printf("Paralel matris çarpımı süresi: %f saniye\n", parallel_time);

    // Paralel sonuçları dosyaya yaz
    write_matrix_to_file("sonuclar.txt", C_parallel, A_rows, B_cols);

    // Bellek temizliği
    for (int i = 0; i < A_rows; i++) {
        free(A[i]);
        free(C_serial[i]);
        free(C_parallel[i]);
    }
    for (int i = 0; i < B_rows; i++) {
        free(B[i]);
    }
    free(A);
    free(B);
    free(C_serial);
    free(C_parallel);

    return 0;
}

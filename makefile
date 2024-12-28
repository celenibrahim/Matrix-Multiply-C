CC = gcc
CFLAGS = -O2 -fopenmp
TARGET = matrix

MATRIX_A = matrix_a.txt
MATRIX_B = matrix_b.txt
RESULT_FILE = sonuclar.txt

# Varsayılan hedef: Derleme ve çalıştırma
all: $(TARGET) check_files
	./$(TARGET)

# Derleme işlemi
$(TARGET): matrix.c
	$(CC) $(CFLAGS) -o $(TARGET) matrix.c

# Dosyaların varlığını kontrol et
check_files:
	@if [ ! -f $(MATRIX_A) ]; then echo "Hata: $(MATRIX_A) dosyası bulunamadı!"; exit 1; fi
	@if [ ! -f $(MATRIX_B) ]; then echo "Hata: $(MATRIX_B) dosyası bulunamadı!"; exit 1; fi

# Temizleme işlemi
clean:
	rm -f $(TARGET) $(RESULT_FILE)

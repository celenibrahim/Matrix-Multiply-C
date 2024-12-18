CC = gcc
CFLAGS = -O2 -fopenmp
TARGET = matrix

# Varsayılan hedef: Derleme ve çalıştırma
all: $(TARGET)
	./$(TARGET)

# Derleme işlemi
$(TARGET): matrix.c
	$(CC) $(CFLAGS) -o $(TARGET) matrix.c

# Temizleme işlemi
clean:
	rm -f $(TARGET) sonuclar.txt

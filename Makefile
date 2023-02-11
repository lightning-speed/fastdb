CC  = gcc
cflags = -O3 -g -I . -lm -Wno-unused-result -Wno-format-overflow -lssl -lcrypto
OUTPUT_FILE  = ./fastdb
SRC = *.c

all:
	$(CC) $(cflags) $(SRC) ./server/*.c -o $(OUTPUT_FILE)
 

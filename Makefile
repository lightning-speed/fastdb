CC  = gcc
cflags = -O3 -I . -lm -Wno-unused-result -Wno-format-overflow
OUTPUT_FILE  = ./fastdb
SRC = *.c

all:
	$(CC) $(cflags) $(SRC) -o $(OUTPUT_FILE)
	$(OUTPUT_FILE) ./db.dat
	$(CC) $(cflags) server/*.c -o fast_server
   
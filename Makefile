CC  = gcc
cflags = -O3 -I . -lm
OUTPUT_FILE  = ./fastdb
SRC = *.c

all:
	$(CC) $(cflags) $(SRC) -o $(OUTPUT_FILE)
	$(OUTPUT_FILE) ./db.dat
   
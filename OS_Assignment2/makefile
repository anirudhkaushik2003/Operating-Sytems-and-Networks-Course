CC=gcc
LIBS=-lm
TARGET=Shell

all : main.c  processor/*.c utils/*.c commands/*.c
	$(CC) -o $(TARGET) main.c  processor/*.c utils/*.c commands/*.c $(LIBS)
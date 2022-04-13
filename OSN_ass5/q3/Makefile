CC=g++
LIBS=-lm
TARGET=server

all : server_prog.cpp dictionary.cpp myqueue.cpp 
	$(CC) -o $(TARGET) server_prog.cpp dictionary.cpp myqueue.cpp  $(LIBS) -lpthread
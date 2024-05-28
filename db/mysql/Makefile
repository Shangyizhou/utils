CC = g++
CFLAGS = -I/usr/include/mysql -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -lz -lm -lrt -latomic -lssl -lcrypto -ldl

all: main

main: main.cpp 
	$(CC) main.cpp $(CFLAGS) -o main
	
clean:
	rm -r main
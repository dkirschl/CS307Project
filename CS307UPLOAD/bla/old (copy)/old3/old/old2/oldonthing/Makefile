all: make

make: handler.cpp server.cpp handler.h
	g++ -o server -lpthread server.cpp handler.cpp handler.h

clean:
	rm -f server

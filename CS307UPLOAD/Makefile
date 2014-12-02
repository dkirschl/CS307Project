all: make

make:
	g++ -o server -lpthread server.cpp handler.cpp databaseInterface.cpp -lsqlite3

recreateDatabase:
	rm serverDatabase.db
	g++ -o createDatabase -lsqlite3 createDatabase.cpp
	./createDatabase

createDatabase:
	g++ -o createDatabase -lsqlite3 createDatabase.cpp
	./createDatabase

clean:
	rm -f server

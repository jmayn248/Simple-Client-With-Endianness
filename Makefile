client: main.o utility.o parse.o
        g++ main.o utility.o parse.o -o client

main.o: utility.h parse.h main.cpp
        g++ -c main.cpp

utility.o: utility.h utility.cpp
        g++ -c utility.cpp

parse.o: parse.h parse.cpp
        g++ -c parse.cpp

clean:
        rm *.o client

.PHONY: clean

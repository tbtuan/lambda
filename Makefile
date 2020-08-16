.PHONY: all

all: main testMain

main: demo/interpreter.cpp
		g++ -Wall -o main.o demo/interpreter.cpp

testMain: test/main.cpp
		  g++ -Wall -o testMain.o test/main.cpp

coverage: test/main.cpp
		  g++ -Wall --coverage -o testMain.o test/main.cpp
		  testMain.o
		  gcov main -r

clean: ;
		rm ./main.o ./testMain.o
cleanCoverage: ;
		rm *.gcda *.gcno *.gcov
		  
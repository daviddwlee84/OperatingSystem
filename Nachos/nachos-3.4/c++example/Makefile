all: stack inheritstack templatestack

stack: stack.h stack.cc
	g++ -o stack stack.cc

inheritstack: inheritstack.h inheritstack.cc list.h list.cc
	g++ -o inheritstack inheritstack.cc list.cc

templatestack: templatestack.h templatestack.cc
	g++ -o templatestack templatestack.cc

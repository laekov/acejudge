cc=g++
target: acejudge.cc
	g++	configure.cc stuff.cc compile.cc homepage.cc judge.cc help.cc acejudge.h acejudge.cc
install: acejudge.cc
	g++	configure.cc stuff.cc compile.cc homepage.cc judge.cc help.cc acejudge.h acejudge.cc -o acejudge
	sudo cp acejudge /usr/bin

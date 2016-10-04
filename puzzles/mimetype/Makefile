all: tst.exe tests clipboard

tst.exe : mimetype.cc
	g++.exe -std=c++11 -o tst.exe mimetype.cc

tests:
	./tst.exe --tests

# this works only for windoze
clipboard:
	cat mimetype.cc | clip

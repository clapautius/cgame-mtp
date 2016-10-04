all: tst.exe tests clipboard

tst.exe : the-accountant.cc
	g++.exe -ggdb -std=c++11 -o tst.exe the-accountant.cc

tests:
	./tst.exe --tests

# this works only for windoze
clipboard:
	cat the-accountant.cc | clip

all: tst.exe tests clipboard

tst.exe : the-accountant.cc
	g++ -Wall -ggdb -std=c++11 -o tst.exe the-accountant.cc

tests:
	./tst.exe --tests

# for windoze
#cat the-accountant.cc | clip
# for linux
#	cat the-accountant.cc | xclip -selection clipboard -display ":0"
clipboard:
	cat the-accountant.cc | clip

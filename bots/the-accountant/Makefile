CFLAGS = -Wall -Wpedantic -ggdb -std=c++11 -I../../common/
FILES = params.h ../../common/cgame-common.h the-accountant.cc

all: tst.exe tests clipboard

tst.exe : $(FILES)
	cat $(FILES) | grep -v ":grep-out:" > .the-accountant.cc
	g++ $(CFLAGS) -o tst.exe .the-accountant.cc

tests:
	./tst.exe --tests

# for windoze
#	cat the-accountant.cc | clip
# for linux
#	cat the-accountant.cc | xclip -selection clipboard -display ":0"
clipboard:
	cat .the-accountant.cc | clip

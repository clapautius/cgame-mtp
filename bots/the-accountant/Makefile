CFLAGS = -Wall -Wpedantic -ggdb -std=c++11 -I../../common/
FILES = params.h ../../common/cgame-common.h ../../common/cgame-common.cc the-accountant.cc

all: tst.exe tests onefile clipboard

tst.exe : $(FILES)
	g++ $(CFLAGS) -c the-accountant.cc
	g++ $(CFLAGS) -c ../../common/cgame-common.cc
	g++ $(CFLAGS) -o tst.exe *.o

tests:
	./tst.exe --tests

onefile:
	cat $(FILES) | grep -v ":grep-out:" > .the-accountant.cc
	g++ $(CFLAGS) -o tst.exe .the-accountant.cc

# for windoze
#	cat .the-accountant.cc | clip
# for linux
#	cat .the-accountant.cc | xclip -selection clipboard -display ":0"
clipboard:
	cat .the-accountant.cc | xclip -selection clipboard -display ":0"

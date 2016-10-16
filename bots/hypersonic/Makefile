CFLAGS = -Wall -Wpedantic -ggdb -std=c++11 -I../../common/
FILES = ../../common/cgame-common.h params.h utils.h world.h bomb.h hypersonic.cc world.cc bomb.cc ../../common/cgame-common.cc
BIGFILE = .hypersonic.cc

all: tst.exe tests onefile clipboard

tst.exe : $(FILES)
	g++ $(CFLAGS) -c hypersonic.cc
	g++ $(CFLAGS) -c world.cc
	g++ $(CFLAGS) -c bomb.cc
	g++ $(CFLAGS) -c ../../common/cgame-common.cc
	g++ $(CFLAGS) -o tst.exe *.o

tests:
	./tst.exe --tests

onefile:
	cat $(FILES) | grep -v ":grep-out:" > $(BIGFILE)
	g++ $(CFLAGS) -o tst.exe $(BIGFILE)

# for windoze
#	cat $(BIGFILE) | clip
# for linux
#	cat $(BIGFILE) | xclip -selection clipboard -display ":0"
clipboard:
	cat $(BIGFILE) | xclip -selection clipboard -display ":0"

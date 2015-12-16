CC = gcc

PATH_CODE = ./
PATH_MJPG = $(PATH_CODE)mjpg-streamer

CFLAGS += -O1 -DLINUX -D_GNU_SOURCE -Wall -shared -fPIC
LFLAGS += -lpthread -ldl
LDLIBS = -lwiringPi -lwiringPiDev -lm

all: input_control.so

fix-dependency:
	chmod +x ./test.sh
	sudo apt-get install subversion libjpeg8-dev imagemagick -y
	svn co "https://svn.code.sf.net/p/mjpg-streamer/code/mjpg-streamer/" mjpg-streamer
	cd mjpg-streamer && make
	git clone "git://git.drogon.net/wiringPi"
	cd wiringPi && git pull origin && ./build

clean:
	rm -f *.a *.o core *~ *.so *.lo ./mjpg-streamer/control.lo ./mjpg-streamer/input_control.so

input_control.so: $(OTHER_HEADERS) input_control.c control.lo
	$(CC) $(CFLAGS) -o $@ input_control.c control.lo
	cp $@ $(PATH_MJPG)
	cp control.lo $(PATH_MJPG)

control.lo: control.c control.h
	$(CC) $(CFLAGS) -o $@ control.c $(LDLIBS)

CXX=g++

CPPFLAGS=-I/usr/include/libusb-1.0 -Isrc/host/libpixyusb2/include -Isrc/host/arduino/libraries/Pixy2/ -I /usr/include/lo
#CPPFLAGS += -g

LDLIBS=build/libpixyusb2/libpixy2.a -lusb-1.0 -llo

SRCS=get_osc.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: build_get_osc

clean:
	rm -f *.o get_osc

build_get_osc: $(OBJS)
	$(CXX) $(LDFLAGS) -o get_osc $(OBJS) $(LDLIBS)

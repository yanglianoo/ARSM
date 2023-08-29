# INCLUDE+= -I../Smw/utility
INCLUDE+= -I/usr/local/include/opencv4
SMWLIBPATH:=../../Smw/lib/           #按照实际系统指定lib路径
OPENCVLIBPATH=/usr/local/lib64/
#指定编译器
CC = g++
CFLAGS:= -std=c++11 #c++11

.PHONY:test_usb
test_usb:test_usb.cpp USB.cpp
	$(CC)  $^ $(CFLAGS) $(INCLUDE) -o $@.out -L$(SMWLIBPATH) -L$(OPENCVLIBPATH) -lsmw -ludev -lopencv_core -lopencv_highgui -lopencv_videoio -Wl,-rpath=$(SMWLIBPATH):$(OPENCVLIBPATH)

.PHONY:clean
clean:
	rm -rf *.out

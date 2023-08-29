
# INCLUDE+= -I../Smw/utility
INCLUDE+= -I/usr/local/include/opencv4
SMW:= Smw
UTILITY:=utility
SMWLIBPATH:=../../../Smw/lib/   #按照实际系统指定lib路径
OPENCVLIBPATH=/usr/local/lib64/
#指定编译器
CC = g++
#编译选项 	$(CC)  $^ $(CFLAGS) $(INCLUDE) -o $@.out -L../../../Smw/lib/ -lsmw -ludev -lrealsense2 -lopencv_core

CFLAGS:= -std=c++11 #c++11

.PHONY:test_d435
test_d435:test_d435.cpp D435.cpp
	$(CC)  $^ $(CFLAGS) $(INCLUDE) -o $@.out -L$(SMWLIBPATH) -L$(OPENCVLIBPATH) -lsmw -ludev -lrealsense2 -lopencv_core -Wl,-rpath=$(SMWLIBPATH):$(OPENCVLIBPATH)


.PHONY:clean
clean:
	rm -rf *.out

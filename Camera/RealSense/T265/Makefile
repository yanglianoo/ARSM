INCLUDE+= -I/usr/local/include/opencv4
#指定编译器
CC = g++
#编译选项
CFLAGS:= -std=c++11 #c++11
SMWLIBPATH:=../../../Smw/lib/
OPENCVLIBPATH=/usr/local/lib64/
.PHONY:test_t265
test_t265: T265.cpp test_t265.cpp
	$(CC)  $^ $(CFLAGS) $(INCLUDE) -o $@.out    \
	-L$(SMWLIBPATH) -L$(OPENCVLIBPATH)  		\
	-lsmw -ludev -lrealsense2 -lopencv_core     \
	-Wl,-rpath=$(SMWLIBPATH):$(OPENCVLIBPATH)   \


.PHONY:clean
clean:
	rm -rf *.out

#指定编译器
CC = g++
#编译选项
CFLAGS:= -std=c++11 #c++11

#链接库路径
SmwLibPath:=../../Smw/ubuntu20.04_X86_lib

.PHONY:test_SL_S1
test_SL_S1:test_SL_S1.cpp SL_S1.CPP
	$(CC) $^ $(CFLAGS)  -o $@.out \
	-L$(SmwLibPath) \
	-lsmw  -lpthread -lsl_lidar_sdk \
	-Wl,-rpath=$(SmwLibPath) \

.PHONY:clean
clean:
	rm -rf *.out

#指定编译器
CC = g++
#编译选项
CFLAGS:= -std=c++11 #c++11


SmwLibPath:=../Smw/lib/      #按照实际系统指定lib路径

.PHONY:test_SL_S1
test_SL_S1:test_SL_S1.cpp SL_S1.CPP
	$(CC) $^ $(CFLAGS)  -o $@.out \
	-L$(SmwLibPath) \
	-lsmw  -lpthread -lsl_lidar_sdk \
	-Wl,-rpath=$(SmwLibPath) \

.PHONY:clean
clean:
	rm -rf *.out

#指定编译器
CC = g++
#编译选项
CFLAGS:= -std=c++11 #c++11

SmwLibPath:=../../Smw/ubuntu20.04_X86_lib/


.PHONY:test_lpmsig1
test_lpmsig1:test_lpmsig1.cpp LpmsIG1.CPP
	$(CC)  $^ $(CFLAGS)  -o $@.out \
	-L$(SmwLibPath) \
	-lsmw  -lpthread -lLpmsIG1_OpenSourceLib  \
	-Wl,-rpath=$(SmwLibPath) \

.PHONY:clean
clean:
	rm -rf *.out

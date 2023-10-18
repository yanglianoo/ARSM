#指定编译器
CC = g++
#编译选项
CFLAGS:= -std=c++11 #c++11


SmwLibPath:=../../Smw/ubuntu20.04_X86_lib/

.PHONY:test_nbit
test_power:LIJU.cpp Nbit.cpp test_nbit.cpp
	$(CC)  $^ $(CFLAGS)  -o $@.out \
	-L$(SmwLibPath) \
	-lsmw \
	-Wl,-rpath=$(SmwLibPath) \
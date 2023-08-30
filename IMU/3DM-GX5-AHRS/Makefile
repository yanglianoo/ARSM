#指定头文件路径
INCLUDE:=-Ic++-mscl/source
INCLUDE+=-Ic++-mscl/Boost/include

#指定编译器
CC = g++
#编译选项
CFLAGS:= -std=c++11 #c++11

SmwLibPath:=../../Smw/ubuntu20.04_X86_lib/
BoostLibPath:=../../Smw/ubuntu20.04_X86_lib/Boost/lib
LIB:=-lmscl
LIB+=-lboost_system
LIB+=-lboost_filesystem
LIB+=-lsmw

.PHONY:test_Gx5_ahrs
test_Gx5_ahrs:Gx5_ahrs.cpp 
	$(CC)  $^ $(CFLAGS) $(INCLUDE) -o $@.out \
	-L$(SmwLibPath) -L$(BoostLibPath) $(LIB) \
	-Wl,-rpath=$(SmwLibPath):$(BoostLibPath) \

.PHONY:clean
clean:
	rm -rf *.out

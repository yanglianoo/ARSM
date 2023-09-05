
UTILITY:=utility
#指定编译器
CC = g++
#编译选项
CFLAGS:= -std=c++11 #c++11

SmwLibPath:=./ubuntu20.04_X86_lib
.PHONY:libsmw.so
libsmw.so: *.cpp $(UTILITY)/*.cpp
	$(CC)  $^  -fPIC -shared -o $(SmwLibPath)/$@ $(CFLAGS) -ludev
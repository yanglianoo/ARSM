
INCLUDE+= -I.
UTILITY:=utility
#指定编译器
CC = g++
#编译选项
CFLAGS:= -std=c++11 #c++11

.PHONY:test
test:test.cpp $(UTILITY)/ClassFactory.cpp
	$(CC) $(INCLUDE) $^ $(CFLAGS) -o $@




INCLUDE+= -I../Smw/utility
INCLUDE+= -I../Smw
SMW:= Smw
UTILITY:=utility
#指定编译器
CC = g++
#编译选项
CFLAGS:= -std=c++11 #c++11

.PHONY:test_factory
test_factory:test_factory.cpp ../$(SMW)/$(UTILITY)/ClassFactory.cpp
	$(CC) $(INCLUDE) $^ $(CFLAGS) -o $@

.PHONY:test_lpmsig1
test_lpmsig1:test_lpmsig1.cpp 
	$(CC) $(INCLUDE) $^ $(CFLAGS)  -o $@ -L../Smw -lsmw

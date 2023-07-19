SMW:= Smw
UTILITY:=utility
#指定编译器
CC = g++
#编译选项
CFLAGS:= -std=c++11 #c++11

# .PHONY:test_factory
# test_factory:test_factory.cpp ../$(SMW)/$(UTILITY)/ClassFactory.cpp
# 	$(CC) $(INCLUDE) $^ $(CFLAGS) -o $@

.PHONY:test_SL_S1
test_SL_S1:test_SL_S1.cpp SL_S1.CPP
	$(CC) $^ $(CFLAGS)  -o $@.out -L../Smw/lib -lsmw  -lpthread -lsl_lidar_sdk 

.PHONY:clean
clean:
	rm -rf *.out

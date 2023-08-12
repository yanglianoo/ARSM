
# INCLUDE+= -I../Smw/utility
# INCLUDE+= -I../Smw
SMW:= Smw
UTILITY:=utility
#指定编译器
CC = g++
#编译选项
CFLAGS:= -std=c++11 #c++11

.PHONY:test_d435i
test_d435i:test_d435i.cpp D435i.cpp
	$(CC)  $^ $(CFLAGS)  -o $@.out -L../../Smw/lib/ -lsmw -ludev -lrealsense2 -lopencv_core `pkg-config --cflags --libs opencv`

.PHONY:clean
clean:
	rm -rf *.out

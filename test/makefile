TARGET = test_cfg_def_msg
CXX    = g++
CFLAGS = -g -O2 -Wall -Wno-deprecated

LIB_DIR   =..
USE_LIB   =-I../inc -L../lib -lcfg_def_msg

SRC      = .
INC      = ../inc
OBJS     = $(addsuffix .o, $(basename $(wildcard $(SRC)/*.cpp)))

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o $@ $(USE_LIB)

%.o: %.cpp
	$(CXX) $(CFLAGS) $(USE_LIB) -c -o $@ $<  

.PHONY: clean

clean:
	-rm -f  $(OBJS)  $(TARGET) protocol.*

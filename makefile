
vpath .

TARGET   = lib/libcfg_def_msg.a
CXX      = g++
CFLAGS   = -g -O2 -Wall -fPIC -Wno-deprecated

INC      = -I_3rd/tinyXML2 -I./inc.  

DIR_3rd  = ./_3rd/tinyXML2
OBJS_3rd = $(addsuffix .o, $(basename $(wildcard $(DIR_3rd)/*.cpp)))

SRC      = ./src
OBJS     = $(addsuffix .o, $(basename $(wildcard $(SRC)/*.cpp)))

OBJS    += $(OBJS_3rd)

$(TARGET): $(OBJS)
	mkdir -p lib
	ar cqs $@ $^

%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $< $(INC) 

.PHONY: clean

clean:
	-rm -f $(OBJS) $(TARGET)

test:
	make -C test

mem:
	valgrind --leak-check=yes ./$(target)


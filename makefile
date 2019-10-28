
INC			= -I. -I__3rd/tinyXML2 -I__utils -I__msgdef 
INCFLAGS	=
CFLAGS		=-g -Wall
LDFLAGS	=-lpthread -rdynamic
CC			=g++
target		= z
src		=$(wildcard *.cpp __3rd/tinyXML2/*.cpp __utils/*.cpp __msgdef/*.cpp)
cpps	=$(notdir $(src))
objs	=$(patsubst %.cpp,%.o,$(src))

INST	=/usr/local/lib

$(target):$(objs)
	$(CC) $^ $(LDFLAGS) -o $@
%.o:%.cpp
	$(CC) $< -c -o $@

.PHONY:clean
clean:
	-rm -f $(objs)
	
unst:
	rm $(INST)/$(target)
inst:
	cp $(target) $(INST)

mem:
	valgrind --leak-check=yes ./$(target)


##########################################################################################
# Project: Lisp++ 
# # Makefile Evilbinary 小E
# # Mail:rootntsd@gmail.com
# ##########################################################################################
CPP = g++
CXXFLAGS = -g     
LDFLAGS =

LIBS = 
INCS = 
SRCS = $(wildcard *.cpp )
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

TARGET=lisp++
.PHONY :all test clean

all:$(TARGET)

test:$(TARGET) test.l
	./$(TARGET) ./test.l
clean:
	rm -rf *.o $(TARGET) *.exe

$(TARGET):$(OBJS)
	$(CPP) -o $(TARGET) $(OBJS) $(CXXFLAGS)

.cpp.o:
	$(CPP) -c -o $@ $< $(LIBS) $(INCS) $(CXXFLAGS)

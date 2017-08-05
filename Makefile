EXE ?= start

CC := g++
DEBUG := -g
CFLAGS := -Wextra -Werror -std=c++11 -pedantic -Wno-error=unused-variable
LIBS := -lm

SRCS := $(shell find . -name '*.cpp')
OBJS := $(SRCS:%=./%.o)
DEPS := $(OBJS:.o=.d)

print-%  : ; @echo $* = $($*)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

%.cpp.o: %.cpp
	$(CC) -c $(CFLAGS) $^ $(LIBS) -o $@

clean:
	rm *.o *.dat $(EXE)

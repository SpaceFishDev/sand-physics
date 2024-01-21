src := $(wildcard src/*.c)
out := main
cflags := -l SDL2 -Wextra -Wall -Werror 

all: build run

build:
	gcc $(src) -o $(out) $(cflags)
run:
	./$(out)

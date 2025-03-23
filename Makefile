CC = g++
CFLAGS = -Iinclude
LDFLAGS = -Llib -lraylib -lGL -lGLU -lX11 -lm -lpthread -ldl

objs = main.o

main: $(objs)
	$(CC) -o main $(objs) $(LDFLAGS)

main.o: main.cc defs.h
	$(CC) -c main.cc $(CFLAGS)

run: main
	./main

debug: main
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=raylib.supp ./main

clean:
	rm -f main $(objs)
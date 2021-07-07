APP=dym
CPPFLAGS=-I./include -I/usr/local/include
LDFLAGS=-L/usr/local/lib -lglfw -ldl
__DIR__="\"$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))/src/GLSL/\""
DEBUG=-D__GL_DEBUG_FLAG__

CC=g++-11

ALL: src/debug.o src/main.o src/glad.o src/model.o 
	$(CC) -o $(APP) $(LDFLAGS) $(DEFINE) $?

%.o: %.c
	$(CC) $(CPPFLAGS) -D__DIR__=$(__DIR__) $(DEBUG) -c $< -o $@ 
%.o: %.cpp
	$(CC) $(CPPFLAGS) -D__DIR__=$(__DIR__) $(DEBUG) -c $< -o $@ 

clean:
	rm src/*.o $(APP)

.PONY: clean

# CPP (main source)
SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
# C files (libs)
CSOURCES=src/stb_image.c src/stb_image_write.c
COBJECTS=$(CSOURCES:.c=.o)
CPPFLAGS+=-std=c++11 -O3 -Wall -Iinc/
CFLAGS+=-O2 -Iinc/

strahl: $(OBJECTS) $(COBJECTS)
	g++ $(CPPFLAGS) $^ -o $@

%.o: %.c
	gcc $(CFLAGS) $^ -c -o $@

clean:
	rm $(OBJECTS)
SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
CPPFLAGS+=-std=c++11 -O3 -Wall -Iinc/
CFLAGS+=-O2 -Iinc/

strahl: $(OBJECTS) src/stb_image.o
	g++ $(CPPFLAGS) $^ -o $@

src/stb_image.o: src/stb_image.c

clean:
	rm $(OBJECTS)
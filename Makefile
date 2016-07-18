SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
CPPFLAGS+=-std=c++11 -O3 -Wall -Iinc/

strahl: $(OBJECTS)
	g++ $(CPPFLAGS) $^ -o $@

clean:
	rm $(OBJECTS)
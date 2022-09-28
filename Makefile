CC=g++ -Wno-everything
CFLAGS=-c -Wall -Wextra -pedantic -std=c++11 -m64 -g
LDFLAGS=
LIBFLAGS=-lsfml-graphics -lsfml-window -lsfml-system
SOURCES=main.cpp engine.cpp snake.cpp fruit.cpp scoreboard.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=s


all: $(SOURCES) $(EXECUTABLE)
        
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@ $(LIBFLAGS)

clean:
	rm *.o $(EXECUTABLE)


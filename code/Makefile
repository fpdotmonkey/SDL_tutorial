LDFLAGS = -L/usr/local/lib

LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
DEPS = LTexture.o LTimer.o dot.o LWindow.o

CFLAGS = $(shell sdl2-config --cflags) -g

SOURCES=testLWindow.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=sdltut20

all: $(SOURCES) $(EXECUTABLE) $(DEPS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

$(EXECUTABLE): $(OBJECTS) $(DEPS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

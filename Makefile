LDFLAGS=-L/usr/local/lib
LIBS=-lSDL2 -lSDL2_image -lSDL2_ttf
CFLAGS=$(shell sdl2-config --cflags)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

hello_window: hello_window.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

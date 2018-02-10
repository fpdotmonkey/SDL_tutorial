LDFLAGS = -L/usr/local/lib

LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
DEPS = LTexture.o LTimer.o dot.o

CFLAGS = $(shell sdl2-config --cflags)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

sdl_tut20: sdl_tut20.o $(DEPS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

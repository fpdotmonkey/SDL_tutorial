all:
	clang -o hello_window hello_window.c $(shell sdl2-config --cflags --libs) \
	$(shell sdl2_image-config --cflags --libs)

Adding SDL2_image has resulted in the error "Undefined symbols for architecture x86_64:".  Need to figure out how Makefiles work with respect to these things.

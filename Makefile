# Source files
SRCS = test.cpp module/sdl_init.cpp module/menu.cpp module/anim.cpp module/sprite.cpp module/background.cpp
# Library files
LIBS = libSDL2.a libSDL2_image.a libSDL2_ttf.a -Wall

all: test

test: test.cpp
	@echo "g++ test.cpp"
	@g++ $(SRCS) $(LIBS) -otest 

clean:
	rm -f test

all: joystick.o glhelper.o serial_talk.o
	g++ my_example.cc joystick.o glhelper.o serial_talk.o -o my_example -L/usr/lib `sdl-config --cflags --libs` -lSDL_ttf -lSDL_image -lGL -lGLU -lpthread

joystick.o: joystick.h joystick.cc
	g++ joystick.cc -c -o joystick.o

glhelper.o: glhelper.h glhelper.cc
	g++ glhelper.cc -c -o glhelper.o

serial_talk.o: serial_talk.h serial_talk.cpp
	g++ -c serial_talk.cpp -lserialib -o serial_talk.o


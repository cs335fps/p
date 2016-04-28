# cs335 lab1
# to compile your project, type make and press enter

all: lab1 project

lab1: openglBlank.cpp
	g++ -o lab1 openglBlank.cpp -Wall -lX11 -lGL -lGLU -lm
	
project: project.cpp game.cpp nick_view.cpp nick_input.cpp nick_vec.cpp cenright.cpp
	g++ -g project.cpp game.cpp nick_view.cpp nick_input.cpp nick_vec.cpp charlesE.cpp -o project -Wall -lX11 -lGL -lGLU -lm

clean:
	rm -f rex project lab1
	rm -f *.o
	rm -f *~


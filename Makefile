# cs335 lab1
# to compile your project, type make and press enter

all: project
	
project: project.cpp game.cpp input.cpp view.cpp vec.cpp charlesE.cpp nick.cpp oalTest.cpp RoyB.cpp
	g++ -g project.cpp game.cpp view.cpp input.cpp vec.cpp charlesE.cpp nick.cpp oalTest.cpp RoyB.cpp -o project -Wall -lX11 -lGL -lGLU -lm

clean:
	rm -f rex project lab1
	rm -f *.o
	rm -f *~


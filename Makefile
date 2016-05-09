# cs335 lab1
# to compile your project, type make and press enter

all: project
	
project: project.cpp game.cpp input.cpp view.cpp vec.cpp charlesE.cpp nickG.cpp royB.cpp lizandroP.cpp
	g++ -g project.cpp game.cpp view.cpp input.cpp vec.cpp charlesE.cpp nickG.cpp royB.cpp lizandroP.cpp -o project -Wall -lX11 -lGL -lGLU -lm -lalut -lopenal 

clean:
	rm -f rex project
	rm -f *.o
	rm -f *~


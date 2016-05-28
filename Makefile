# cs335 lab1
# to compile your project, type make and press enter
CFLAGS = -I ./include
all: project
	
project: project.cpp game.cpp input.cpp view.cpp vec.cpp charlesE.cpp nickG.cpp royB.cpp lizandroP.cpp ppm.cpp web.cpp
	g++ -g $(CFLAGS) project.cpp game.cpp view.cpp input.cpp vec.cpp charlesE.cpp nickG.cpp royB.cpp lizandroP.cpp libggfonts.a web.cpp -Wall -lX11 -lGL -lGLU -lm -lalut -lopenal -ofast -o project

clean:
	rm -f project
	rm -f *.o
	rm -f *~


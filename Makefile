# cs335 lab1
# to compile your project, type make and press enter
CFLAGS = -I ./include
all: project
	
project: project.cpp game.cpp input.cpp view.cpp vec.cpp charlesE.cpp nickG.cpp royB.cpp lizandroP.cpp ppm.cpp log.cpp
	g++ -g $(CFLAGS) project.cpp game.cpp view.cpp input.cpp vec.cpp charlesE.cpp nickG.cpp royB.cpp lizandroP.cpp log.cpp libggfonts.a -Wall -lX11 -lGL -lGLU -lm -lalut -lopenal -ofast -o project

clean:
	rm -f rex project
	rm -f *.o
	rm -f *~


# cs335 lab1
# to compile your project, type make and press enter

#all: lab1

lab1: openglBlank.cpp
	g++ -o lab1 openglBlank.cpp -Wall -lX11 -lGL -lGLU -lm

clean:
	rm -f rex
	rm -f *.o
	rm -f *~


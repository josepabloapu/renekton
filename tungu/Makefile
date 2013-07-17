LFLAGS = -L /usr/local/lib
INCPATH = -I /usr/local/include 
LIBS = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

all: 	
	g++ -c tunguV4.cpp $(INCPATH)
	g++ tunguV4.o -o TunguV4 `pkg-config --cflags --libs opencv` $(LFLAGS) $(LIBS) 
	./TunguV4
clean:
	

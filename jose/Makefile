LFLAGS = -L /usr/local/lib
INCPATH = -I /usr/local/include 
LIBS = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

all: 	
	g++ -c game.cpp $(INCPATH)
	g++ game.o -o pong `pkg-config --cflags --libs opencv` $(LFLAGS) $(LIBS) 
	./pong
clean:
	



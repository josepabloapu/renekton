LFLAGS = -L/home/nano/local/renekton/lib/sfml/lib 
INCPATH = -I /home/nano/local/renekton/lib/sfml/include 
LIBS = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

all: 	
	gcc -c game.cpp $(INCPATH)
	gcc game.o -o pong `pkg-config --cflags --libs opencv` $(LFLAGS) $(LIBS) 
	./pong
clean:
	


LFLAGS = -L/home/nano/local/renekton/lib/sfml/lib 
INCPATH = -I /home/nano/local/renekton/lib/sfml/include 
LIBS = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

all: 	
	gcc -c game6.cpp $(INCPATH)
	gcc game6.o -o pong6 `pkg-config --cflags --libs opencv` $(LFLAGS) $(LIBS) 
	./pong6
clean:
	



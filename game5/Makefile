
LFLAGS = -L/home/nano/local/renekton/lib/sfml/lib 
INCPATH = -I /home/nano/local/renekton/lib/sfml/include 
LIBS = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

all: 	
	gcc -c nuevo1.cpp $(INCPATH)
	gcc nuevo1.o -o nuevo1 `pkg-config --cflags --libs opencv` $(LFLAGS) $(LIBS) 
	./nuevo1
clean:
	



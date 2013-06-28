all: 	
	g++ -c game.cpp -I ~/local/renekton/lib/sfml/include
	g++ game.o -o pong -L ~/local/renekton/lib/sfml/lib -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
	./pong
clean:
	


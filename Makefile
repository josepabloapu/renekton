all: 	
	g++ -c game.cpp -I /home/nano/local/renekton/lib/sfml/include
	g++ game.o -o pong -L /home/nano/local/renekton/lib/sfml/lib -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system `pkg-config --cflags --libs opencv`
	./pong
clean:
	


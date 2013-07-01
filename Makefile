all: 	
<<<<<<< HEAD
	g++ -c game.cpp -I /home/nano/local/renekton/lib/sfml/include
	g++ game.o -o pong -L /home/nano/local/renekton/lib/sfml/lib -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system `pkg-config --cflags --libs opencv`
=======
	g++ -c game.cpp -I ~/local/renekton/lib/sfml/include
	g++ game.o -o pong -L ~/local/renekton/lib/sfml/lib -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
>>>>>>> 38b19002bd484a2627d6e06e06fe54b22a2a9474
	./pong
clean:
	


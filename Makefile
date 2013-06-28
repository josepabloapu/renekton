all: 	
	g++ -c game.cpp -I /home/<your_user_name>/local/sfml2/include
	g++ game.o -o pong -L /home/<your_user_name>/local/sfml2/lib -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
	./pong
clean:
	


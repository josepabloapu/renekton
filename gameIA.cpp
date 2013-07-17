#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <sstream>
#include <string>

#define RENDERWIDTH 800
#define RENDERHEIGHT 600

std::string convertInt(int number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

int main()
{
	//window properties
	sf::RenderWindow pong(sf::VideoMode(RENDERWIDTH, RENDERHEIGHT, 32), "GameName", sf::Style::Fullscreen);
	pong.setMouseCursorVisible(false);
	pong.setFramerateLimit(60);

	//music
	sf::Music bgm;
	bgm.openFromFile("musica.wav");
	bgm.setPitch(1.5);
	bgm.setLoop(true);
	bgm.play();

	//sound
	sf::SoundBuffer buffer1;
	buffer1.loadFromFile("/usr/lib/libreoffice/share/gallery/sounds/beam.wav");
	sf::Sound bounce;
	bounce.setBuffer(buffer1);
	sf::SoundBuffer buffer2;
	buffer2.loadFromFile("/usr/lib/libreoffice/share/gallery/sounds/beam2.wav");
	sf::Sound point;
	point.setBuffer(buffer2);	
	sf::SoundBuffer buffer3;
	buffer3.loadFromFile("/usr/lib/libreoffice/share/gallery/sounds/beam.wav"); //perfecti hit!!!!!
	sf::Sound perfecthit;
	perfecthit.setBuffer(buffer3);

	//player 1 properties
	int p1Len = 80;
	sf::RectangleShape player1(sf::Vector2f(15, p1Len));
	player1.setFillColor(sf::Color(0, 0, 255));
	player1.setPosition(0, RENDERHEIGHT / 2 - player1.getSize().y / 2);
	int player1Score = 0;
	

	//player 2 properties
	int p2Len = 80;
	sf::RectangleShape player2(sf::Vector2f(15, p2Len));
	player2.setFillColor(sf::Color(0, 255, 0));
	player2.setPosition(RENDERWIDTH - player2.getSize().x, RENDERHEIGHT / 2 - player2.getSize().y / 2);
	int player2Score = 0;
	

	//ball properties
	sf::CircleShape ball(7, 25);
	ball.setFillColor(sf::Color(255,255,255));
	ball.setPosition(RENDERWIDTH / 2 - ball.getRadius(), RENDERHEIGHT / 2 - ball.getRadius());
	float BALLSPEED = 2;
	float ballVelX = -BALLSPEED, ballVelY = -BALLSPEED;
	float ballX = RENDERWIDTH / 2 - ball.getRadius(), ballY = RENDERHEIGHT / 2 - ball.getRadius();
	float ballDiameter = ball.getRadius() * 2;
	
	//BONUS properties 
	/*
	sf::CircleShape bonus(10, 25);
	ball.setFillColor(sf::Color(0,255,0));
	ball.setPosition(RENDERWIDTH / 2 , RENDERHEIGHT / rand ()%10+1);
	float BONUSSPEED = 2;
	float bonusVelX = -BONUSSPEED, bonusVelY = -BONUSSPEED;
	float bonusX = RENDERWIDTH / 2 - bonus.getRadius(), bonusY = RENDERHEIGHT / 2 - bonus.getRadius();
	float bonusDiameter = bonus.getRadius() * 2;
	*/
	//score text
	sf::Font font;

	font.loadFromFile("/usr/share/cups/fonts/FreeMonoOblique.ttf");

	font.loadFromFile("/usr/share/fonts/truetype/ttf-liberation/LiberationSerif-Bold.ttf");

	sf::Text score1("0", font, 80);
	score1.setPosition(RENDERWIDTH / 4, 0);
	sf::Text score2("0", font, 80);
	score2.setPosition(3 * RENDERWIDTH / 4, 0);
	
	//game loop
	while(pong.isOpen())
	{
		sf::Event event;
		while(pong.pollEvent(event))
		{
			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				pong.close();
		}

		//player 1 movement
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
			
			player1.move(0, -10);}

		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
			player1.move(0, 10);
		}
		//MOVIMIENTO GOLPE PLAYER1
	
		if (player1.getPosition().x <= 10){
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
			
			player1.move(10, 0);
			
			
		}}

		//player 2 movement

		if(ball.getPosition().y < player1.getPosition().y)
			player2.move(0, -5);
		if(ball.getPosition().y > player1.getPosition().y)
			player2.move(0, 5);
		if(ball.getPosition().y == player1.getPosition().y)
			player2.move(-10, 0);
		
		//MOVIMIENTO GOLPE PLAYER2
		

		//player 1 and wall collision
		if(player1.getPosition().y <= 0)
			player1.setPosition(0, 0);
		if(player1.getPosition().y >= RENDERHEIGHT - player1.getSize().y)
			player1.setPosition(0, RENDERHEIGHT - player1.getSize().y);

		//PLAYER1 AND WALL BACK COLLISION
		if(player1.getPosition().x != 0)
			player1.move(-1,0);
		
		//PLAYER2 AND WALL BACK COLLISION
		if(player2.getPosition().x != RENDERWIDTH-player2.getSize().x)
			player2.move(1,0);
		
		//player 2 and wall collision
		if(player2.getPosition().y <= 0)
			player2.setPosition(RENDERWIDTH - player2.getSize().x, 0);
		if(player2.getPosition().y >= RENDERHEIGHT - player2.getSize().y)
			player2.setPosition(RENDERWIDTH - player2.getSize().x, RENDERHEIGHT - player2.getSize().y);

		//ball and wall collision
		if(ball.getPosition().y <= 0 || ball.getPosition().y >= RENDERHEIGHT - ballDiameter)
		{
			ballVelY *= -1;
			bounce.play();
		}

		//ball and player 1 collision
		if (ball.getPosition().x <= player1.getPosition().x + player1.getSize().x)
		{
			if ((ball.getPosition().y + ballDiameter >= player1.getPosition().y && ball.getPosition().y + ballDiameter <= player1.getPosition().y + player1.getSize().y) || ball.getPosition().y <= player1.getPosition().y + player1.getSize().y && ball.getPosition().y >= player1.getPosition().y){


	if (player1.getPosition().x > 14){

				ballVelX = (ballVelX - 2) * -1;
				ball.setFillColor(sf::Color(255,0,0));
				perfecthit.play();
		
			}

	else if (player1.getPosition().x <= 14){

				ballVelX = (ballVelX - 1) * -1;
				ball.setFillColor(sf::Color(0,0,255));
				bounce.play();
}
			

			}

			else
			{
				point.play();
				player2Score += 1;  
				ballX = RENDERWIDTH / 2 - ball.getRadius();
				if (BALLSPEED < 8){
					BALLSPEED += 0.2;
					}
				ballVelX = BALLSPEED;
				score2.setString(convertInt(player2Score));
				score2.setPosition(3 * RENDERWIDTH / 4 - score2.getLocalBounds().width, 0);
				if (p2Len > 30)
					p2Len -= 10;
				player2.setSize(sf::Vector2f(15, p2Len));
				if (p1Len < 80)
					p1Len += 10;
				player1.setSize(sf::Vector2f(15, p1Len));
				
			}
		}

		//ball and player 2 collision
		if (ball.getPosition().x + ballDiameter >= player2.getPosition().x)
		{
			if ((ball.getPosition().y + ballDiameter >= player2.getPosition().y && ball.getPosition().y + ballDiameter <= player2.getPosition().y + player2.getSize().y) || ball.getPosition().y <= player2.getPosition().y + player2.getSize().y && ball.getPosition().y >= player2.getPosition().y)
			{


	if (player2.getPosition().x < (RENDERWIDTH-9-player2.getSize().x)){

				ballVelX = (ballVelX + 2) * -1;
				ball.setFillColor(sf::Color(255,0,0));
				perfecthit.play();
				
			
			}

	else if (player2.getPosition().x >= (RENDERWIDTH-9-player2.getSize().x)){

				ballVelX = (ballVelX + 1) * -1;
				ball.setFillColor(sf::Color(0,255,0));
				bounce.play();
			}
		}
				
			
			else
			{
				point.play();
				player1Score += 1;
				ballX = RENDERWIDTH / 2 - ball.getRadius();
				if (BALLSPEED < 8)
					BALLSPEED += 0.5;
				ballVelX = -BALLSPEED;
				score1.setString(convertInt(player1Score));
				if (p1Len > 30)
					p1Len -= 10;
				player1.setSize(sf::Vector2f(15, p1Len));
				if (p2Len < 80)
					p2Len += 10;
				player2.setSize(sf::Vector2f(15, p2Len));
			}
		}

		//ball position update
		ballX += ballVelX;
		ballY += ballVelY;
		ball.setPosition(ballX, ballY);

		//render updates
		pong.clear();
		pong.draw(score1);
		pong.draw(score2);
	
		pong.draw(player1);
		pong.draw(player2);
		pong.draw(ball);
		pong.display();
	}

	return 0;
} 

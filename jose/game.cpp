#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#define RENDERWIDTH 800
#define RENDERHEIGHT 600

std::string convertInt(int number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

void counter(int seconds)
{
	clock_t endTurn;
	endTurn = clock() + seconds * CLOCKS_PER_SEC;
	while (clock() < endTurn){}
  
}
    
void move()
{
	int countDown;
	for (countDown = 15 ; countDown > 0; countDown--)
	{
		counter(1);
	}
}

int main()
{
	//window properties
	sf::RenderWindow pong(sf::VideoMode(RENDERWIDTH, RENDERHEIGHT, 32), "GameName"/*, sf::Style::Fullscreen*/);
	pong.setMouseCursorVisible(false);
	pong.setFramerateLimit(60);

	//music
	sf::Music bgm;
	bgm.openFromFile("multimedia/audio/background.wav");
	bgm.setPitch(1.5);
	bgm.setLoop(true);
	bgm.play();

	//sound
	sf::SoundBuffer buffer1;
	buffer1.loadFromFile("multimedia/audio/bounce.wav");
	sf::Sound bounce;
	bounce.setBuffer(buffer1);
	sf::SoundBuffer buffer2;
	buffer2.loadFromFile("multimedia/audio/point.wav");
	sf::Sound point;
	point.setBuffer(buffer2);
	
	//ncp properties
	sf::RectangleShape ncp(sf::Vector2f(5, RENDERHEIGHT / 1.6));
	ncp.setFillColor(sf::Color(50, 50, 50));
	ncp.setPosition(RENDERWIDTH / 2, RENDERHEIGHT / 2 - (RENDERHEIGHT / 1.6) / 2 );

	//player 1 properties
	int p1Len = 100;
	sf::RectangleShape player1(sf::Vector2f(10, p1Len));
	player1.setFillColor(sf::Color(0, 0, 255));
	player1.setPosition(0, RENDERHEIGHT / 2 - player1.getSize().y / 2);
	int player1Score = 0;

	//player 2 properties
	int p2Len = 100;
	sf::RectangleShape player2(sf::Vector2f(10, p2Len));
	player2.setFillColor(sf::Color(255, 0, 0));
	player2.setPosition(RENDERWIDTH - player2.getSize().x, RENDERHEIGHT / 2 - player2.getSize().y / 2);
	int player2Score = 0;

	//ball properties
	sf::CircleShape ball(10, 25);
	ball.setFillColor(sf::Color(255, 255, 255));
	ball.setPosition(RENDERWIDTH / 2 - ball.getRadius(), RENDERHEIGHT / 2 - ball.getRadius());
	float BALLSPEED = 2;
	float ballVelX = -BALLSPEED, ballVelY = -BALLSPEED;
	float ballX = RENDERWIDTH / 2 - ball.getRadius(), ballY = RENDERHEIGHT / 2 - ball.getRadius();
	float ballDiameter = ball.getRadius() * 2;
	
	//score-timer text
	sf::Font font;
	font.loadFromFile("fonts/LiberationSerif-Regular.ttf");
	sf::Text score1("0", font, 80);
	score1.setPosition(RENDERWIDTH / 4, 0);
	sf::Text score2("0", font, 80);
	score2.setPosition(3 * RENDERWIDTH / 4 - score2.getLocalBounds().width, 0);
	sf::Text timer1("", font, 80);
	timer1.setPosition(0 , 5 * RENDERHEIGHT / 6);
	sf::Text timer2("", font, 80);
	timer2.setPosition(RENDERWIDTH / 2 - timer2.getLocalBounds().width, 5 * RENDERHEIGHT / 6);
	int time1 = 0;
	int time2 = 0;
	
	//gameover
	sf::Text gameover("GAME OVER", font, 120);
	gameover.setColor(sf::Color::Red);
	gameover.setPosition(0, RENDERHEIGHT / 3);
	
	
	//game loop
	while(player1Score + player2Score != 3)
	{
		sf::Event event;
		while(pong.pollEvent(event))
		{
			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				pong.close();
		}
		

		//score for player one winning
		if(player1Score == player2Score + 1)
		{
		    timer1.setString(convertInt(time1 += 2));
		    timer2.setString(convertInt(time2 += 1));
		}
		
		if(player1Score == player2Score + 2)
		{
		    timer1.setString(convertInt(time1 += 4));
		    timer2.setString(convertInt(time2 += 1));
		}
		
		if(player1Score == player2Score + 3)
		{
		    timer1.setString(convertInt(time1 += 8));
		    timer2.setString(convertInt(time2 += 1));
		}
		
		if(player1Score == player2Score + 4)
		{
		    timer1.setString(convertInt(time1 += 16));
		    timer2.setString(convertInt(time2 += 1));
		}
		
		if(player1Score == player2Score + 5)
		{
		    timer1.setString(convertInt(time1 += 32));
		    timer2.setString(convertInt(time2 += 1));
		}
		
		if(player1Score == player2Score + 6)
		{
		    timer1.setString(convertInt(time1 += 64));
		    timer2.setString(convertInt(time2 += 1));
		}
		
		//score on equals
		if(player1Score == player2Score)
		{
		    timer1.setString(convertInt(time1 += 1));
		    timer2.setString(convertInt(time2 += 1));
		}
		
		//score for player two winning
		if(player2Score == player1Score + 1)
		{
		    timer2.setString(convertInt(time2 += 2));
		    timer1.setString(convertInt(time1 += 1));
		}
		
		if(player2Score == player1Score + 2)
		{
		    timer2.setString(convertInt(time2 += 4));
		    timer1.setString(convertInt(time1 += 1));
		}
		
		if(player2Score == player1Score + 3)
		{
		    timer2.setString(convertInt(time2 += 8));
		    timer1.setString(convertInt(time1 += 1));
		}
		
		if(player2Score == player1Score + 4)
		{
		    timer2.setString(convertInt(time2 += 16));
		    timer1.setString(convertInt(time1 += 1));
		}
		
		if(player2Score == player1Score + 5)
		{
		    timer2.setString(convertInt(time2 += 32));
		    timer1.setString(convertInt(time1 += 1));
		}
		
		if(player2Score == player1Score + 6)
		{
		    timer2.setString(convertInt(time2 += 64));
		    timer1.setString(convertInt(time1 += 1));
		}
		
		//player 1 movement
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			player1.move(0, -10);
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			player1.move(0, 10);

		//player 2 movement
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			player2.move(0, -10);
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			player2.move(0, 10);

		//player 1 and wall collision
		if(player1.getPosition().y <= 0)
			player1.setPosition(0, 0);
		if(player1.getPosition().y >= RENDERHEIGHT - player1.getSize().y)
			player1.setPosition(0, RENDERHEIGHT - player1.getSize().y);

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
			if ((ball.getPosition().y + ballDiameter >= player1.getPosition().y 
			&& ball.getPosition().y + ballDiameter <= player1.getPosition().y + player1.getSize().y) 
			|| ball.getPosition().y <= player1.getPosition().y + player1.getSize().y 
			&& ball.getPosition().y >= player1.getPosition().y)
			{
				ballVelX = (ballVelX - 1) * -1;
				bounce.play();
			}
			else
			{
				point.play();
				player2Score += 1;
				ballX = RENDERWIDTH / 2 - ball.getRadius();
				if (BALLSPEED < 8)
					BALLSPEED += 0.5;
				ballVelX = BALLSPEED;
				score2.setString(convertInt(player2Score));
				score2.setPosition(3 * RENDERWIDTH / 4 - score2.getLocalBounds().width, 0);
				if (p2Len > 30)
					p2Len -= 10;
				player2.setSize(sf::Vector2f(10, p2Len));
				if (p1Len < 80)
					p1Len += 10;
				player1.setSize(sf::Vector2f(10, p1Len));
			}
		}

		//ball and player 2 collision
		if (ball.getPosition().x + ballDiameter >= player2.getPosition().x)
		{
			if ((ball.getPosition().y + ballDiameter >= player2.getPosition().y 
			&& ball.getPosition().y + ballDiameter <= player2.getPosition().y + player2.getSize().y) 
			|| ball.getPosition().y <= player2.getPosition().y + player2.getSize().y 
			&& ball.getPosition().y >= player2.getPosition().y)
			{
				ballVelX = (ballVelX + 1) * -1;
				bounce.play();
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
				player1.setSize(sf::Vector2f(10, p1Len));
				if (p2Len < 80)
					p2Len += 10;
				player2.setSize(sf::Vector2f(10, p2Len));
			}
		}
		
		//ball position update
		ballX += ballVelX;
		ballY += ballVelY;
		ball.setPosition(ballX, ballY);
		
		//render updates
		pong.clear();
		pong.draw(score1);
		pong.draw(timer1);
		pong.draw(timer2);
		pong.draw(score2);
		pong.draw(player1);
		pong.draw(player2);
		pong.draw(ball);
		pong.draw(ncp);
		pong.display();
		
		while(player1Score + player2Score == 3)
		{
			if(player1Score > player2Score)
				timer1.setString(convertInt(time1 += 10000));
			if(player1Score < player2Score)
				timer2.setString(convertInt(time2 += 10000));
		
			pong.clear(sf::Color::Black);
			pong.draw(score1);
			pong.draw(timer1);
			pong.draw(timer2);
			pong.draw(score2);
			pong.draw(gameover);
			pong.display();

			counter(3);
			break;
		}
		
	}
	
	std::stringstream ss;
	ss.str (timer1.getString());
	std::string scorePlayer1 = ss.str();
	ss.str (timer2.getString());
	std::string scorePlayer2 = ss.str();
	std::cout << "Final Score:" << '\n';
	std::cout << "Player1: " + scorePlayer1 << '\n';
	std::cout << "Player2: " + scorePlayer2 << '\n';

	
	std::ofstream myfile ("highscores.txt", std::ofstream::in | std::ofstream::out | std::ofstream::app);
  	if (myfile.is_open())
  	{
  		myfile << scorePlayer1 << std::endl;
    		myfile << scorePlayer2 << std::endl;
    		myfile.close();
  	}
  	else std::cout << "Unable to open file";
	
	return 0;
} 

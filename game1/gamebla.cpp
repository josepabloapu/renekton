
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

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			player2.move(0, -10);
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			player2.move(0, 10);
		
		//MOVIMIENTO GOLPE PLAYER2
			
		if (player2.getPosition().x >= RENDERWIDTH-player2.getSize().x-10){
			  
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
			
			player2.move(-10, 0);
			
			
		}}

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
ball.setFillColor(sf::Color(255,255,255));
				point.play();
				player2Score += 1;  
				ballX = RENDERWIDTH / 2 - ball.getRadius();
				if (BALLSPEED < 8){
					BALLSPEED += 0.2;
					}
				ballVelX = BALLSPEED;
				score2.setString(convertInt(player2Score));
				score2.setPosition(3 * RENDERWIDTH / 4 - score2.getLocalBounds().width, 0);

				if (p2Len > 40)
					p2Len -= 10;
				player2.setSize(sf::Vector2f(15, p2Len));
				if (p1Len < 100)
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
ball.setFillColor(sf::Color(255,255,255));
				point.play();
				player1Score += 1;
				ballX = RENDERWIDTH / 2 - ball.getRadius();
				if (BALLSPEED < 8)
					BALLSPEED += 0.5;
				ballVelX = -BALLSPEED;
				score1.setString(convertInt(player1Score));
				if (p1Len > 40)
					p1Len -= 10;
				player1.setSize(sf::Vector2f(15, p1Len));
				if (p2Len < 100)
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

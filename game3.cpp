#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <sstream>
#include <string>

#define RENDERWIDTH 800
#define RENDERHEIGHT 600

IplImage* imgTracking;
IplImage* imgThresh;
int lastX = -1;
int lastY = -1;


//This function threshold the HSV image and create a binary image
IplImage* GetThresholdedImage(IplImage* imgHSV){  
     
    imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);

    cvInRangeS(imgHSV, cvScalar(17,90,154), cvScalar(61,219,206), imgThresh); 

    return imgThresh;
}

int trackObject(IplImage* imgThresh){
  int Abajo=0;
   CvCapture* capture =0;       
      capture = cvCaptureFromCAM(0);
      if(!capture){
printf("Capture failure\n");

      }
      
      IplImage* frame=0;
      frame = cvQueryFrame(capture);           
      
  
     //create a blank image and assigned to 'imgTracking' which has the same size of original video
     imgTracking=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U, 3);
     cvZero(imgTracking); //covert the image, 'imgTracking' to black

     //cvNamedWindow("Video");     
    // cvNamedWindow("Ball");

      //iterate through each frames of the video     
      while(true){

            frame = cvQueryFrame(capture);           
            if(!frame) break;
            frame=cvCloneImage(frame); 
            
           cvSmooth(frame, frame, CV_GAUSSIAN,3,3); //smooth the original image using Gaussian kernel

            IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 
            cvCvtColor(frame, imgHSV, CV_BGR2HSV); //Change the color format from BGR to HSV
            IplImage* imgThresh = GetThresholdedImage(imgHSV);
          
            cvSmooth(imgThresh, imgThresh, CV_GAUSSIAN,3,3); //smooth the binary image using Gaussian kernel
            
          //track the possition of the ball
          

            // Add the tracking image and the frame
          //cvAdd(frame, imgTracking, frame);

          //cvShowImage("Ball", imgThresh);           
           //cvShowImage("Video", frame);
           
           //Clean up used images
           cvReleaseImage(&imgHSV);
           cvReleaseImage(&imgThresh);            
           cvReleaseImage(&frame);

            //Wait 10mS
            int c = cvWaitKey(10);
            //If 'ESC' is pressed, break the loop
            if((char)c==27 ) break;      
      }

      cvDestroyAllWindows() ;
      cvReleaseImage(&imgTracking);
      cvReleaseCapture(&capture);     

  
// Calculate the moments of 'imgThresh'
CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
cvMoments(imgThresh, moments, 1);
double moment10 = cvGetSpatialMoment(moments, 1, 0);
double moment01 = cvGetSpatialMoment(moments, 0, 1);
double area = cvGetCentralMoment(moments, 0, 0);

     // if the area<1000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
if(area>1000){
        // calculate the position of the ball
int posX = moment10/area;
int posY = moment01/area;  
        //printf ("posX:%i, posY:%i, lastX:%i, lastY:%i", posX, posY, lastX, posY);
 if(lastX>=0 && lastY>=0 && posX>=0 && posY>=0)
{
// Draw a yellow line from the previous point to the current point
//cvLine(imgTracking, cvPoint(posX, posY), cvPoint(lastX, lastY), cvScalar(255,255,255), 4);




if (posY-lastY>20){Abajo=1;}


}

lastX = posX;
lastY = posY;
}

free(moments);
return Abajo;
}


std::string convertInt(int number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

int main()
{
  
  
       
     
	//window properties
	sf::RenderWindow pong(sf::VideoMode(RENDERWIDTH, RENDERHEIGHT, 32), "GameName");
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

	//player 1 properties
	int p1Len = 80;
	sf::RectangleShape player1(sf::Vector2f(10, p1Len));
	player1.setFillColor(sf::Color(0, 0, 255));
	player1.setPosition(0, RENDERHEIGHT / 2 - player1.getSize().y / 2);
	int player1Score = 0;

	//player 2 properties
	int p2Len = 80;
	sf::RectangleShape player2(sf::Vector2f(10, p2Len));
	player2.setFillColor(sf::Color(255, 0, 0));
	player2.setPosition(RENDERWIDTH - player2.getSize().x, RENDERHEIGHT / 2 - player2.getSize().y / 2);
	int player2Score = 0;

	//ball properties
	sf::CircleShape ball(5, 25);
	ball.setFillColor(sf::Color(255,255,255));
	ball.setPosition(RENDERWIDTH / 2 - ball.getRadius(), RENDERHEIGHT / 2 - ball.getRadius());
	float BALLSPEED = 2;
	float ballVelX = -BALLSPEED, ballVelY = -BALLSPEED;
	float ballX = RENDERWIDTH / 2 - ball.getRadius(), ballY = RENDERHEIGHT / 2 - ball.getRadius();
	float ballDiameter = ball.getRadius() * 2;
	
	//BONUS properties 
	/*
	sf::CircleRactangle bonus(10, 25);
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
	sf::Text score1("0", font, 80);
	score1.setPosition(RENDERWIDTH / 4, 0);
	sf::Text score2("0", font, 80);
	score2.setPosition(3 * RENDERWIDTH / 4 - score2.getLocalBounds().width, 0);

	//game loop
	while(pong.isOpen())
	{
		sf::Event event;
		while(pong.pollEvent(event))
		{
			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				pong.close();
		}
		
		int Abajo = trackObject(imgThresh);
		//player 1 movement
		
	
			
	 if(Abajo==1){
			player1.move(0, 10);
		}
		//MOVIMIENTO GOLPE PLAYER1	
		

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
			if ((ball.getPosition().y + ballDiameter >= player1.getPosition().y && ball.getPosition().y + ballDiameter <= player1.getPosition().y + player1.getSize().y) || ball.getPosition().y <= player1.getPosition().y + player1.getSize().y && ball.getPosition().y >= player1.getPosition().y)
			{
				ballVelX = (ballVelX - 1) * -1;
				ball.setFillColor(sf::Color(0,0,255));
				bounce.play();
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
				player2.setSize(sf::Vector2f(10, p2Len));
				if (p1Len < 80)
					p1Len += 10;
				player1.setSize(sf::Vector2f(10, p1Len));
				
			}
		}

		//ball and player 2 collision
		if (ball.getPosition().x + ballDiameter >= player2.getPosition().x)
		{
			if ((ball.getPosition().y + ballDiameter >= player2.getPosition().y && ball.getPosition().y + ballDiameter <= player2.getPosition().y + player2.getSize().y) || ball.getPosition().y <= player2.getPosition().y + player2.getSize().y && ball.getPosition().y >= player2.getPosition().y)
			{
				ballVelX = (ballVelX + 1) * -1;
				ball.setFillColor(sf::Color(255,0,0));
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
		pong.draw(score2);
		pong.draw(player1);
		pong.draw(player2);
		pong.draw(ball);
		pong.display();
	}

	return 0;
} 

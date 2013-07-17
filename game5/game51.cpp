#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <sstream>
#include <string>
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#define RENDERWIDTH 800
#define RENDERHEIGHT 600



int lastX1 = -1;
int lastY1 = -1;
int lastX2 = -1;
int lastY2 = -1;
int posX1;int posY1; 
int posX2;int posY2; 
CvMoments* moments = (CvMoments*)malloc(sizeof(CvMoments));
double moment10;
double moment01;
double area;
IplImage* imgTemp;
int caso =0;


IplImage* GetThresholdedImage(IplImage* imgHSV3, int H, int S, int V, int H1, int S1, int V1){  
     
    imgTemp=cvCreateImage(cvGetSize(imgHSV3),IPL_DEPTH_8U, 1);
    
    cvInRangeS(imgHSV3, cvScalar(H,S,V), cvScalar(H1,S1,V1), imgTemp); 
	
	 

    return imgTemp;
}

int trackObject(IplImage* imgThresh, int R, int G, int B, int player){



// Calculate the moments of 'imgThresh'


cvMoments(imgThresh, moments, 1);
moment10 = cvGetSpatialMoment(moments, 1, 0);
moment01 = cvGetSpatialMoment(moments, 0, 1);
area = cvGetCentralMoment(moments, 0, 0);


     // if the area<1000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 

if(area>1000){

// Draw a yellow line from the previous point to the current point

if (player ==1){

posX1 = moment10/area;
posY1 = moment01/area; 

 }

if (player ==2){

posX2 = moment10/area;
posY2 = moment01/area; 

 }

switch (player){ 
 
  case 1:

if (posY1-lastY1 > 20  ){
caso=-1;
}

if (posY1-lastY1<-20 ){
caso=1;
}

if (posX1-lastX1<-50 ){
caso=2;
}

if (posX1-lastX1>50 ){
caso=-2;
}



break;

  case 2:
    

if (posY2-lastY2 > 20 ){
caso= -1;
}

if (posY2-lastY2<-20 ){
caso= 1;
}

if (posX2-lastX2<-50 ){
caso= 2;
}

if (posX2-lastX2>50 ){
caso= -2;
}




break;

}

lastX1 = posX1;
lastY1 = posY1;
lastX2 = posX2;
lastY2 = posY2;

}


return caso;

}//fin funcion #2


std::string convertInt(int number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

int main()
{
	//window properties
	sf::RenderWindow pong(sf::VideoMode(RENDERWIDTH, RENDERHEIGHT, 32), "GameName"); //, sf::Style::Fullscreen
	pong.setMouseCursorVisible(false);
	pong.setFramerateLimit(24);
/*
	//music
	sf::Music bgm;
	bgm.openFromFile("musica.wav");
	bgm.setPitch(1.5);
	bgm.setLoop(true);
	bgm.play();
*/
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

	sf::Font font;

	font.loadFromFile("/usr/share/cups/fonts/FreeMonoOblique.ttf");

	font.loadFromFile("/usr/share/fonts/truetype/ttf-liberation/LiberationSerif-Bold.ttf");

	sf::Text score1("0", font, 80);
	score1.setPosition(RENDERWIDTH / 4, 0);
	sf::Text score2("0", font, 80);
	score2.setPosition(3 * RENDERWIDTH / 4, 0);
	
	//game loop
	//while(pong.isOpen())

/////////////////////Declaraciones////////////////////////////////


IplImage* frame=0;
IplImage* imgTracking;
CvCapture* capture =0; 

IplImage* imgThresh;
IplImage* imgThresh2;
sf::Event event;
int player100=0;
int player200=0;


 capture = cvCaptureFromCAM(1);


	for(int i=0;i<1000;++i)
	{
		

   		  if(!capture){
			printf("Capture failure\n");
				return -1;
      			}
      
      
      frame = cvRetrieveFrame(capture); 
          
      if(!frame) return -1;
  
     //create a blank image and assigned to 'imgTracking' which has the same size of original video
     imgTracking=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U, 3);
            

           cvSmooth(frame, frame, CV_GAUSSIAN,3,3); //smooth the original image using Gaussian kernel

	IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 

            cvCvtColor(frame, imgHSV, CV_BGR2HSV); //Change the color format from BGR to HSV

	imgThresh = GetThresholdedImage(imgHSV, 24, 0, 224, 46, 89, 256);

	cvReleaseImage(&imgHSV);
	delete(imgHSV);
	    
	     cvSmooth(imgThresh, imgThresh, CV_GAUSSIAN,3,3); //smooth the binary image using Gaussian kernel
	     
	     player100 = trackObject(imgThresh, 255, 0, 0, 1);

	     /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
          	


	IplImage* imgHSV2 = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 
	    
	      cvCvtColor(frame, imgHSV2, CV_BGR2HSV);

	imgThresh2 = GetThresholdedImage(imgHSV2, 24, 0, 224, 46, 89, 256);

  
	   cvReleaseImage(&imgHSV2);
	   delete(imgHSV2);

 		cvSmooth(imgThresh2, imgThresh2, CV_GAUSSIAN,3,3); //smooth the binary image using Gaussian kernel

		player200 = trackObject(imgThresh2, 0, 255, 0, 2);

/*
		//player 1 movement
		
		if( player100 == 1 ) {
			
			player1.move(0, -10);}

		else if(player100==-1){
			player1.move(0, 10);
		}
		//MOVIMIENTO GOLPE PLAYER1
	
		if (player1.getPosition().x <= 10){
		if(player100==2){
			
			player1.move(10, 0);
			
			
		}}

		//player 2 movement

		if(player200==1)
			player2.move(0, -10);
		else if(player200==-1)
			player2.move(0, 10);
		
		//MOVIMIENTO GOLPE PLAYER2
			
		if (player2.getPosition().x >= RENDERWIDTH-player2.getSize().x-10){
			  
		if(player200==-2){
			
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
		}//fin IF

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

					}//fin if elseh
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
				
				if (p2Len < 10)
					p2Len += 10;
				player2.setSize(sf::Vector2f(15, p2Len));
			}//fin else
		}//fin IF
*/
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

	 
		
	}//fin for

	   cvReleaseCapture(&capture); 
           cvReleaseImage(&imgThresh);
	   cvReleaseImage(&imgThresh2);             
           cvReleaseImage(&frame);   
	   
      	   cvReleaseImage(&imgTracking);


//cvReleaseImage(&imgTemp); revisar este puntero

	   delete(moments);

	return 0;
} 

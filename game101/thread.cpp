
extern "C"
 {
    #include <pthread.h>
    #include <unistd.h>
 }

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <sstream>
#include <string>
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#define RENDERWIDTH 800
#define RENDERHEIGHT 600

int player100=0;
int player200=0;

int lastX1 = -1;
int lastY1 = -1;
int lastX2 = -1;
int lastY2 = -1;
int posX1;int posY1; 
int posX2;int posY2; 


using namespace std ;

void  * function1(void * argument);
void  * function2(void * argument);

int main( void )
{
    pthread_t t1, t2 ; // declare 2 threads.
    pthread_create( &t1, NULL, function1,NULL); // create a thread running function1
    pthread_create( &t2, NULL, function2,NULL); // create a thread running function2

    // Because all created threads are terminated when main() finishes, we have
    // to give the threads some time to finish. Unfortunately for function1, main()
    // will give only 1 second, but function1 needs at least 2 seconds. So function1 will
    // probably be terminated before it can finish. This is a BAD way to manage threads.
    sleep(1);

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void * function1(void * argument)
{
    IplImage* imgTracking;

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

      
      

            frame = cvQueryFrame(capture);           
            

            frame=cvCloneImage(frame); 

            

           cvSmooth(frame, frame, CV_GAUSSIAN,3,3); //smooth the original image using Gaussian kernel

	IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 

            cvCvtColor(frame, imgHSV, CV_BGR2HSV); //Change the color format from BGR to HSV

	IplImage* imgThresh = GetThresholdedImage(imgHSV, 24, 0, 224, 46, 89, 256);
	    
	     cvSmooth(imgThresh, imgThresh, CV_GAUSSIAN,3,3); //smooth the binary image using Gaussian kernel
	     
	     player100 = trackObject(imgThresh, 255, 0, 0, 1);

	     /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
          	
	IplImage* imgHSV2 = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 
	    
	      cvCvtColor(frame, imgHSV2, CV_BGR2HSV);

	IplImage* imgThresh2 = GetThresholdedImage(imgHSV2, 24, 0, 224, 46, 89, 256);

 		cvSmooth(imgThresh2, imgThresh2, CV_GAUSSIAN,3,3); //smooth the binary image using Gaussian kernel

		player200 = trackObject(imgThresh2, 0, 255, 0, 2);

           
           //Clean up used images
           cvReleaseImage(&imgHSV);
	   cvReleaseImage(&imgHSV2);
           cvReleaseImage(&imgThresh);
	   cvReleaseImage(&imgThresh2);             
           cvReleaseImage(&frame);   
      	   cvReleaseImage(&imgTracking);

  	   free( imgHSV);
	   free( imgHSV2);
           free( imgThresh);
	   free( imgThresh2);             
           free( frame);   
      	   free( imgTracking);
      		cvDestroyAllWindows() ;
      
      		cvReleaseCapture(&capture);   
    sleep(2); // fall alseep here for 2 seconds...
   
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void * function2(void * argument)
{
    //window properties
	sf::RenderWindow pong(sf::VideoMode(RENDERWIDTH, RENDERHEIGHT, 32), "GameName"); //, sf::Style::Fullscreen
	pong.setMouseCursorVisible(false);
	pong.setFramerateLimit(24);

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
				
				if (p2Len < 10)
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

IplImage* GetThresholdedImage(IplImage* imgHSV, int H, int S, int V, int H1, int S1, int V1){  
     
    IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
    
    cvInRangeS(imgHSV, cvScalar(H,S,V), cvScalar(H1,S1,V1), imgThresh); 
	
	 

    return imgThresh;
}


int trackObject(IplImage* imgThresh, int R, int G, int B, int player){

 int caso =0;

// Calculate the moments of 'imgThresh'

CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
cvMoments(imgThresh, moments, 1);
double moment10 = cvGetSpatialMoment(moments, 1, 0);
double moment01 = cvGetSpatialMoment(moments, 0, 1);
double area = cvGetCentralMoment(moments, 0, 0);


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

switch (player){ //// revisar aqui  }}}}}}}}}
 
  case 1:

if (posY1-lastY1 > 20  ){printf ("Player%i: Abajo\n", player);
caso=-1;
}

if (posY1-lastY1<-20 ){printf ("Player%i: Arriba\n", player);
caso=1;
}

if (posX1-lastX1<-50 ){printf ("Player%i: Derecha\n", player);
caso=2;
}

if (posX1-lastX1>50 ){printf ("Player%i: Izquierda\n", player);
caso=-2;
}




//cvLine(imgTracking, cvPoint(posX1, posY1), cvPoint(lastX1, lastY1), cvScalar(R,G,B), 4);

break;

  case 2:
    

if (posY2-lastY2 > 20 ){printf ("Player%i: Abajo\n", player);
caso= -1;
}

if (posY2-lastY2<-20 ){printf ("Player%i: Arriba\n", player);
caso= 1;
}

if (posX2-lastX2<-50 ){printf ("Player%i: Derecha\n", player);
caso= 2;
}

if (posX2-lastX2>50 ){printf ("Player%i: Izquierda\n", player);
caso= -2;
}


//cvLine(imgTracking, cvPoint(posX2, posY2), cvPoint(lastX2, lastY2), cvScalar(R,G,B), 4);

break;

}

lastX1 = posX1;
lastY1 = posY1;
lastX2 = posX2;
lastY2 = posY2;
free(moments);
}


return caso;
}


std::string convertInt(int number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}


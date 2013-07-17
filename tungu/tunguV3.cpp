#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <sstream>
#include <string>
#include <fstream>

#define RENDERWIDTH 800
#define RENDERHEIGHT 600

int vel = 10;

void counter(int seconds)
{
	clock_t endTurn;
	endTurn = clock() + seconds * CLOCKS_PER_SEC;
	while (clock() < endTurn){}
  
}
    

std::string convertInt(int number)

  {
	std::stringstream ss;
	ss << number;
	return ss.str();
  }


  IplImage* imgTracking;
  int lastX1 = -1;
  int lastY1 = -1;
  int lastX2 = -1;
  int lastY2 = -1;
  int posX1;int posY1; int posX2;int posY2; 
  int player100;
  int player200;


//This function threshold the HSV image and create a binary image


  IplImage* GetThresholdedImage(IplImage* imgHSV3, int H, int S, int V, int H1, int S1, int V1){  
     
    IplImage* imgTemp=cvCreateImage(cvGetSize(imgHSV3),IPL_DEPTH_8U, 1);

    cvInRangeS(imgHSV3, cvScalar(H,S,V), cvScalar(H1,S1,V1), imgTemp); 

    return imgTemp;
  }



 int trackObject(IplImage* imgThresh, int R, int G, int B, int player){

    int caso;

    // Calculate the moments 

    CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));

    cvMoments(imgThresh, moments, 1);

    double moment10 = cvGetSpatialMoment(moments, 1, 0);

    double moment01 = cvGetSpatialMoment(moments, 0, 1);

    double area = cvGetCentralMoment(moments, 0, 0);

     // if the area<1000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 

       if(area>1000){

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

	///Quieto///

	if (posY1 < 240 && posY1 > 210){printf ("Player%i: Quieto\n", player); caso=0;}

	///Abajo///

	if (posY1 < 440 && posY1 > 400){printf ("Player%i: Abajo Rapido\n", player);caso=-5;}

	if (posY1 < 400 && posY1 > 360 ){printf ("Player%i: Abajo Medio\n", player);caso=-4;}

	if (posY1 < 360 && posY1 > 320){printf ("Player%i: Abajo \n", player);caso=-3;}

	if (posY1 < 320 && posY1 > 280 ){printf ("Player%i: Abajo Medio\n", player);caso=-2;}

	if (posY1 < 280 && posY1 > 240){printf ("Player%i: Abajo \n", player);caso=-1;}

	if (posY1 < 240 && posY1 > 210){printf ("Player%i: Abajo muy lento\n", player);caso=-6;}

	///Arriba///

	if (posY1 < 40 && posY1 > 0){printf ("Player%i: Arriba Rapido\n", player);caso=5;}
	
	if (posY1 < 80 && posY1 > 40){printf ("Player%i: Arriba Medio\n", player);caso=4;}

	if (posY1 < 120 && posY1 > 80){printf ("Player%i: Arriba\n", player);caso=3;}

	if (posY1 < 160 && posY1 > 120){printf ("Player%i: Arriba Medio\n", player);caso=2;}

	if (posY1 < 200 && posY1 > 160){printf ("Player%i: Arriba\n", player);caso=1;}
	
	if (posY1 < 210 && posY1 > 200){printf ("Player%i: Arriba muy lento\n", player);caso=6;}

	///Izq y Der///

	if (posX1-lastX1 < -50 && posX1-lastX1 > -100){printf ("Player%i: Derecha\n", player);caso=2;}

	if (posX1-lastX1 > 50 && posX1-lastX1 < 100){printf ("Player%i: Izquierda\n", player);caso=-2;}

	lastX1 = posX1;

	lastY1 = posY1;



  break;

  case 2:
    
	///Quieto///

	if (posY2 < 240 && posY2 > 210){printf ("Player%i: Quieto\n", player);caso=0;}

	///Abajo///

	if (posY2 < 440 && posY2 > 400){printf ("Player%i: Abajo Rapido\n", player);caso=-5;}

	if (posY2 < 400 && posY2 > 360){printf ("Player%i: Abajo Medio\n", player);caso=-4;}

	if (posY2 < 360 && posY2 > 320){printf ("Player%i: Abajo \n", player);caso=-3;}

	if (posY2 < 320 && posY2 > 280){printf ("Player%i: Abajo Medio\n", player);caso=-2;}

	if (posY2 < 280 && posY2 > 240){printf ("Player%i: Abajo \n", player);caso=-1;}

	if (posY2 < 240 && posY2 > 210){printf ("Player%i: Abajo muy lento\n", player);caso=-6;}

	///Arriba///

	if (posY2 < 40 && posY2 > 0){printf ("Player%i: Arriba Rapido\n", player);caso=5;}
	
	if (posY2 < 80 && posY2 > 40){printf ("Player%i: Arriba Medio\n", player);caso=4;}

	if (posY2 < 120 && posY2 > 80){printf ("Player%i: Arriba\n", player);caso=3;}

	if (posY2 < 160 && posY2 > 120){printf ("Player%i: Arriba Medio\n", player);caso=2;}

	if (posY2 < 200 && posY2 > 160){printf ("Player%i: Arriba\n", player);caso=1;}

	if (posY2 < 210 && posY2 > 200){printf ("Player%i: Arriba muy lento\n", player);caso=6;}
	
	///Izq y Der///

	if (posX2-lastX2<-50 && posX2-lastX2>-100){printf ("Player%i: Derecha\n", player);caso=22;}

	if (posX2-lastX2>50 && posX2-lastX2<100){printf ("Player%i: Izquierda\n", player);caso=-22;}

	lastX2 = posX2;

	lastY2 = posY2;

   break;

  } //fin switch

 } //fin area 1000

   free(moments);

  return caso;

}// fin funcion


int main(){
  
////////////////////GAME PROPERTIES///////////////////////////////////

	//window properties
	sf::RenderWindow pong(sf::VideoMode(RENDERWIDTH, RENDERHEIGHT, 32), "Tungu");
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
	sf::RectangleShape player1(sf::Vector2f(15, p1Len));
	player1.setFillColor(sf::Color(0, 0, 255));
	player1.setPosition(0, RENDERHEIGHT / 2 - player1.getSize().y / 2);
	int player1Score = 0;

	//player 2 properties
	int p2Len = 100;
	sf::RectangleShape player2(sf::Vector2f(15, p2Len));
	player2.setFillColor(sf::Color(0, 255, 0));
	player2.setPosition(RENDERWIDTH - player2.getSize().x, RENDERHEIGHT / 2 - player2.getSize().y / 2);
	int player2Score = 0;

	//ball properties
	sf::CircleShape ball(10, 25);
	ball.setFillColor(sf::Color(255, 255, 255));
	ball.setPosition(RENDERWIDTH / 2 - ball.getRadius(), RENDERHEIGHT / 2 - ball.getRadius());
	float BALLSPEED = 15;
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
	sf::Text timer1("", font, 50);
	timer1.setPosition(0 , 5 * RENDERHEIGHT / 6);
	sf::Text timer2("", font, 50);
	timer2.setPosition(RENDERWIDTH / 2 - timer2.getLocalBounds().width, 5 * RENDERHEIGHT / 6);
	int time1 = 0;
	int time2 = 0;
	
	//gameover
	sf::Text gameover("GAME OVER", font, 120);
	gameover.setColor(sf::Color::Red);
	gameover.setPosition(0, RENDERHEIGHT / 3);
	
		
	///////////////CAMERA FUNTIONS//////////////////////

      CvCapture* capture =0;   
    
      capture = cvCaptureFromCAM(0);


     	 if(!capture){

		printf("Capture failure\n");

		return -1;
      }
      
      IplImage* frame=0;

      frame = cvQueryFrame(capture);  
         
      if(!frame) return -1;
  
     //create a blank image and assigned to 'imgTracking' which has the same size of original video

     imgTracking=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U, 3);

     cvZero(imgTracking); //covert the image, 'imgTracking' to black

     //cvNamedWindow("Player1"); 
    
     ///cvNamedWindow("Player2");

      //iterate through each frames of the video
     
      while(player1Score + player2Score != 5){

		frame = cvQueryFrame(capture); 
          
		if(!frame) break;

		frame=cvCloneImage(frame); 
            
		cvSmooth(frame, frame, CV_GAUSSIAN,3,3); //smooth the original image using Gaussian kernel

			/////////////////// Player 1 ////////////////////

			IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 

			cvCvtColor(frame, imgHSV, CV_BGR2HSV); //Change the color format from BGR to HSV

			IplImage* imgThresh = GetThresholdedImage(imgHSV, 90, 117, 129, 103, 190, 256); //guante cyan
	 
			cvSmooth(imgThresh, imgThresh, CV_GAUSSIAN,3,3); //smooth the binary image using Gaussian kernel
	    
			player100 =  trackObject(imgThresh, 255, 0, 0, 1);

			/////////////////// Player 2 ////////////////////

			IplImage* imgHSV2 = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 
	    
			cvCvtColor(frame, imgHSV2, CV_BGR2HSV);//Change the color format from BGR to HSV

			IplImage* imgThresh2 = GetThresholdedImage(imgHSV2, 39, 105, 88, 56, 184, 200); //guante verde

 			cvSmooth(imgThresh2, imgThresh2, CV_GAUSSIAN,3,3); //smooth the binary image using Gaussian kernel

			player200 = trackObject(imgThresh2, 0, 255, 0, 2);

            	// Add the tracking image and the frame

		cvAdd(frame, imgTracking, frame);

		//cvShowImage("Player1", imgThresh);
 
		//cvShowImage("Player2", imgThresh2); 
           
		cvShowImage("Video", frame);
           
           //Clean up used images

           cvReleaseImage(&imgHSV);

	   cvReleaseImage(&imgHSV2);

           cvReleaseImage(&imgThresh);

	   cvReleaseImage(&imgThresh2);   
          
           cvReleaseImage(&frame);

            //Wait 10mS
            int c = cvWaitKey(10);

            //If 'ESC' is pressed, break the loop
            if((char)c==27 ) break;  


/////////////////////////////Scores///////////////////////////////////

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

//////////////////////////////Game////////////////////////////////////

		//player 1 movement muy lento
		
		if(player100==6)
			
			player1.move(0, -(vel-3));

		else if(player100==-6)

			player1.move(0, vel-3);

		//player 1 movement LENTO	
		
		if(player100==1)
			
			player1.move(0, -vel);

		else if(player100==-1)

			player1.move(0, vel);


		//player 1 movement MEDIO
		
		if(player100==2)
			
			player1.move(0, -(vel+5));

		else if(player100==-2)

			player1.move(0, vel+5);

		//player 1 movement RAPIDO
		
		if(player100==3)
			
			player1.move(0, -(vel+10));

		else if(player100==-3)

			player1.move(0, vel+10);

		//player 1 movement muy rapido
		
		if(player100==4)
			
			player1.move(0, -(vel+15));

		else if(player100==-4)

			player1.move(0, vel+15);

		//player 1 movement Extreme
		
		if(player100==5)
			
			player1.move(0, -(vel+20));

		else if(player100==-5)

			player1.move(0, vel+20);

		
		//MOVIMIENTO GOLPE PLAYER1

		if(player100==0)

			player1.move(0,0);
			
		
	
		if (player1.getPosition().x <= 10){

		if(player100==22)
			
			player1.move(10, 0);
			
			
		}


		//player 2 movement muy LENTO

		if(player200==6)

			player2.move(0, -(vel-3));

		else if(player200==-6)

			player2.move(0, vel-3);

		//player 2 movement LENTO

		if(player200==1)

			player2.move(0, -vel);

		else if(player200==-1)

			player2.move(0, vel);

		//player 2 movement MEDIO

		if(player200==2)

			player2.move(0, -(vel+5));

		else if(player200==-2)

			player2.move(0, vel+5);

		//player 2 movement RAPIDO

		if(player200==3)

			player2.move(0, -(vel+10));

		else if(player200==-3)

			player2.move(0, vel+10);

		//player 2 movement muy rapido

		if(player200==4)

			player2.move(0, -(vel+15));

		else if(player200==-4)

			player2.move(0, vel+15);

		//player 2 movement Extreme

		if(player200==5)
			player2.move(0, -(vel+20));

		else if(player200==-5)

			player2.move(0, vel+20);

		
		if(player200==0) 

			player2.move(0,0);

		//MOVIMIENTO GOLPE PLAYER2
		
		if (player2.getPosition().x >= RENDERWIDTH-player2.getSize().x-10){
			  
		if(player200==-22)
			
			player2.move(-10, 0);
			
			
		}

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

		if(ball.getPosition().y <= 0 || ball.getPosition().y >= RENDERHEIGHT - ballDiameter){


			ballVelY *= -1;

			bounce.play();

		}

		//ball and player 1 collision

		if (ball.getPosition().x <= player1.getPosition().x + player1.getSize().x){


			if ((ball.getPosition().y + ballDiameter >= player1.getPosition().y && ball.getPosition().y + ballDiameter <= player1.getPosition().y + player1.getSize().y) || ball.getPosition().y <= player1.getPosition().y + player1.getSize().y && ball.getPosition().y >= player1.getPosition().y){


	if (player1.getPosition().x > 14){

				ballVelX = (ballVelX - 5) * -1;

				ball.setFillColor(sf::Color(255,0,0));

				
			}

	else if (player1.getPosition().x <= 14){

				ballVelX = (ballVelX - 3) * -1;

				ball.setFillColor(sf::Color(0,0,255));

				bounce.play();
}
			

			}

			else{
				ball.setFillColor(sf::Color(255,255,255));

				point.play();

				player2Score += 1;  

				ballX = RENDERWIDTH / 2 - ball.getRadius();

				if (BALLSPEED < 8)

					BALLSPEED += 0.2;

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

		if (ball.getPosition().x + ballDiameter >= player2.getPosition().x){

		
			if ((ball.getPosition().y + ballDiameter >= player2.getPosition().y && ball.getPosition().y + ballDiameter <= player2.getPosition().y + player2.getSize().y) || ball.getPosition().y <= player2.getPosition().y + player2.getSize().y && ball.getPosition().y >= player2.getPosition().y){
			


				if (player2.getPosition().x < (RENDERWIDTH-9-player2.getSize().x)){

					ballVelX = (ballVelX + 5) * -1;

					ball.setFillColor(sf::Color(255,0,0));

					
				
				}

				else if (player2.getPosition().x >= (RENDERWIDTH-9-player2.getSize().x)){

					ballVelX = (ballVelX + 3) * -1;

					ball.setFillColor(sf::Color(0,255,0));

					bounce.play();
			}
		}
				
			
				else{
			
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
		pong.draw(timer1);
		pong.draw(timer2);
		pong.draw(score2);
		pong.draw(player1);
		pong.draw(player2);
		pong.draw(ball);
		pong.draw(ncp);
		pong.display();


		while(player1Score + player2Score == 5){

			if(player1Score > player2Score)
				timer1.setString(convertInt(time1 += 500));
			if(player1Score < player2Score)
				timer2.setString(convertInt(time2 += 500));
		
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

/////////////////////Finish Game/////////////////////////////////


    
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

      cvDestroyAllWindows() ;
      cvReleaseImage(&imgTracking);
      cvReleaseCapture(&capture);     

      return 0;
}

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

void trackObject(IplImage* imgThresh, int player){



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


lastX1 = posX1;
lastY1 = posY1;
lastX2 = posX2;
lastY2 = posY2;
free(moments);

}

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


	//ball properties
	sf::CircleShape ball(7, 25);
	ball.setFillColor(sf::Color(0,255,0));
	
	float ballX = RENDERWIDTH / 4;
 	float ballY = RENDERHEIGHT/2 ;
	
	ball.setPosition(ballX, ballY);

//ball properties
	sf::CircleShape balll(7, 25);
	balll.setFillColor(sf::Color(255,0,0));
	float balllX = RENDERWIDTH*3 / 4;
 	float balllY = RENDERHEIGHT/2;
	balll.setPosition(balllX, balllY);
	
	
	


	while(pong.isOpen())
	{
		sf::Event event;
		while(pong.pollEvent(event))
		{
			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				pong.close();
		}

	

		

		 CvCapture* capture =0;       
      		
		 capture = cvCaptureFromCAM(0);

      
      IplImage* frame=0;

      frame = cvQueryFrame(capture);           
   
            if(!frame) break;

            frame=cvCloneImage(frame); 

            

           cvSmooth(frame, frame, CV_GAUSSIAN,3,3); //smooth the original image using Gaussian kernel

	IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 

            cvCvtColor(frame, imgHSV, CV_BGR2HSV); //Change the color format from BGR to HSV

	

	IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
    
   	 cvInRangeS(imgHSV, cvScalar(0, 134, 107), cvScalar(41, 243, 149), imgThresh); 

	    
	     cvSmooth(imgThresh, imgThresh, CV_GAUSSIAN,3,3); //smooth the binary image using Gaussian kernel
	     
	     trackObject(imgThresh, 1);

	     /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
          	
	IplImage* imgHSV2 = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 
	    
	      cvCvtColor(frame, imgHSV2, CV_BGR2HSV);


	IplImage* imgThresh2 =cvCreateImage(cvGetSize(imgHSV2),IPL_DEPTH_8U, 1);
    
   	 cvInRangeS(imgHSV, cvScalar(24, 0, 224), cvScalar(46, 89, 256), imgThresh);

	

 		cvSmooth(imgThresh2, imgThresh2, CV_GAUSSIAN,3,3); //smooth the binary image using Gaussian kernel

		trackObject(imgThresh2, 2);

           
           //Clean up used images
           cvReleaseImage(&imgHSV);
	   cvReleaseImage(&imgHSV2);
           cvReleaseImage(&imgThresh);
	   cvReleaseImage(&imgThresh2);             
           cvReleaseImage(&frame);   
      	   

      		cvDestroyAllWindows() ;
      
      		cvReleaseCapture(&capture);   
	
		
			
		ball.setPosition((ballX-lastX1)/2, (ballY-lastY1)/2);
		balll.setPosition((balllX-lastX2)/2, (balllY-lastY2)/2);
	
		//render updates
		pong.clear();
		pong.draw(ball);
		pong.draw(balll);
		pong.display();
	}

	return 0;
} 



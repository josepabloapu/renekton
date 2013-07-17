#include <cv.h>
#include <highgui.h>

IplImage* imgTracking;
IplImage* frame;
int lastX1 = -1;
int lastY1 = -1;
int lastX2 = -1;
int lastY2 = -1;
int posX1;int posY1; 
int posX2;int posY2; 


//This function threshold the HSV image and create a binary image
IplImage* GetThresholdedImage(IplImage* imgHSV, int H, int S, int V, int H1, int S1, int V1){  
     
    IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);

    cvInRangeS(imgHSV, cvScalar(H,S,V), cvScalar(H1,S1,V1), imgThresh); 

    return imgThresh;
}



void trackObject(IplImage* imgThresh, int R, int G, int B, int player){



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

switch (player){ 
  
  case 1:

if (posX1 > (frame->width/2) && posY1 > (frame->height/2)){printf("Sector 4, Player %i \n", player);}

if (posX1 > (frame->width/2) && posY1 < (frame->height/2)){printf("Sector 3, Player %i \n", player);}

if (posX1 < (frame->width/2) && posY1 > (frame->height/2)){printf("Sector 2, Player %i \n", player);}

if (posX1 < (frame->width/2) && posY1 < (frame->height/2)){printf("Sector 1, Player %i \n", player);}

cvLine(imgTracking, cvPoint(posX1, posY1), cvPoint(lastX1, lastY1), cvScalar(R,G,B), 4);

break;

  case 2:

if (posX2 > frame->width/2 && posY2 > frame->height/2){printf("Sector 4, Player %i \n", player);}

if (posX2 > frame->width/2 && posY2 < frame->height/2){printf("Sector 3, Player %i \n", player);}

if (posX2 < frame->width/2 && posY2 > frame->height/2){printf("Sector 2, Player %i \n", player);}

if (posX2 < frame->width/2 && posY2 < frame->height/2){printf("Sector 1, Player %i \n", player);}

cvLine(imgTracking, cvPoint(posX2, posY2), cvPoint(lastX2, lastY2), cvScalar(R,G,B), 4);

break;

}

lastX1 = posX1;
lastY1 = posY1;
lastX2 = posX2;
lastY2 = posY2;

}

free(moments);

}


int main(){
  
      CvCapture* capture =0;       
      capture = cvCaptureFromCAM(0);

      if(!capture){
printf("Capture failure\n");
return -1;
      }
      
      frame=0;
      frame = cvQueryFrame(capture);           
      if(!frame) return -1;
  
     //create a blank image and assigned to 'imgTracking' which has the same size of original video
     imgTracking=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U, 3);

     cvZero(imgTracking); //covert the image, 'imgTracking' to black

     cvNamedWindow("Video");     
     cvNamedWindow("Ball");

      //iterate through each frames of the video     
      while(true){

            frame = cvQueryFrame(capture);           
            if(!frame) break;
            frame=cvCloneImage(frame); 
            
           cvSmooth(frame, frame, CV_GAUSSIAN,3,3); //smooth the original image using Gaussian kernel

IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 

            cvCvtColor(frame, imgHSV, CV_BGR2HSV); //Change the color format from BGR to HSV

IplImage* imgThresh = GetThresholdedImage(imgHSV, 0, 143, 149, 6, 245, 256);
	    
	     cvSmooth(imgThresh, imgThresh, CV_GAUSSIAN,3,3); //smooth the binary image using Gaussian kernel
	     
	     trackObject(imgThresh, 255, 0, 0, 1);

	     
          	
IplImage* imgHSV2 = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 
	    
	      cvCvtColor(frame, imgHSV2, CV_BGR2HSV);

IplImage* imgThresh2 = GetThresholdedImage(imgHSV2, 26, 61, 152, 79, 166, 248);

 		cvSmooth(imgThresh2, imgThresh2, CV_GAUSSIAN,3,3); //smooth the binary image using Gaussian kernel

		trackObject(imgThresh2, 0, 255, 0, 2);

            // Add the tracking image and the frame

          cvAdd(frame, imgTracking, frame);

          cvShowImage("Ball", imgThresh); 
	  cvShowImage("Ball2", imgThresh2);            
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
      }

      cvDestroyAllWindows() ;
      cvReleaseImage(&imgTracking);
      cvReleaseCapture(&capture);     

      return 0;
}
#include <cv.h>
#include <highgui.h>

using namespace std;

IplImage* imgTracking;
int lastX1 = -1;
int lastY1 = -1;
bool C1=false; 
bool C_1=false; 
bool C2=false; 
bool C_2=false;
int posX1;int posY1;
int contador=0;
string clave;
//This function threshold the HSV image and create a binary image




int trackObjectCaso(IplImage* imgThresh, int R, int G, int B){

int caso =0;

// Calculate the moments of 'imgThresh'

CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
cvMoments(imgThresh, moments, 1);
double moment10 = cvGetSpatialMoment(moments, 1, 0);
double moment01 = cvGetSpatialMoment(moments, 0, 1);
double area = cvGetCentralMoment(moments, 0, 0);

     // if the area<1000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
if(area>1000){



posX1 = moment10/area;
posY1 = moment01/area; 


if (posY1-lastY1>20 && posY1-lastY1<100 ){caso=1;}
if (posY1-lastY1<-20 && posY1-lastY1>-100 ){caso=-1;}
if (posX1-lastX1<-50 && posX1-lastX1>-100){caso=2;}
if (posX1-lastX1>50 && posX1-lastX1<100){caso=-2;}
cvLine(imgTracking, cvPoint(posX1, posY1), cvPoint(lastX1, lastY1), cvScalar(R,G,B), 4);

}

lastX1 = posX1;
lastY1 = posY1;

free(moments);
return caso;

}


bool clave(){
  
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

     cvNamedWindow("Video");     
     cvNamedWindow("Ball");

      //iterate through each frames of the video     
      while(contador <= 5 ){

            frame = cvQueryFrame(capture);           
            if(!frame) break;
            frame=cvCloneImage(frame); 
            
           cvSmooth(frame, frame, CV_GAUSSIAN,3,3); //smooth the original image using Gaussian kernel

IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 

            cvCvtColor(frame, imgHSV, CV_BGR2HSV); //Change the color format from BGR to HSV

IplImage* imgThresh = GetThresholdedImage(imgHSV, 0, 80, 132, 4, 182, 190);
	    
	     cvSmooth(imgThresh, imgThresh, CV_GAUSSIAN,3,3); //smooth the binary image using Gaussian kernel
	     
	   int caso =  trackObjectCaso(imgThresh, 255, 0, 0);
if(lastX1 =! -1 && lastY1 =! -1){
	   if (caso==1  && C1==false){printf("1"); C1=true;C_1=false;C2=false;C_2=false;contador++;clave+= "1";}
	   if (caso==-1 && C_1==false){printf("2"); C1=false;C_1=true;C2=false;C_2=false;contador++;clave+= "2";}
           if (caso==2  && C2==false){printf("3"); C1=false;C_1=false;C2=true;C_2=false;contador++;clave+= "3";}
	   if (caso==-2 && C_2==false){printf("4"); C1=false;C_1=false;C2=false;C_2=true;contador++;clave+= "4";}
	     }
          	if(contador==5){cout<<"\nLa clave es:"<<clave<<"\n"; clave="";contador=0;}

          cvAdd(frame, imgTracking, frame);

          cvShowImage("Ball", imgThresh); 
	           
          cvShowImage("Video", frame);
           
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

	if (clave == "13241"){

		return true;

		}

      return false;
}

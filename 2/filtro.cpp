#include "opencv/cv.h"

#include "opencv/cxcore.h"

#include "opencv/highgui.h"

#include "math.h"

#include <iostream>

#include <stdio.h>

#include <string.h>



#include <sstream>




using namespace std;

/*

--------------------------------------------*/

int main()

{

	int c = 0;

    CvCapture* capture = cvCaptureFromCAM(1);

		if(!cvQueryFrame(capture)){ cout<<"Video capture failed, please check the camera."<<endl;}else{cout<<"Video camera capture status: OK"<<endl;};

    CvSize sz = cvGetSize(cvQueryFrame( capture));

	IplImage* src = cvCreateImage( sz, 8, 3 );

	IplImage* hsv_image = cvCreateImage( sz, 8, 3);

	IplImage* hsv_mask = cvCreateImage( sz, 8, 1);

	IplImage* edges = cvCreateImage( sz, 8, 1 );

	CvScalar  hsv_min = cvScalar(0, 20, 80, 0); //0, 20, 80, 0

	CvScalar  hsv_max = cvScalar(20,150,200,0); //20,150,200,0

	//////

	while( c != 27)

	{

		src = cvQueryFrame( capture);

			cvNamedWindow( "src",1); cvShowImage( "src", src);

		cvCvtColor(src, hsv_image, CV_BGR2HSV);

			cvNamedWindow( "hsv-img",1); cvShowImage( "hsv-img", hsv_image);

		cvInRangeS (hsv_image, hsv_min, hsv_max, hsv_mask);

			cvNamedWindow( "hsv-msk",1); cvShowImage( "hsv-msk", hsv_mask); hsv_mask->origin = 1; 

	cvThreshold( hsv_mask, edges, 150, 255, CV_THRESH_OTSU);

		CvMemStorage* storage = cvCreateMemStorage();
    		CvSeq* first_contour = NULL;

    int Nc = cvFindContours( edges,storage,&first_contour,sizeof(CvContour),CV_RETR_EXTERNAL);

    // Finding convex Hull
    CvMemStorage* hull_storage = cvCreateMemStorage();
    CvSeq* retHulls = NULL;
   

    for(CvSeq* i = first_contour; i != 0; i = i->h_next){
    // note h_next is next sequence.
    retHulls = cvConvexHull2(first_contour,hull_storage,CV_CLOCKWISE,1);
    
    
    }

    // drawing contours and hull
    IplImage* draw = cvCreateImage(sz, 8, 3 ); // 8,3
   

    for(CvSeq* i = first_contour; i != 0; i = i->h_next){
        //#,#thickness(si es negativo rellena!!),#
        cvDrawContours(draw,retHulls,cvScalar(0,255,0,0),cvScalar(0,255,0,0),3,-1,8);  //retHulls
	cvDrawContours(draw,first_contour,cvScalar(255,255,255,255),cvScalar(255,255,255,255),3,-1,8);
    }
    
   cvNamedWindow( "fnal",1); cvShowImage( "fnal", draw); 
    
    


		c = cvWaitKey( 10);

	}

	//////

	cvReleaseCapture( &capture);

	cvDestroyAllWindows();



}

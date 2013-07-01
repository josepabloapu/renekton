#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;

int main()
{

IplImage* img =  cvLoadImage("./hand.jpg");

//show the original image
cvNamedWindow("Original");
cvShowImage("Original",img);

 //smooth the original image using Gaussian kernel to remove noise
cvSmooth(img, img, CV_GAUSSIAN,3,3);

//converting the original image into grayscale
IplImage* imgGrayScale = cvCreateImage(cvGetSize(img), 8, 1); 
cvCvtColor(img,imgGrayScale,CV_BGR2GRAY);

cvNamedWindow("GrayScale Image");
cvShowImage("GrayScale Image",imgGrayScale);

//thresholding the grayscale image to get better results
cvThreshold(imgGrayScale,imgGrayScale,100,255,CV_THRESH_OTSU);

cvNamedWindow("Thresholded Image");
cvShowImage("Thresholded Image",imgGrayScale);

CvSeq* contour;  //hold the pointer to a contour
CvSeq* result;   //hold sequence of points of a contour
CvMemStorage *storage = cvCreateMemStorage(0); //storage area for all contours

//finding all contours in the image
cvFindContours(imgGrayScale, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));

//iterating through each contour
while(contour)
{
//obtain a sequence of points of the countour, pointed by the variable 'countour'
result = cvApproxPoly(contour, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contour)*0.02, 0);
  


CvPoint *pt[15];

for(int i=0;i<15;i++){
pt[i] = (CvPoint*)cvGetSeqElem(result, i);


}


cvLine(img, *pt[2], *pt[3], cvScalar(0,0,255),4);
cvLine(img, *pt[3], *pt[4], cvScalar(0,0,255),4);
cvLine(img, *pt[4], *pt[5], cvScalar(0,0,255),4);
cvLine(img, *pt[5], *pt[6], cvScalar(0,0,255),4);
cvLine(img, *pt[6], *pt[7], cvScalar(0,0,255),4);
cvLine(img, *pt[7], *pt[8], cvScalar(0,0,255),4);
cvLine(img, *pt[8], *pt[9], cvScalar(0,0,255),4);
cvLine(img, *pt[9], *pt[10], cvScalar(0,0,255),4);
cvLine(img, *pt[10], *pt[11], cvScalar(0,0,255),4);
cvLine(img, *pt[11], *pt[12], cvScalar(0,0,255),4);
cvLine(img, *pt[12], *pt[2], cvScalar(0,0,255),4);
//obtain the next contour*/
contour = contour->h_next;
}

//show the image in which identified shapes are marked   
cvNamedWindow("Tracked");
cvShowImage("Tracked",img);
   
cvWaitKey(0); //wait for a key press

//cleaning up
cvDestroyAllWindows(); 
cvReleaseMemStorage(&storage);
cvReleaseImage(&img);
cvReleaseImage(&imgGrayScale);

return 0;
}

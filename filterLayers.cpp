#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
using namespace std;
using namespace cv;
void on_low_r_thresh_trackbar(int, void *);
void on_high_r_thresh_trackbar(int, void *);
void on_low_g_thresh_trackbar(int, void *);
void on_high_g_thresh_trackbar(int, void *);
void on_low_b_thresh_trackbar(int, void *);
void on_high_b_thresh_trackbar(int, void *);
Mat removeColorRange(Mat, int *, Mat);
Mat filter(Mat, int *);
Mat err(Mat input, int times);
Mat dil(Mat input, int times);
int orangeThreshold[] = {245,255,154,255,15,65};
int low_r=0, low_g=0, low_b=75;
int high_r=32, high_g=43, high_b=139;
int redThreshold[]   = {137,216,0,112,0,50};
int red2Threshold[]   = {30,216,0,40,0,50};
int greenThreshold[]   = {52,106,106,247,55,109};
int outsideThreshold[]   = {0,255,0,255,150,255};
int shadow1Threshold[]   = {65,103,66,96,69,97};
int shadow2Threshold[]   = {105,155,110,156,108,156};


int green[]   = { 36, 100, 105, 188,49, 136};
int red[]   = {109,255,0,71,0,92};
int lightGray[]   = {161,212,160,225,157,216};
int lightGray2[]   = {104,150,105,164,118,163};
int white[]   = {106,185,66,172,27,136};
int black[]   = {1,120,0,78,0,78};


int main(int argc, char *argv[])
{

    if (argc <2){
	printf("Enter input file\n");
        return -1;
    }


 Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

    Mat mask = imread("boardMask.jpg", CV_LOAD_IMAGE_COLOR);   // Read the file
    threshold(mask, mask, 100, 255, THRESH_BINARY);

    mask = removeColorRange(image,green,mask);
    mask = removeColorRange(image,red,mask);
    mask = removeColorRange(image,lightGray,mask);
    mask = removeColorRange(image,lightGray2,mask);
    mask = err(mask,5);
    mask = dil(mask,5);

    Mat removeBoard;
    image.copyTo(removeBoard, mask);

    Mat blackPieces,whitePieces;
    blackPieces = filter(removeBoard,black);

    int x =5;
    blackPieces = dil(blackPieces,x);
    blackPieces = err(blackPieces,x);

    whitePieces = filter(removeBoard,white);
    whitePieces = dil(whitePieces,x);
    whitePieces = err(whitePieces,x);

    Mat pieces;
    removeBoard.copyTo(pieces,whitePieces);
    removeBoard.copyTo(pieces,blackPieces);
    namedWindow("pieces", WINDOW_NORMAL);
    namedWindow("w", WINDOW_NORMAL);
    namedWindow("b", WINDOW_NORMAL);
   imshow("w",whitePieces);
   imshow("b",blackPieces);
   imshow("pieces",pieces);

    Mat white2;
    mask.copyTo(white2);

    Mat frame, frame_threshold;
    namedWindow("Video Capture", WINDOW_NORMAL);
    namedWindow("Object Detection", WINDOW_NORMAL);
    //-- Trackbars to set thresholds for RGB values
    createTrackbar("Low R","Object Detection", &low_r, 255, on_low_r_thresh_trackbar);
    createTrackbar("High R","Object Detection", &high_r, 255, on_high_r_thresh_trackbar);
    createTrackbar("Low G","Object Detection", &low_g, 255, on_low_g_thresh_trackbar);
    createTrackbar("High G","Object Detection", &high_g, 255, on_high_g_thresh_trackbar);
    createTrackbar("Low B","Object Detection", &low_b, 255, on_low_b_thresh_trackbar);
    createTrackbar("High B","Object Detection", &high_b, 255, on_high_b_thresh_trackbar);
    while((char)waitKey(1)!='q'){
	    frame = image;
        if(frame.empty())
            break;
        //-- Detect the object based on RGB Range Values
        inRange(frame,Scalar(low_r,low_g,low_b), Scalar(high_r,high_g,high_b),frame_threshold);

        Mat curMask;
        int threshold[]   = {low_r,high_r,low_g,high_g,low_b,high_b};
        curMask = removeColorRange(image,threshold,mask);
        Mat newImage;
        image.copyTo(newImage,curMask);

        Mat canvas;
        white2.copyTo(canvas);
        newImage.copyTo(canvas,curMask);


        //-- Show the frames
        imshow("Video Capture",frame);
        imshow("Object Detection",canvas);
	usleep(10000);
    }
    return 0;
}

Mat removeColorRange(Mat image, int *t, Mat mask) {
    Mat filteredImage = filter(image, t);
    Mat negative;
    bitwise_not(filteredImage, negative);
    // consider cleaning image
    Mat newMask;
    mask.copyTo(newMask, negative);
    return newMask;
}
Mat filter(Mat input, int *t){
	Mat ret;
    inRange(input,Scalar(t[4],t[2],t[0]), Scalar(t[5],t[3],t[1]),ret);
    threshold(ret, ret, 100, 255, THRESH_BINARY);
	return ret;
}
void on_low_r_thresh_trackbar(int, void *)
{
    low_r = min(high_r-1, low_r);
    setTrackbarPos("Low R","Object Detection", low_r);
}
void on_high_r_thresh_trackbar(int, void *)
{
    high_r = max(high_r, low_r+1);
    setTrackbarPos("High R", "Object Detection", high_r);
}
void on_low_g_thresh_trackbar(int, void *)
{
    low_g = min(high_g-1, low_g);
    setTrackbarPos("Low G","Object Detection", low_g);
}
void on_high_g_thresh_trackbar(int, void *)
{
    high_g = max(high_g, low_g+1);
    setTrackbarPos("High G", "Object Detection", high_g);
}
void on_low_b_thresh_trackbar(int, void *)
{
    low_b= min(high_b-1, low_b);
    setTrackbarPos("Low B","Object Detection", low_b);
}
void on_high_b_thresh_trackbar(int, void *)
{
    high_b = max(high_b, low_b+1);
    setTrackbarPos("High B", "Object Detection", high_b);
}


Mat err(Mat input, int times){
	 Mat err;
  Mat str = getStructuringElement(MORPH_RECT,Size(3,3));
  erode(input,err,str);
	for(int i = 1 ; i < times ; i++){
	  erode(err,err,str);
	}
	return err;
}
Mat dil(Mat input, int times){
	 Mat err;
  Mat str = getStructuringElement(MORPH_RECT,Size(3,3));
  dilate(input,err,str);
	for(int i = 1 ; i < times ; i++){
	  dilate(err,err,str);
	}
	return err;
}

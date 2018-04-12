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

Mat filter(Mat input, int *t);
Mat err(Mat input, int times);
Mat dil(Mat input, int times);
Mat removeColorRange(Mat , int *, Mat );
Mat subtractAnImage(Mat , Mat );
Mat removeTheMostlyRedAndGreenPixels(Mat );
Mat cropAroundCenter(Mat );
int computeValue(Mat );

int main(int argc, char *argv[])
{


    char squares0[4][3] = {"E5"};
    char colors0[2][2] = {"b"};
    char pieces0[6][2] = {"B","K","N","P","Q","R"};
    int numbers0[6] = {1,  1,  1,  1,  1,  1};

    for(int i0 = 0 ; i0 < 1 ; i0 ++) { // squares
    for(int j0 = 0 ; j0 < 1 ; j0++) { // colors
    for(int k0 = 0 ; k0 < 6; k0 ++) { // pieces
    for(int l0 = 1 ; l0 <= numbers0[k0]; l0 ++) { // counts
            char* square0 = squares0[i0];
            char* color0 = colors0[j0];
            char* piece0 = pieces0[k0];

        char input1[50];
        sprintf(input1, "TrainingData/TestData/o_%s_%s_%s_%d.jpg",square0,color0,piece0,l0);
        Mat myImage;
        myImage = imread(input1, CV_LOAD_IMAGE_COLOR);   // Read the file

        char min[50];
        int minDist = -1;


        char squares[4][3] = {"E5"};
        char colors[2][2] = {"b"};
        char pieces[6][2] = {"B","K","N","P","Q","R"};
        int numbers[6] = {5,  3,  4,  3,  2,  4};

        for(int i = 0 ; i < 1 ; i ++) { // squares
        for(int j = 0 ; j < 1 ; j++) { // colors
        for(int k = 0 ; k < 6; k ++) { // pieces
        for(int l = 1 ; l <= numbers[k]; l ++) { // counts
            char* square = squares[i];
            char* color = colors[j];
            char* piece = pieces[k];


            char input[50];
            char output[50];
            sprintf(input, "TrainingData/TrainingPieces/o_%s_%s_%s_%d.jpg",square,color,piece,l);
            sprintf(output, "TrainingData/temp/o_%s_%s_%s_%d.jpg",square,color,piece,l);
        //    printf("%s\n",input);

            Mat image, mask, brd;
            image = imread(input, CV_LOAD_IMAGE_COLOR);   // Read the file

            Mat dif1 = image - myImage;
            Mat dif2 = myImage - image;

            Mat sum = dif1 + dif2;

            imshow(input,sum);

            imwrite(output,sum);
            int dist = computeValue(sum);
//            printf("%s: %d\n", input, dist);

            if(minDist == -1 || dist < minDist) {
                minDist = dist;
                sprintf(min,"%s",input);
            }

        }
        }
        }
        }

        printf("%s\n",input1);
        printf("Min: %s,%d\n\n",min,minDist);


    }
    }
    }
    }

    waitKey(0);
    return 0;
}

int computeValue(Mat image) {
    int count = 0;
    Mat mask;

    cvtColor( image, mask, CV_BGR2GRAY );


    for(int i = 0; i < mask.rows; i++){
        for(int j = 0; j < mask.cols; j++){
        if(mask.at<bool>(i,j)>1){
            count+= mask.at<bool>(i,j);
        }
        }
    }

    return count;
}

Mat cropAroundCenter(Mat image) {
    int x = 0;
    int y = 0;
    int count = 0;
    Mat mask;

    cvtColor( image, mask, CV_BGR2GRAY );


    for(int i = 0; i < mask.rows; i++){
        for(int j = 0; j < mask.cols; j++){
        if(mask.at<bool>(i,j)>1){
            x+=j;
            y+=i;
            count++;
        }
        }
    }

    x = x / count;
    y = y / count;

    printf("%d,%d\n",x,y);

    int width = 200;

    Rect myROI(x - width/2, y - width/2, width, width);

    Mat crop_img;
    crop_img = image(myROI);

    return crop_img;

}

Mat removeTheMostlyRedAndGreenPixels(Mat image) {
    Mat res;
    Mat mask;
    image.copyTo(mask);
    	for(int i = 0; i < mask.cols; i++){
    	    for(int j = 0; j < mask.rows; j++){
    	        Vec3b & color = mask.at<Vec3b>(Point(i,j));
    	        float b = color[0];
    	        float g = color[1];
    	        float r = color[2];
    	        float percentGreen = g / (r + g + b);
    	        if(percentGreen > .4 && g > 20) {
    	            color[0] = 0;
    	            color[1] = 0;
    	            color[2] = 0;
    	        }
    	        float percentRed = r / (r + g + b);
    	        if(percentRed > .7 && r > 30) {
    	            color[0] = 0;
    	            color[1] = 0;
    	            color[2] = 0;
    	        }
//    			printf("\t%d\t%d\t%d\n",image.at<Vec3b>(Point(i,j))[0],image.at<Vec3b>(Point(i,j))[1],image.at<Vec3b>(Point(i,j))[2]);
    	    }
    	}
    cvtColor( mask, mask, CV_BGR2GRAY );
    threshold(mask, mask, 1, 255, THRESH_BINARY);

    namedWindow("a", WINDOW_NORMAL);
    imshow("a",mask);
    mask = dil(mask,1);
    mask = err(mask,1);
    mask = err(mask,2);
    mask = dil(mask,2);
    namedWindow("b", WINDOW_NORMAL);
    imshow("b",mask);

    image.copyTo(res,mask);

    return res;
}

Mat subtractAnImage(Mat image1, Mat image2) {
    Mat res1 = image1-image2;
    Mat res2 = image2-image1;


  /// Separate the image in 3 places ( B, G and R )
  vector<Mat> bgr_planes1;
  split( res1, bgr_planes1 );
  vector<Mat> bgr_planes2;
  split( res2, bgr_planes2 );


    threshold(bgr_planes1[0], bgr_planes1[0], 20, 255, THRESH_BINARY);
    threshold(bgr_planes1[1], bgr_planes1[1], 20, 255, THRESH_BINARY);
    threshold(bgr_planes1[2], bgr_planes1[2], 20, 255, THRESH_BINARY);
    threshold(bgr_planes2[0], bgr_planes2[0], 20, 255, THRESH_BINARY);
    threshold(bgr_planes2[1], bgr_planes2[1], 20, 255, THRESH_BINARY);
    threshold(bgr_planes2[2], bgr_planes2[2], 20, 255, THRESH_BINARY);

    Mat mask1 = bgr_planes1[0] + bgr_planes1[1] + bgr_planes1[2];
    Mat mask2 = bgr_planes2[0] + bgr_planes2[1] + bgr_planes2[2];


    mask1 = err(mask1,4);
    mask1 = dil(mask1,4);
    mask1 = dil(mask1,4);
    mask1 = err(mask1,4);

    mask2 = err(mask2,4);
    mask2 = dil(mask2,4);
    mask2 = dil(mask2,4);
    mask2 = err(mask2,4);

    Mat res = mask1 + mask2;
    res = dil(res,3);
    res = err(res,3);
    return res;
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

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
int orange[] = {170,210,60,100,0,30};
int red[]   = {109,255,0,71,0,92};
int lightGray[]   = {161,212,160,225,157,216};
int lightGray2[]   = {104,150,105,164,118,163};
int white[]   = {106,185,66,172,27,136};
int black[]   = {1,120,0,78,0,78};

int myOrange[] = {164,219,23,143,8,120};

Mat filter(Mat input, int *t);
Mat err(Mat input, int times);
Mat dil(Mat input, int times);
Mat removeColorRange(Mat , int *, Mat );
Mat subtractAnImage(Mat , Mat );
Mat removeTheMostlyRedAndGreenPixels(Mat );
Mat cropAroundCenter(Mat );
Mat isolateLargestConnectedComponent(Mat );
void fillComponent(int , int , int** , Mat , int , int* );
Mat fillHoles(Mat , Mat );
Mat simpleFiltering1(Mat);
void processData();
void processTestData();

int main(int argc, char *argv[])
{
//    processData();
    processTestData();
    waitKey(0);
    return 0;
}

void processTestData() {

char input[50];
char maskName[50];
char output[50];
char rows[8][2] = {"A","B","C","D","E","F","G","H"};
char cols[8][2] = {"1","2","3","4","5","6","7","8"};
for(int n = 16 ; n < 18 ; n++){
    sprintf(input, "testData/t_%d.jpg",n);
    Mat original = imread(input, CV_LOAD_IMAGE_COLOR);   // Read the file
    printf("%s\n",input);
    Mat image = simpleFiltering1(original);


    for(int h = 0 ; h < 8 ; h++) {
    for(int i = 0 ; i < 8 ; i ++) {
        char* row = rows[h];
        char* col = cols[i];

        sprintf(maskName, "data_masks/o_%s%s.jpg",row,col);

        sprintf(output, "testData_o/t_%d_%s%s.jpg",n,row,col);

        Mat mask = imread(maskName, CV_LOAD_IMAGE_COLOR);   // Read the file

        Mat piece;
        image.copyTo(piece,mask);
        piece = isolateLargestConnectedComponent(piece);
        piece = fillHoles(piece, original);


        Mat blackWhite;
        cvtColor( piece, blackWhite, CV_BGR2GRAY );
        threshold(blackWhite, blackWhite, 1, 255, THRESH_BINARY);
        int nonZero = countNonZero(blackWhite);
        if(nonZero>2000){
            printf("%s %d\n",output,nonZero);
            Mat crop_img = cropAroundCenter(piece);
        	imwrite(output,crop_img);
//            imshow(output,crop_img);
        }

//        waitKey(0);
    }
    }
}
}

void processData() {
    char rows[8][2] = {"A","B","C","D","E","F","G","H"};
    char cols[8][2] = {"1","2","3","4","5","6","7","8"};
    char colors[2][2] = {"b","w"};
    char pieces[6][2] = {"B","K","N","P","Q","R"};
    int numbers[6] = {1,  1,  1,  1,  1,  1};

    for(int h = 0 ; h < 4 ; h++) {
    for(int i = 0 ; i < 8 ; i ++) {
    char* row = rows[h];
    char* col = cols[i];

    Mat pieceMask;
    bool first = true;
    for(int j = 0 ; j < 2 ; j++) { // colors
    for(int k = 0 ; k < 6; k ++) { // pieces
    for(int l = 0 ; l < numbers[k]; l ++) { // counts
    char* color = colors[j];
    char* piece = pieces[k];

        char input[50];
        char output[50];
        sprintf(input, "data/%s%s_%s_%s_%d.jpg",row,col,color,piece,l);
        sprintf(output, "data_o/o_%s%s_%s_%s_%d.jpg",row,col,color,piece,l);
        printf("%s\n",input);

        Mat original = imread(input, CV_LOAD_IMAGE_COLOR);   // Read the file
        Mat image = simpleFiltering1(image);
        image = isolateLargestConnectedComponent(image);
        image = fillHoles(image, original);

        Mat singlePieceMask;
        threshold(image, singlePieceMask, 1, 255, THRESH_BINARY);
        if( first ) {
            pieceMask = singlePieceMask;
            first = false;
        } else {
            pieceMask = pieceMask + singlePieceMask;
            threshold(image, singlePieceMask, 1, 255, THRESH_BINARY);
        }

        Mat crop_img = cropAroundCenter(image);
    //	imwrite(output,crop_img);

    }
    }
    }
        char output2[50];
        sprintf(output2, "data_masks/o_%s%s.jpg",row,col);
    //	imwrite(output2,pieceMask);
    }
    }
}

Mat simpleFiltering1(Mat image) {
    Mat mask, brd;
    mask = imread("boardMask2.jpg", CV_LOAD_IMAGE_COLOR);   // Read the file
    threshold(mask, mask, 100, 255, THRESH_BINARY);
    brd = imread("emptyBoard.jpg", CV_LOAD_IMAGE_COLOR);   // Read the file


    Mat resMask = subtractAnImage(image,brd);
    Mat res,res2;
    image.copyTo(res2,resMask);
    res2.copyTo(res,mask);

    Mat filteredImage, negative, res3;

    filteredImage = filter(res,myOrange);
    bitwise_not(filteredImage, negative);
    Mat res4;
    res.copyTo(res4,negative);

    Mat removedColors;

    removedColors = removeTheMostlyRedAndGreenPixels(res4);
    return removedColors;
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

    int width = 180;

    Rect myROI(x - width/2, y - width/2, width, width);

    Mat crop_img;
    crop_img = image(myROI);

    return crop_img;
}

Mat fillHoles(Mat image, Mat original) {
    Mat mask;
    cvtColor( image, mask, CV_BGR2GRAY );
    threshold(mask, mask, 1, 255, THRESH_BINARY);

    for(int i = 0; i < mask.rows; i++){
        int firstWhite = -1;
        int firstBlack = -1;
        int secondWhite = -1;
        for(int j = 0; j < mask.cols; j++){
            int value = mask.at<bool>(i,j);

            if(value==255 && firstWhite == -1){
                firstWhite = j;
            }

            if(firstWhite!=-1 && value==0 && firstBlack == -1) {
                firstBlack = j;
            }

            if(firstWhite!=-1 && firstBlack!=-1 && value==255) {
                secondWhite = j;
                for(int q = firstWhite ; q < secondWhite ; q++) {
                    mask.at<uchar>(i,q) = 255;
                }
                firstWhite = j;
                firstBlack = -1;
                secondWhite = -1;
            }
        }
    }



    for(int i = 0; i < mask.cols; i++){
        int firstWhite = -1;
        int firstBlack = -1;
        int secondWhite = -1;
        for(int j = 0; j < mask.rows; j++){
            int value = mask.at<bool>(j,i);

            if(value==255 && firstWhite == -1){
                firstWhite = j;
            }

            if(firstWhite!=-1 && value==0 && firstBlack == -1) {
                firstBlack = j;
            }

            if(firstWhite!=-1 && firstBlack!=-1 && value==255) {
                secondWhite = j;
                for(int q = firstWhite ; q < secondWhite ; q++) {
                    mask.at<uchar>(q,i) = 255;
                }
                firstWhite = j;
                firstBlack = -1;
                secondWhite = -1;
            }
        }
    }
    threshold(mask, mask, 1, 255, THRESH_BINARY);

    Mat res;
    original.copyTo(res,mask);


    return res;
}

Mat isolateLargestConnectedComponent(Mat image) {
    Mat binary;
    cvtColor( image, binary, CV_BGR2GRAY );
    int cols = binary.cols;
    int rows = binary.rows;
    int** arr = new int*[rows];
    for(int i = 0; i < rows; ++i){
        arr[i] = new int[cols];
    }

    int* components = new int[100];

    for(int i = 0 ; i < 100 ; i ++) {
        components[i] = 0;
    }


    for(int i = 0 ; i < rows ; i ++ ) {
        for(int j = 0 ; j < cols; j ++) {
            arr[i][j] = -1;
        }
    }
    int counter = 0;

    for(int i = 0 ; i < rows ; i ++ ) {
        for(int j = 0 ; j < cols; j ++) {
            if(binary.at<bool>(i,j)>0){
                if(arr[i][j] == -1) {
                    counter++;
                    fillComponent(i,j,arr,binary,counter,components);
                }
            }
        }
    }

    int keep = 0;
    for(int i = 0; i < 100 ; i ++) {
        if(components[i] > components[keep]) {
            keep = i;
        }
    }

    for(int i = 0 ; i < rows ; i ++ ) {
        for(int j = 0 ; j < cols; j ++) {
            if(binary.at<bool>(i,j)>0){
                if(arr[i][j] != keep) {
                    binary.at<bool>(i,j) = 0;
                }
            }
        }
    }


    for(int i = 0; i < rows; ++i){
        free(arr[i]);
    }
    free(arr);
    free(components);

    Mat res;
    image.copyTo(res,binary);
    return res;

}

void fillComponent(int i , int j, int** arr, Mat binary, int counter, int* components) {
    if(i<0 || j < 0 || i >= binary.rows || j >= binary.cols) {
        return;
    }
    if(binary.at<bool>(i,j)>0 && arr[i][j] == -1) {
        arr[i][j] = counter;
        components[counter]++;
        fillComponent(i+1,j,arr,binary,counter,components);
        fillComponent(i-1,j,arr,binary,counter,components);
        fillComponent(i,j+1,arr,binary,counter,components);
        fillComponent(i,j-1,arr,binary,counter,components);
    }
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
    	        if(percentRed > .6 && r > 30) {
    	            color[0] = 0;
    	            color[1] = 0;
    	            color[2] = 0;
    	        }
//    			printf("\t%d\t%d\t%d\n",image.at<Vec3b>(Point(i,j))[0],image.at<Vec3b>(Point(i,j))[1],image.at<Vec3b>(Point(i,j))[2]);
    	    }
    	}
    cvtColor( mask, mask, CV_BGR2GRAY );
    threshold(mask, mask, 1, 255, THRESH_BINARY);

    mask = dil(mask,1);
    mask = err(mask,1);
    mask = err(mask,2);
    mask = dil(mask,2);

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

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <experimental/filesystem>
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

std::vector<std::string> get_filenames( std::experimental::filesystem::path path )
{
    namespace stdfs = std::experimental::filesystem ;

    std::vector<std::string> filenames ;

    // http://en.cppreference.com/w/cpp/experimental/fs/directory_iterator
    const stdfs::directory_iterator end{} ;

    for( stdfs::directory_iterator iter{path} ; iter != end ; ++iter )
    {
        // http://en.cppreference.com/w/cpp/experimental/fs/is_regular_file
        if( stdfs::is_regular_file(*iter) ) // comment out if all names (names of directories tc.) are required
            filenames.push_back( iter->path().string() ) ;
    }

    return filenames ;
}

int main(int argc, char *argv[])
{
    for( const auto& name : get_filenames( "" ) ) std::cout << name << '\n' ;

    if (argc < 2){
	printf("Enter input files\n");
        return -1;
    }

int count = 4;
char *input = "pawn";
char *output = "pawn_o";

for(int p = 1 ; p < count+1; p++) {
    char str[5];
    sprintf(str, "%s%d.jpg", input, p);
    char str2[5];
    sprintf(str2, "%s%d.jpg", output, p);

    Mat image, mask, brd;
    image = imread(str, CV_LOAD_IMAGE_COLOR);   // Read the file
    mask = imread("boardMask2.jpg", CV_LOAD_IMAGE_COLOR);   // Read the file
    threshold(mask, mask, 100, 255, THRESH_BINARY);
    brd = imread("emptyBoard.jpg", CV_LOAD_IMAGE_COLOR);   // Read the file


    Mat resMask = subtractAnImage(image,brd);
    Mat res,res2;
    image.copyTo(res2,resMask);

    res2.copyTo(res,mask);

    namedWindow(str, WINDOW_NORMAL);
    imshow(str,res);


	imwrite(str2,res);

   }
    waitKey(0);
    return 0;
}

Mat subtractAnImage(Mat image1, Mat image2) {
    Mat res1 = image1-image2;
    Mat res2 = image2-image1;
    cvtColor(res1, res1, CV_BGR2GRAY);
    cvtColor(res2, res2, CV_BGR2GRAY);
    Mat mask1,mask2;
    threshold(res1, mask1, 20, 255, THRESH_BINARY);
    mask1 = err(mask1,2);
    mask1 = dil(mask1,2);
    mask1 = dil(mask1,2);
    mask1 = err(mask1,2);
    threshold(res2, mask2, 20, 255, THRESH_BINARY);
    mask2 = err(mask2,2);
    mask2 = dil(mask2,2);
    mask2 = dil(mask2,2);
    mask2 = err(mask2,2);
    Mat res = mask1 + mask2;
    res = dil(res,2);
    res = err(res,2);
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

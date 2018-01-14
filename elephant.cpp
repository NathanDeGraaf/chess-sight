#include <iostream> // for standard I/O
#include <string>   // for strings
#include <stdlib.h>
#include <unistd.h>

#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat)
#include <opencv2/highgui/highgui.hpp>  // Video write
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;


int elephantThreshold[] = {35,47,134,146,230,242};
int orangeThreshold[] = {245,255,154,255,15,65};
int purpleThreshold[] = {178,206,55,105,159,255};

Mat err(Mat input, int times);
Mat dil(Mat input, int times);
Mat clean(Mat input);
Mat filter(Mat input, int *t);
void getCenter(Mat input, int* ret, bool left);
Mat drawSquareOnImage(Mat image, int x, int y, int color);

int main(int argc, char *argv[])
{
    if (argc <2){
	printf("Enter input file\n");
        return -1;
    }


    const string source = argv[1];
    VideoCapture inputVideo(source);              // Open input
    if (!inputVideo.isOpened()){
	printf("failed to open file\n");
        return -1;
    }

    const string output = "O_" + source;
   

//This video read in was taken from OpenCV docs somewhere
 int ex = static_cast<int>(inputVideo.get(CV_CAP_PROP_FOURCC));     // Get Codec Type- Int form

    char EXT[] = {(char)(ex & 0XFF) , (char)((ex & 0XFF00) >> 8),(char)((ex & 0XFF0000) >> 16),(char)((ex & 0XFF000000) >> 24), 0};

    Size S = Size((int) inputVideo.get(CV_CAP_PROP_FRAME_WIDTH),    // Acquire input size
                  (int) inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT));

    VideoWriter outputVideo;  
        outputVideo.open(output, ex, inputVideo.get(CV_CAP_PROP_FPS), S, true);

    if (!outputVideo.isOpened()){
	printf("failed to open output file");
	return -1;
    }

    
	Mat frame;
    namedWindow("Video", WINDOW_NORMAL);


	int elephant[2];
	int orange[2];
	int purple[2];

    while((char)waitKey(1)!='q'){
        inputVideo >> frame; // Get next frame
        if(frame.empty())
		break;

	//Get filtered images of each object's color
	Mat elephantFrame,orangeFrame,purpleFrame;

	elephantFrame = filter(frame,purpleThreshold);
	elephantFrame = clean(elephantFrame);

	
	purpleFrame = filter(frame,elephantThreshold);
	purpleFrame = clean(purpleFrame);

	orangeFrame = filter(frame,orangeThreshold);
	orangeFrame = clean(orangeFrame);

	int coor[2];
	
	// Get object cooredinates
	getCenter(elephantFrame,elephant,false);
	getCenter(purpleFrame,purple,false);
	getCenter(orangeFrame,orange,true);

	
	// Mark objects
	if(elephant[0]>-1)
		frame = drawSquareOnImage(frame,elephant[0],elephant[1],1);
	if(orange[0]>-1)
		frame = drawSquareOnImage(frame,orange[0],orange[1],2);
	if(purple[0]>-1)
		frame = drawSquareOnImage(frame,purple[0],purple[1],0);

        imshow("Video",frame); // Display frame
	outputVideo << frame; // write to output

    }
	waitKey(0);
    return 0;
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

Mat clean(Mat input){
	Mat temp;
	temp = dil(input,3);
	temp = err(temp,3);
	temp = err(temp,3);
	temp = dil(temp,3);
	return temp;
}
void getCenter(Mat input, int *ret,bool left){
// Pixel iteration idea from http://stackoverflow.com/questions/19242662/opencv-find-all-non-zero-coordinates-of-a-binary-mat-image
	double x,y = 0;
	int count = 0;
	Mat binary = input>128;
   assert(binary.cols > 0 && binary.rows > 0 && binary.channels() == 1 && binary.depth() == CV_8U);
    const int M = binary.rows;
    const int N = binary.cols;
    for (int m = 0; m < M; ++m) {
        const char* bin_ptr = binary.ptr<char>(m);
        for (int n = 0; n < N; ++n) {
            if (bin_ptr[n] < 0)
		{
		if(left && n>N/2)
			continue;
		x+=m;
		y+=n;
		count++;
		}
        }
    }
	if(count == 0){
		ret[0]=-1;
		ret[1]=-1;
		return;
	}

	x/=count;
	y/=count;


	ret[0]=(int)x;
	ret[1]=(int)y;
	
	
}
Mat filter(Mat input, int *t){
	Mat ret;
        inRange(input,Scalar(t[4],t[2],t[0]), Scalar(t[5],t[3],t[1]),ret);
	return ret;
}

Mat drawSquareOnImage(Mat image, int x, int y, int color){
// Partial structure taken from: http://docs.opencv.org/2.4/doc/tutorials/core/how_to_scan_images/how_to_scan_images.html#performance-difference
Mat I = image;
    CV_Assert(I.depth() == CV_8U);

    int channels = I.channels();
    int nRows = I.rows;
    int nCols = I.cols * channels;


    if (I.isContinuous())
    {
        nCols *= nRows;
        nRows = 1;
    }

    int i,j;
    uchar* p;
int count = 0;
int target = (x*I.cols+y)*3;

    for( i = 0; i < nRows; ++i)
    {
        p = I.ptr<uchar>(i);
        for ( j = 0; j < nCols; ++j)
        {	

		// My addition
		for(int k = -5 ; k < 6 ; k++)
			if(count-target-3*k*I.cols<15 && count-target-3*k*I.cols>-15){
				if(count%3==color)
					p[j] = 255;
				else
					p[j] = 0;
			}
		count++;
		// My addition end
        }
    }
    return I;
}

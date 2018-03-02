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

int greenThreshold[] = {34,82,94,154,56,110};
int redThreshold[]   = {85,203,0,50,0,60};
int whiteThreshold[] = {93,215,58,191,56,177};
int blackThreshold[] = {5,76,5,56,0,75};
int blueThreshold[] = {2,105,92,190,147,212};


Mat err(Mat input, int times);
Mat dil(Mat input, int times);
Mat clean(Mat input);
Mat filter(Mat input, int *t);
void getCenter(Mat input, int* ret, bool left);
Mat drawSquareOnImage(Mat image, int x, int y, int color);


void processMove(Mat image);
bool getBWPixel(Mat image, int x, int y);
void extractComponent(Mat image, int x, int y);
void stackImages(Mat a, Mat b);
void calculatePercentageFilled(Mat image, Mat board, int x, int y, pair<int,int> &counter);

void getPercent(Mat image, Mat board, double *percent);
void updatePercentages(Mat move, Mat squares);
int getStartPosition();
int getEndPosition(int start);
int getPiece(int move);
void takeMove(int move,int move2);
void addImage(int x, int y, Mat a, Mat b);
vector<pair<int,int> >squareCoordinates(64);

int countBlue(Mat image);

Mat drawBoard();

void waitForBlue(bool up);
void addBWImageToOutput(int x, int y, Mat a, Mat b);

void addImageToOutput(int x, int y, Mat a, Mat b);
Mat whiteFrame,blackFrame,greenFrame,redFrame,boardFrame;

    VideoWriter outputVideo;  
	Mat output(960,1280,CV_8UC3,Scalar(0,0,255));
	int board[8][8]={{-4,-1,0,0,0,0,1,4},
			{-2,-1,0,0,0,0,1,2},
			{-3,-1,0,0,0,0,1,3},
			{-6,-1,0,0,0,0,1,6},
			{-5,-1,0,0,0,0,1,5},
			{-3,-1,0,0,0,0,1,3},
			{-2,-1,0,0,0,0,1,2},
			{-4,-1,0,0,0,0,1,4}};


	double *boardPercent_old = new double[64];
	double *whitePercent_old = new double[64];
	double *blackPercent_old = new double[64];


	double *boardPercent = new double[64];
	double *whitePercent = new double[64];
	double *blackPercent = new double[64];
	Mat pieces[13];
	Mat emptyBoard;
	Mat chessBoard;
	VideoCapture cap;
int main(int argc, char *argv[])
{

    const string outputstr = "output.mp4";
   

//This video read in was taken from OpenCV docs somewhere
 int ex = 828601953;

    char EXT[] = {(char)(ex & 0XFF) , (char)((ex & 0XFF00) >> 8),(char)((ex & 0XFF0000) >> 16),(char)((ex & 0XFF000000) >> 24), 0};

    Size S = Size(1280,960);

        outputVideo.open(outputstr, ex, 30, S, true);

    if (!outputVideo.isOpened()){
	printf("failed to open output file");
	return -1;
    }
  
    namedWindow("Current Board", WINDOW_NORMAL);

	chessBoard = imread("Chess_board.png", CV_LOAD_IMAGE_COLOR);   // Read the file
	pieces[0] = imread("-6.png", CV_LOAD_IMAGE_COLOR);   // Read the file
	pieces[1] = imread("-5.png", CV_LOAD_IMAGE_COLOR);   // Read the file
	pieces[2] = imread("-4.png", CV_LOAD_IMAGE_COLOR);   // Read the file
	pieces[3] = imread("-3.png", CV_LOAD_IMAGE_COLOR);   // Read the file
	pieces[4] = imread("-2.png", CV_LOAD_IMAGE_COLOR);   // Read the file
	pieces[5] = imread("-1.png", CV_LOAD_IMAGE_COLOR);   // Read the file

	pieces[7] = imread("1.png", CV_LOAD_IMAGE_COLOR);   // Read the file
	pieces[8] = imread("2.png", CV_LOAD_IMAGE_COLOR);   // Read the file
	pieces[9] = imread("3.png", CV_LOAD_IMAGE_COLOR);   // Read the file
	pieces[10] = imread("4.png", CV_LOAD_IMAGE_COLOR);   // Read the file
	pieces[11] = imread("5.png", CV_LOAD_IMAGE_COLOR);   // Read the file
	pieces[12] = imread("6.png", CV_LOAD_IMAGE_COLOR);   // Read the file


	Mat currentBoard = drawBoard();
	imshow("Current Board",currentBoard);
	waitKey(1);

	cap.open(0); // open the default camera
   	if(!cap.isOpened())  // check if we succeeded
        	return -1;
	
	printf("Waiting for blue button to initialize image...\n");
	Mat save;
	cap>>save;
	imwrite("newButton.jpg",save);
	waitForBlue(true);

	printf("Initializing empty board...\n");
	//emptyBoard = imread("b.jpg", CV_LOAD_IMAGE_COLOR);   // Read the file
	cap>>emptyBoard;
	
	
	Mat greenInitialFrame,redInitialFrame;
	greenInitialFrame = filter(emptyBoard,greenThreshold);
	redInitialFrame = filter(emptyBoard,redThreshold);

	Mat greenSquares = clean(greenInitialFrame);
	greenSquares = err(greenSquares,3);

	Mat redSquares = clean(redInitialFrame);
	redSquares = err(redSquares,3);

	Mat squares = redSquares;
	stackImages(squares,greenSquares);

	//Initialize coordinates
	Mat brd = squares.clone();
	imshow("base board",squares);
	//imwrite( "err_board2.jpg", squares );
	bool newRow = true;

	int count = 0;
	for(int i = 0; i < brd.rows; i++){
	    for(int j = 0; j < brd.cols; j++){
		if(brd.at<bool>(i,j)>128){
			if(count%8==0 && newRow){
				i+=25; // THIS IS A HACK THAT MAKES ASSUMPTIONS ABOUT IMAGE SIZE AND ORIENTATION. THE BETTER SOLUTION IS TO JUST IMPLEMENT A SMALL SORTING ALGORITHM.
				j=0;
				//printf("__\n");
				newRow=false;
				continue;
			}
	       		//printf("(%d,%d)\n",i,j);
			//printf("\t%d\t%d\t%d\n",move.at<Vec3b>(Point(i,j))[0],move.at<Vec3b>(Point(i,j))[1],move.at<Vec3b>(Point(i,j))[2]);
			extractComponent(brd,i,j);
			squareCoordinates.at(count) = make_pair(i,j);
			count++;
			newRow=true;
		}
	    }
	}





	printf("Flip button when board is set...\n");
	
	waitForBlue(false);

	printf("Reading move...\n");
	Mat move;
	//move = imread("Game1_0.jpg", CV_LOAD_IMAGE_COLOR);   // Read the file
	cap>>move;

	updatePercentages(move,squares);
	
	bool button_up = true;
	for(;;){
		printf("waiting for move...\n");
		waitForBlue(button_up);
		button_up = !button_up;

		Mat new_move;
		cap>>new_move;

		updatePercentages(new_move,squares);
		int move = getStartPosition();
		printf("%d (%d,%d) to ",getPiece(move),move/8,move%8);
		int move2 = getEndPosition(move);
		printf("%d (%d,%d)\n",getPiece(move2),move2/8,move2%8);
		bool endGame = abs(getPiece(move2))== 6;
		takeMove(move,move2);

		Mat currentBoard = drawBoard();

	
	addBWImageToOutput(0,480,output,whiteFrame);
	addBWImageToOutput(640,0,output,boardFrame);
	addImageToOutput(640,480,output,currentBoard);

	outputVideo << output;
	imshow("output",output);

	if(endGame)
		break;
	waitKey(1);

		//waitKey(0);
	}
	/*for(int i = 1 ; i < 28 ; i ++){
		char name[10];
		sprintf(name,"Game1_%d.jpg",i);
		Mat new_move = imread(name, CV_LOAD_IMAGE_COLOR);   // Read the file

		updatePercentages(new_move,squares);
		int move = getStartPosition();
		printf("%d (%d,%d) to ",getPiece(move),move/8,move%8);
		int move2 = getEndPosition(move);
		printf("%d (%d,%d)\n",getPiece(move2),move2/8,move2%8);
		takeMove(move,move2);

		Mat currentBoard = drawBoard();
		imshow("Current Board",currentBoard);
		waitKey(0);
	}*/


	waitKey(0);
	return 0;
}

void waitForBlue(bool up){
	for(;;){
        	Mat frame;
        	cap >> frame; // get a new frame from camera

		addImageToOutput(0,0,output,frame);
		imshow("output",output);
		outputVideo << output;

		int b = countBlue(frame);
		waitKey(30);
		if(up && b>2000 || !up && b<100)
			break;
    	}
}

int countBlue(Mat image){

	Mat blueFrame = filter(image,blueThreshold);
	blueFrame = clean(blueFrame);
//	imshow("blue", blueFrame);
//	imshow("webcam", image);
	int count = 0;
	for(int i = 0; i < image.rows; i++){
	    for(int j = 0; j < image.cols; j++){
		if(blueFrame.at<bool>(i,j)>128){
			count++;
		}
	    }
	}
	return count;
}

Mat drawBoard(){
	Mat currentBoard = chessBoard.clone();
	for(int i = 0 ; i < 8 ; i ++){
		for(int j = 0 ; j < 8 ; j ++){
			if(board[i][j]!=0)
				addImage(j,i,currentBoard,pieces[board[i][j]+6]);
		}
	}
	return currentBoard;
}

void addImage(int x, int y, Mat a, Mat b){
	x*=60;
	y*=60;
	for(int i = 0 ; i < 60 ; i++){
		for(int j = 0 ; j < 60 ; j++){
			if(b.at<Vec3b>(Point(i,j))[2]!=237)
				a.at<Vec3b>(Point(x+i,y+j))  = b.at<Vec3b>(Point(i,j));
		}
	}
}

void addImageToOutput(int x, int y, Mat a, Mat b){
	for(int i = 0 ; i < b.cols ; i++){
		for(int j = 0 ; j < b.rows ; j++){
			a.at<Vec3b>(Point(x+i,y+j))  = b.at<Vec3b>(Point(i,j));
		}
	}
}

void addBWImageToOutput(int x, int y, Mat a, Mat b){
	for(int i = 0 ; i < b.cols ; i++){
		for(int j = 0 ; j < b.rows ; j++){
			if(b.at<bool>(j,i)>128)
				a.at<Vec3b>(Point(x+i,y+j))  = Vec3b(255,255,255);
			else
				a.at<Vec3b>(Point(x+i,y+j))  = Vec3b(0,0,0);
		}
	}
}

int getPiece(int move){
	return board[move/8][move%8];
}

void takeMove(int move,int move2){
	board[move2/8][move2%8] = board[move/8][move%8];
	board[move/8][move%8] = 0;
}
int getStartPosition(){

	int move = -1;
	double maxChange = 0;
	for(int i = 0 ; i < 64 ; i ++){
		double change = boardPercent[i] - boardPercent_old[i];
		if(change > maxChange){
			move = i;
			maxChange = change;
		}
	}
return move;
}

int getEndPosition(int start){
	int move = -1;
	double maxChange = 0;
	for(int i = 0 ; i < 64 ; i ++){
		if(i == start)
			continue;
		double change = abs(whitePercent[i] - whitePercent_old[i]) + abs(blackPercent[i] - blackPercent_old[i]);
		if(change > maxChange){
			move = i;
			maxChange = change;
		}
	}
	return move;
}

void updatePercentages(Mat move, Mat squares){

	delete[] whitePercent_old;
	delete[] blackPercent_old;
	delete[] boardPercent_old;
	whitePercent_old = whitePercent;
	blackPercent_old = blackPercent;
	boardPercent_old = boardPercent;

	whitePercent = new double[64];
	blackPercent = new double[64];
	boardPercent = new double[64];


	
	greenFrame = filter(move,greenThreshold);
	redFrame = filter(move,redThreshold);
	whiteFrame = filter(move,whiteThreshold);
	blackFrame = filter(move,blackThreshold);

	boardFrame = greenFrame;
	stackImages(boardFrame,redFrame);


	getPercent(whiteFrame,squares,whitePercent);
	getPercent(blackFrame,squares,blackPercent);
	getPercent(boardFrame,squares,boardPercent);
}

void getPercent(Mat image, Mat board, double *percent){
	Mat squareClone = board.clone();
	for(int i = 0 ; i < 64 ; i++){
		pair<int,int> coordinate = squareCoordinates.at(i);
		pair<int,int> counter = make_pair(0,0);
		calculatePercentageFilled(image, squareClone, coordinate.first, coordinate.second, counter);
		double f = (double)counter.second / (double) counter.first;
		//printf("(%d,%d): %d\n",i/8,i%8,f>.2);	
		percent[i]=f;
	}

}

void calculatePercentageFilled(Mat image, Mat board, int x, int y, pair<int,int> &counter){
	if(!getBWPixel(board,x,y))
		return;
	board.at<bool>(x,y) = 0;
	counter.first++;
	if(getBWPixel(image,x,y))
		counter.second++;
	if(x>0)
		calculatePercentageFilled(image,board,x-1,y,counter);
	if(x<image.rows-1)
		calculatePercentageFilled(image,board,x+1,y,counter);
	if(y>0)
		calculatePercentageFilled(image,board,x,y-1,counter);
	if(y<image.cols-1)
		calculatePercentageFilled(image,board,x,y+1,counter);
}

void stackImages(Mat a, Mat b){
	for(int i = 0; i < a.rows; i++){
	    for(int j = 0; j < a.cols; j++){
		if(b.at<bool>(i,j)>128){
			a.at<bool>(i,j) = b.at<bool>(i,j);;
		}
	    }
	}
}

void processMove(Mat image){
	return;
}


bool getBWPixel(Mat image, int x, int y){
	return image.at<bool>(x,y)>128;
}

void extractComponent(Mat image, int x, int y){
	if(!getBWPixel(image,x,y))
		return;
	image.at<bool>(x,y) = 0;
	if(x>0)
		extractComponent(image,x-1,y);
	if(x<image.rows-1)
		extractComponent(image,x+1,y);
	if(y>0)
		extractComponent(image,x,y-1);
	if(y<image.cols-1)
		extractComponent(image,x,y+1);
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

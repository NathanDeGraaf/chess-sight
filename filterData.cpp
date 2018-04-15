#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stack>
#include <ctime>

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
bool*** orangePixels;
//int black[]   = {1,120,0,78,0,78};


char input[50];
char output[50];

int myOrange[] = {140,240,10,153,0,130};

char rows[8][2] = {"A","B","C","D","E","F","G","H"};
char cols[8][2] = {"1","2","3","4","5","6","7","8"};
char maskName[50];

Mat chessBoard;
Mat black;
Mat drawBoard();
void addImage(int x, int y, Mat a, Mat b);
int board[8][8]={
                {-4,-1,0,0,0,0,1,4},
                {-2,-1,0,0,0,0,1,2},
                {-3,-1,0,0,0,0,1,3},
                {-6,-1,0,0,0,0,1,6},
                {-5,-1,0,0,0,0,1,5},
                {-3,-1,0,0,0,0,1,3},
                {-2,-1,0,0,0,0,1,2},
                {-4,-1,0,0,0,0,1,4}};

int corners[8][8]={
                {2,2,2,2,1,1,1,1},
                {2,0,0,0,3,3,3,1},
                {2,0,4,4,4,4,3,1},
                {2,0,4,4,4,4,3,1},
                {3,1,4,4,4,4,2,0},
                {3,1,4,4,4,4,2,0},
                {3,1,1,1,2,2,2,0},
                {3,3,3,3,0,0,0,0}};

Mat filter(Mat input, int *t);
Mat err(Mat input, int times);
Mat dil(Mat input, int times);
Mat removeColorRange(Mat , int *, Mat );
Mat subtractAnImage(Mat , Mat, int );
Mat removeTheMostlyRedAndGreenPixels(Mat );
Mat cropAroundCenter(Mat );
Mat isolateLargestConnectedComponent(Mat );
void fillComponent(int , int , int** , Mat , int , int* );
Mat fillHoles(Mat , Mat );
Mat simpleFiltering1(Mat);
void processData();
void processTestData();
int computeValue(Mat);
float compareUsingSubtraction(Mat, Mat);
int isBroken(Mat , Mat , Mat );
void testIsBroken();
void processImage(Mat );
Mat pieces[13];
Mat cropToCorner(Mat, int, int);
void stackImages(Mat a, Mat b);
void readOrange();

int main(int argc, char *argv[])
{
    readOrange();
//    processData();
//    processTestData();

//    testIsBroken();

	black = imread("black.jpg", CV_LOAD_IMAGE_COLOR);   // Read the file
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
    imshow("board",currentBoard);

    sprintf(input, "border.jpg");
    Mat mask = imread(input, CV_LOAD_IMAGE_COLOR);   // Read the file
    cvtColor( mask, mask, CV_BGR2GRAY );
    threshold(mask, mask, 1, 255, THRESH_BINARY);

    VideoCapture cap = VideoCapture("video1.mp4");
//    cap.set(CV_CAP_PROP_FRAME_WIDTH,1920);
//    cap.set(CV_CAP_PROP_FRAME_HEIGHT,1080);
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    Mat edges;

    bool aTurnIsBeingMade = true;

    for(;;) {
        Mat frame;
        cap >> frame; // get a new frame from camera
        imshow("Game", frame);



    Mat filteredImage = filter(frame,myOrange);

    Mat border;
    filteredImage.copyTo(border,mask);

    threshold(border, border, 1, 255, THRESH_BINARY);
    imshow("brder",border);

    int borderNum = countNonZero(border);
//    printf("%d\n",borderNum);

        if(aTurnIsBeingMade && borderNum > 140000) {
            aTurnIsBeingMade = false;
            processImage(frame);

            currentBoard = drawBoard();
            imshow("board",currentBoard);

            waitKey(0);
        }

        if(!aTurnIsBeingMade && borderNum < 134000) {
            aTurnIsBeingMade = true;
        }


        waitKey(10);
        if(waitKey(10) >= 0) break;
    }


    for(int i = 0 ; i < 256 ; i ++) {
        for(int j = 0 ; j < 256; j++) {
            free(orangePixels[i][j]);
        }
        free(orangePixels[i]);
    }
    free(orangePixels);

    return 0;
}

void readOrange() {
    orangePixels = new bool**[256];
    for(int i = 0 ; i < 256 ; i ++) {
        orangePixels[i] = new bool*[256];

        for(int j = 0 ; j < 256; j++) {
            orangePixels[i][j] = new bool[256];
            for(int k = 0 ; k < 256 ; k++) {
                orangePixels[i][j][k] = false;
            }
        }
    }


    Mat myColor;
    myColor = imread("orange.png", CV_LOAD_IMAGE_COLOR);   // Read the file

    for(int i = 0; i < myColor.cols; i++){
        for(int j = 0; j < myColor.rows; j++){
            Vec3b & color = myColor.at<Vec3b>(Point(i,j));
            int b = color[0];
            int g = color[1];
            int r = color[2];
            if(r+g+b == 0)
            {continue;}
            orangePixels[r][g][b] = true;
            for(int a = -2; a <3 ; a++) {
                for(int x = -2; x <3 ; x++) {
                    for(int c = -2; c <3 ; c++) {
                        if(r + a < 0 || g + x < 0 || b + c < 0) {
                            continue;
                        }
                        if(r + a > 255 || g + x > 255 || b + c > 255) {
                            continue;
                        }

                        orangePixels[r+a][g+x][b+c] = true;
                    }
                }
            }
//            printf("%d,%d,%d\n",r,g,b);
//    			printf("\t%d\t%d\t%d\n",image.at<Vec3b>(Point(i,j))[0],image.at<Vec3b>(Point(i,j))[1],image.at<Vec3b>(Point(i,j))[2]);
        }
    }

}

void testIsBroken() {
    sprintf(input, "hand.jpg");
    Mat image = imread(input, CV_LOAD_IMAGE_COLOR);   // Read the file

    sprintf(input, "emptyBoard.jpg");
    Mat original = imread(input, CV_LOAD_IMAGE_COLOR);   // Read the file

    sprintf(input, "boardMask2.jpg");
    Mat mask = imread(input, CV_LOAD_IMAGE_COLOR);   // Read the file

    int a = isBroken(image, original, mask);
    if(a < 770000) {
        printf("correctly identified image as broken\n");
    } else {
        printf("error!\n");
    }
    int b = isBroken(original, original, mask);
    if(b > 770000) {
        printf("correctly identified original as unbroken\n");
    } else {
        printf("error!\n");
    }
    waitKey(0);
}

int isBroken(Mat image, Mat original, Mat mask) {
    clock_t begin;
    clock_t end;
    double elapsed_secs;
        begin = clock();
    Mat resMask = subtractAnImage(image, original, 80);
    Mat res,res2;
    image.copyTo(res2,resMask);
    res2.copyTo(res,mask);
    threshold(res, res, 1, 255, THRESH_BINARY);
    Mat border = mask - res;
    cvtColor( border, border, CV_BGR2GRAY );
    threshold(border, border, 1, 255, THRESH_BINARY);
    int nonZero = countNonZero(border);
    imshow("borer",border);
    printf("non zer: %d\n",nonZero);

    return nonZero;


//    bitwise_not(border, border);
//        end = clock();
//        elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
//        printf("time1: %f\n",elapsed_secs);
//
//        begin = clock();
//
//
//
//
//    int cols = border.cols;
//    int rows = border.rows;
//    int** arr = new int*[rows];
//    for(int i = 0; i < rows; ++i){
//        arr[i] = new int[cols];
//    }
//
//    int* components = new int[1];
//
//    components[0] = 0;
//
//
//    for(int i = 0 ; i < rows ; i ++ ) {
//        for(int j = 0 ; j < cols; j ++) {
//            arr[i][j] = -1;
//        }
//    }
//        end = clock();
//        elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
//        printf("time2: %f\n",elapsed_secs);
//        begin = clock();
//    fillComponent(0,0,arr,border,0,components);
//
//        end = clock();
//        elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
//        printf("time3: %f\n",elapsed_secs);
//        begin = clock();
//    for(int i = 0; i < rows; ++i){
//        free(arr[i]);
//    }
//    int result = components[0];
//    free(arr);
//    free(components);
//        end = clock();
//        elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
//        printf("time4: %f\n",elapsed_secs);
//    return result > 1400000;
}

float compareUsingSubtraction(Mat i1, Mat i2) {
    int* sum1 = new int[3];
    int* sum2 = new int[3];
            sum1[0] = 0;
            sum1[1] = 0;
            sum1[2] = 0;
            sum2[0] = 0;
            sum2[1] = 0;
            sum2[2] = 0;
    int count1 = 0;
    int count2 = 0;

    for(int i = 0; i < i1.cols; i++){
        for(int j = 0; j < i1.rows; j++){
            Vec3b & color = i1.at<Vec3b>(Point(i,j));
            Vec3b & color2 = i2.at<Vec3b>(Point(i,j));

            if(color2[0] + color2[1] + color2[2] < 30) {
                continue;
            }

            int b = color[0];
            int g = color[1];
            int r = color[2];



            sum1[0] += r;
            sum1[1] += g;
            sum1[2] += b;
            if(r+g+b > 0) {
                count1++;
            }
            b = color2[0];
            g = color2[1];
            r = color2[2];
            sum2[0] += r;
            sum2[1] += g;
            sum2[2] += b;
            if(r+g+b > 0) {
                count2++;
            }
        }
    }
    float *average1 = new float[3];
    float *average2 = new float[3];

    average1[0] = sum1[0] / (float) count1;
    average1[1] = sum1[1] / (float) count1;
    average1[2] = sum1[2] / (float) count1;

    average2[0] = sum2[0] / (float) count2;
    average2[1] = sum2[1] / (float) count2;
    average2[2] = sum2[2] / (float) count2;

    float pieceColor1 = (average1[0] + average1[1] + average1[2] ) / (float) 3;
    float pieceColor2 = (average2[0] + average2[1] + average2[2] ) / (float) 3;

//    printf("Piece colors: %f %f\n",pieceColor1, pieceColor2);
    if(abs(pieceColor1 - pieceColor2) > 30) {
        return 1000000000;
    }
    Mat m1 = i1.clone();
    Mat m2 = i2.clone();

    float dist = 0;
    for(int i = 0; i < i1.cols; i++){
        for(int j = 0; j < i1.rows; j++){
            Vec3b & color = i1.at<Vec3b>(Point(i,j));
            Vec3b & color2 = i2.at<Vec3b>(Point(i,j));

            if(color2[0] + color2[1] + color2[2] + color[0]+color[1]+color[2] == 0) {
                continue;
            }

            float b = color[0] - average1[2];
            float g = color[1] - average1[1];
            float r = color[2] - average1[0];
            float b2 = color2[0] - average2[2];
            float g2 = color2[1] - average2[1];
            float r2 = color2[2] - average2[0];


            dist += abs(r-r2);
            dist += abs(g-g2);
            dist += abs(b-b2);

            Vec3b & color3 = m1.at<Vec3b>(Point(i,j));
            Vec3b & color4 = m2.at<Vec3b>(Point(i,j));
            color3[0] = abs((int) color[0] - average1[2]);
            color3[1] = abs((int) color[1] - average1[1]);
            color3[2] = abs((int) color[2] - average1[0]);
            color4[0] = abs((int) color2[0] - average2[2]);
            color4[1] = abs((int) color2[1] - average2[1]);
            color4[2] = abs((int) color2[2] - average2[0]);
        }
    }
    free(sum1);
    free(sum2);
    free(average1);
    free(average2);
//    printf("dist: %f\n", dist);
//    imshow("m1",m1);
//    imshow("m2",m2);
//    waitKey(0);
   return dist;
}

int compareUsingSubtractionOld(Mat i1, Mat i2) {
   Mat dif1 = i1 - i2;
   Mat dif2 = i2 - i1;

   Mat sum = dif1 + dif2;

   int dist = computeValue(sum);
   return dist;
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

void computeClosest(int rowNum, int colNum, Mat image) {
    char colors[2][2] = {"b","w"};
    char pieces[6][2] = {"P","N","B","R","Q","K"};
    int minDist = -1;
    int minDist2 = -1;
    
    char* row = rows[rowNum];
    char* col = cols[colNum];

    int minColor = 0;
    int minPiece = 0;
    int minColor2 = 0;
    int minPiece2 = 0;

    for(int i = 0; i < 2 ;i ++) {
    for(int j = 0; j < 6 ;j ++) {
        char* color = colors[i];
        char* piece = pieces[j];
        sprintf(input, "data_o/o_%s%s_%s_%s_0.png",row,col,color,piece);
        Mat original = imread(input, CV_LOAD_IMAGE_COLOR);   // Read the file
            original = cropToCorner(original,rowNum,colNum);

        float distance = compareUsingSubtraction(image,original);

        if(distance < minDist || minDist == -1){
            minDist = distance;
            minColor = i;
            minPiece = j;
        } else if(distance < minDist2 || minDist2 == -1) {
            minDist2 = distance;
            minColor2 = i;
            minPiece2 = j;
        }
    }
    }

    printf("\n%s%s : %f\n",row,col,minDist2 / (float) minDist);
    printf("1: %s %s : %d\n",colors[minColor],pieces[minPiece],minDist);
    printf("2: %s %s : %d\n",colors[minColor2],pieces[minPiece2],minDist2);
    int piece = minPiece+1;
    int color = minColor*2 - 1;

    board[7-rowNum][7-colNum] = piece*color;

}

void processTestData() {



for(int n = 0 ; n < 18; n++){
    sprintf(input, "testData/t_%d.jpg",n);
    Mat original = imread(input, CV_LOAD_IMAGE_COLOR);   // Read the file
    printf("%s\n",input);
    processImage(original);
}
}

void processImage(Mat original){
    Mat image = simpleFiltering1(original);
    imshow("filteredImage",image);



    for(int h = 0 ; h < 8 ; h++) {
    for(int i = 0 ; i < 8 ; i ++) {
        char* row = rows[h];
        char* col = cols[i];

        sprintf(maskName, "data_masks/o_%s%s.jpg",row,col);
        sprintf(output, "temp/o_%s%s.png",row,col);

//        sprintf(output, "testData_o/t_%d_%s%s.jpg",n,row,col);

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
//            Mat crop_img = cropAroundCenter(piece);
            Mat crop_img = cropToCorner(piece,h,i);
        	imwrite(output,crop_img);
        	computeClosest(h,i,crop_img);
        } else {
            board[7-h][7-i] = 0;
        }

//        waitKey(0);
    }
    }
}

void processData() {
    char rows[8][2] = {"A","B","C","D","E","F","G","H"};
    char cols[8][2] = {"1","2","3","4","5","6","7","8"};
    char colors[2][2] = {"b","w"};
    char pieces[6][2] = {"B","K","N","P","Q","R"};
    int numbers[6] = {1,  1,  1,  1,  1,  1};

    for(int h = 0 ; h < 8; h++) {
    for(int i = 0 ; i < 8; i ++) {
    char* row = rows[h];
    char* col = cols[i];

    Mat pieceMask;
    bool first = true;
    for(int j = 0 ; j < 2 ; j++) { // colors
    for(int k = 0 ; k < 6; k ++) { // pieces
    for(int l = 0 ; l < numbers[k]; l ++) { // counts
    char* color = colors[j];
    char* piece = pieces[k];

        sprintf(input, "data/%s%s_%s_%s_%d.jpg",row,col,color,piece,l);
        sprintf(output, "data_o/o_%s%s_%s_%s_%d.png",row,col,color,piece,l);
        printf("%s\n",input);

        Mat original = imread(input, CV_LOAD_IMAGE_COLOR);   // Read the file
        Mat image = simpleFiltering1(original);
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
    	imwrite(output,crop_img);

    }
    }
    }
        sprintf(output, "data_masks/o_%s%s.png",row,col);
    	imwrite(output,pieceMask);
    }
    }
}

Mat simpleFiltering1(Mat image) {
    Mat mask, brd;
    mask = imread("boardMask2.jpg", CV_LOAD_IMAGE_COLOR);   // Read the file
    threshold(mask, mask, 100, 255, THRESH_BINARY);
    brd = imread("emptyBoard.jpg", CV_LOAD_IMAGE_COLOR);   // Read the file

    Mat resMask = subtractAnImage(image,brd,20);
    Mat res,res2;
    image.copyTo(res2,resMask);
    res2.copyTo(res,mask);

    Mat filteredImage, negative, res3;

//    filteredImage = filter(res,myOrange);
    bitwise_not(res, negative);
    Mat res4;
    res.copyTo(res4,negative);

    Mat removedColors;

    removedColors = removeTheMostlyRedAndGreenPixels(res4);
    return removedColors;
}

Mat cropToCorner(Mat image, int row, int col) {
    int corner = 0;
//    if(row >= 4 && col >= 4) {
//        corner = 2;
//    }
//    if(row < 4 && col >= 4) {
//        corner = 3;
//    }
//    if(row < 4 && col < 4) {
//        corner = 0;
//    }
//    if(row >= 4 && col < 4) {
//        corner = 1;
//    }

    corner = corners[row][col];
    if(corner == 4) {
        return cropAroundCenter(image);
    }

    Mat ret = black.clone();
    Mat mask;

    cvtColor( image, mask, CV_BGR2GRAY );
    threshold(mask,mask,1,255,THRESH_BINARY);

    Mat Points;
    findNonZero(mask,Points);
    Rect minRect = boundingRect(Points);


    Mat crop_img;
    crop_img = image(minRect);

    int x = 0;
    int y = 0;
    switch(corner) {
        case 0:
            break;
        case 1:
            x = 179 - crop_img.rows;
            y = 0;
            break;
        case 2:
            x = 179 - crop_img.rows;
            y = 179 - crop_img.cols;
            break;
        case 3:
            x = 0;
            y = 179 - crop_img.cols;
            break;
    }


    Mat subImg = ret(Range(x,(crop_img.rows)+x), Range(y,(crop_img.cols)+y));
    crop_img.copyTo(subImg);

    return ret;
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

void fillComponent(int i0 , int j0, int** arr, Mat binary, int counter, int* components) {
    stack <int> iStack;
    stack <int> jStack;
    iStack.push(i0);
    jStack.push(j0);
    int rows = binary.rows;
    int cols = binary.cols;

    while(iStack.size() > 0) {
        int i = iStack.top();
        int j = jStack.top();
        iStack.pop();
        jStack.pop();

        if(i<0 || j < 0 || i >= rows || j >= cols) {
            continue;
        }

        if(arr[i][j] == -1) {
            int value = binary.at<bool>(i,j);

            if(value>0 ) {
                arr[i][j] = counter;
                components[counter]++;
                iStack.push(i+1);
                jStack.push(j);

                iStack.push(i-1);
                jStack.push(j);

                iStack.push(i);
                jStack.push(j+1);

                iStack.push(i);
                jStack.push(j-1);
            } else {
                arr[i][j] = -2;
            }
        }
    }
}

void badFillComponent(int i , int j, int** arr, Mat binary, int counter, int* components) {
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

            if(orangePixels[color[2]][color[1]][color[0]]) {
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

Mat subtractAnImage(Mat image1, Mat image2, int minThreshold) {
    Mat res1 = image1-image2;
    Mat res2 = image2-image1;

  /// Separate the image in 3 places ( B, G and R )
  vector<Mat> bgr_planes1;
  split( res1, bgr_planes1 );
  vector<Mat> bgr_planes2;
  split( res2, bgr_planes2 );

    threshold(bgr_planes1[0], bgr_planes1[0], minThreshold, 255, THRESH_BINARY);
    threshold(bgr_planes1[1], bgr_planes1[1], minThreshold, 255, THRESH_BINARY);
    threshold(bgr_planes1[2], bgr_planes1[2], minThreshold, 255, THRESH_BINARY);
    threshold(bgr_planes2[0], bgr_planes2[0], minThreshold, 255, THRESH_BINARY);
    threshold(bgr_planes2[1], bgr_planes2[1], minThreshold, 255, THRESH_BINARY);
    threshold(bgr_planes2[2], bgr_planes2[2], minThreshold, 255, THRESH_BINARY);

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

void stackImages(Mat a, Mat b){
	for(int i = 0; i < a.rows; i++){
	    for(int j = 0; j < a.cols; j++){
		if(b.at<bool>(i,j)>128){
			a.at<bool>(i,j) = b.at<bool>(i,j);;
		}
	    }
	}
}

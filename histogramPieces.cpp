#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <fstream>

using namespace std;
using namespace cv;
void writeToFile(double * ,double * ,double * , int , char *);
double ** readFromFile(char * , int ) ;
/**
 * @function main
 */
int main( int argc, char** argv )
{


char squares[4][3] = {"E4","F6","G2","H8"};
char colors[2][2] = {"b","w"};
char pieces[6][2] = {"B","K","N","P","Q","R"};
int numbers[6] = {1,  1,  4,  1,  1,  1};

for(int i = 0 ; i < 4 ; i ++) { // squares
for(int j = 0 ; j < 2 ; j++) { // colors
for(int k = 0 ; k < 6; k ++) { // pieces
char* square = squares[i];
char* color = colors[j];
char* piece = pieces[k];


      /// Establish the number of bins
      int histSize = 256;
    double* average_r = new double[histSize];
    double* average_g = new double[histSize];
    double* average_b = new double[histSize];


    char input[20];
    char output[20];
    sprintf(output, "PieceHistograms/%s_%s_%s.txt",square,color,piece);
    printf("%s\n",input);

    int count = numbers[k];

      for(int i = 0 ; i < histSize ; i++) {
        average_r[i]=0;
        average_g[i]=0;
        average_b[i]=0;
      }

    for(int p = 0 ; p < count; p++) {
    sprintf(input, "IsolatedPieces/o_%s_%s_%s_%d.jpg",square,color,piece,p);

      Mat src, dst;

      /// Load image
      src = imread( input, 1 );

      if( !src.data )
        { return -1; }

      /// Separate the image in 3 places ( B, G and R )
      vector<Mat> bgr_planes;
      split( src, bgr_planes );


      /// Set the ranges ( for B,G,R) )
      float range[] = { 1, 256 } ;
      const float* histRange = { range };

      bool uniform = true; bool accumulate = false;

      Mat b_hist, g_hist, r_hist;

      /// Compute the histograms:
      calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
      calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
      calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

      // Draw the histograms for B, G and R
      int hist_w = 512; int hist_h = 400;
      int bin_w = cvRound( (double) hist_w/histSize );

      Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

      /// Normalize the result to [ 0, histImage.rows ]
      normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
      normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
      normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );



      for(int i = 0 ; i < histSize ; i++) {
        average_r[i]+=r_hist.at<float>(i);
        average_g[i]+=g_hist.at<float>(i);
        average_b[i]+=b_hist.at<float>(i);
      }


      /// Draw for each channel
      for( int i = 1; i < histSize; i++ )
      {
          line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                           Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                           Scalar( 255, 0, 0), 2, 8, 0  );
          line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                           Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                           Scalar( 0, 255, 0), 2, 8, 0  );
          line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                           Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                           Scalar( 0, 0, 255), 2, 8, 0  );
      }

      /// Display
      namedWindow(input, CV_WINDOW_AUTOSIZE );
      imshow(input, histImage );

       }

      int hist_w = 512; int hist_h = 400;
      int bin_w = cvRound( (double) hist_w/histSize );

      Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

         /// Draw for each channel
         for( int i = 1; i < histSize; i++ )
         {
             line( histImage, Point( bin_w*(i-1), hist_h - cvRound(average_b[i-1]/count) ) ,
                              Point( bin_w*(i), hist_h - cvRound(average_b[i]/count) ),
                              Scalar( 255, 0, 0), 2, 8, 0  );
             line( histImage, Point( bin_w*(i-1), hist_h - cvRound(average_g[i-1]/count) ) ,
                              Point( bin_w*(i), hist_h - cvRound(average_g[i]/count) ),
                              Scalar( 0, 255, 0), 2, 8, 0  );
             line( histImage, Point( bin_w*(i-1), hist_h - cvRound(average_r[i-1]/count) ) ,
                              Point( bin_w*(i), hist_h - cvRound(average_r[i]/count) ),
                              Scalar( 0, 0, 255), 2, 8, 0  );
         }

         /// Display
         namedWindow("average", CV_WINDOW_AUTOSIZE );
         imshow("average", histImage );


      for(int i = 0 ; i < histSize ; i++) {
        average_r[i]=average_r[i]/count;
        average_g[i]=average_g[i]/count;
        average_b[i]=average_b[i]/count;
      }
         writeToFile(average_r,average_g,average_b,histSize,output);

    //     double ** arr = readFromFile("test.txt", histSize);
    //
    //     for( int i = 1; i < histSize; i++ )
    //     {
    //         printf("%f ",arr[1][i]);
    //     }
}
}
}

  waitKey(0);

  return 0;
}

void writeToFile(double * arr1,double * arr2,double * arr3, int size, char * fileName) {
      ofstream myfile(fileName);
      if (myfile.is_open())
      {
        for(int count = 0; count < size; count ++){
            myfile << arr1[count] << " " ;
        }
        for(int count = 0; count < size; count ++){
            myfile << arr2[count] << " " ;
        }
        for(int count = 0; count < size; count ++){
            myfile << arr3[count] << " " ;
        }
        myfile.close();
      }
      else cout << "Unable to open file";
      return;
}

double** readFromFile(char * fileName, int size) {
    double** arr = 0;
    arr = new double*[3];
    for(int i = 0 ; i < 3 ; i++) {
        arr[i] = new double[size];
    }
        ifstream file(fileName);
        if(file.is_open())
        {
            for(int j = 0 ; j < 3 ; j++)
            {
                for(int i = 0; i < size; ++i)
                {
                    file >> arr[j][i];
                }
            }
        }
        return arr;
}

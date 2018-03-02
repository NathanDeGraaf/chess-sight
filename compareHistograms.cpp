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


  /// Establish the number of bins
  int histSize = 256;
double* average_r = new double[histSize];
double* average_g = new double[histSize];
double* average_b = new double[histSize];

int count = 4;


  for(int i = 0 ; i < histSize ; i++) {
    average_r[i]=0;
    average_g[i]=0;
    average_b[i]=0;
  }


  Mat src, dst;

  /// Load image
  src = imread( argv[1], 1 );

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
  namedWindow("image", CV_WINDOW_AUTOSIZE );
  imshow("image", histImage );



     double ** pawn = readFromFile("pawn.txt", histSize);
     double ** bishop = readFromFile("bishop.txt", histSize);
     double ** rook = readFromFile("rook.txt", histSize);
     double ** queen = readFromFile("queen.txt", histSize);
     double ** king = readFromFile("king.txt", histSize);
     double ** knight = readFromFile("knight.txt", histSize);

     double pawnDif = 0;
     double bishDif = 0;
     double rookDif = 0;
     double queenDif = 0;
     double kingDif = 0;
     double knightDif = 0;

     for(int i = 0 ; i < histSize ; i++) {
        pawnDif+=abs(pawn[0][i]-average_r[i]);
        pawnDif+=abs(pawn[1][i]-average_g[i]);
        pawnDif+=abs(pawn[2][i]-average_b[i]);

        bishDif+=abs(bishop[0][i]-average_r[i]);
        bishDif+=abs(bishop[1][i]-average_g[i]);
        bishDif+=abs(bishop[2][i]-average_b[i]);

        rookDif+=abs(rook[0][i]-average_r[i]);
        rookDif+=abs(rook[1][i]-average_g[i]);
        rookDif+=abs(rook[2][i]-average_b[i]);

        queenDif+=abs(queen[0][i]-average_r[i]);
        queenDif+=abs(queen[1][i]-average_g[i]);
        queenDif+=abs(queen[2][i]-average_b[i]);

        kingDif+=abs(king[0][i]-average_r[i]);
        kingDif+=abs(king[1][i]-average_g[i]);
        kingDif+=abs(king[2][i]-average_b[i]);

        knightDif+=abs(knight[0][i]-average_r[i]);
        knightDif+=abs(knight[1][i]-average_g[i]);
        knightDif+=abs(knight[2][i]-average_b[i]);

     }

     printf("pawn dif: %f\n",pawnDif);
     printf("bish dif: %f\n",bishDif);
     printf("king dif: %f\n",kingDif);
     printf("quee dif: %f\n",queenDif);
     printf("knig dif: %f\n",knightDif);
     printf("rook dif: %f\n",rookDif);



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

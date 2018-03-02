#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/**
 * @function main
 */
int main( int argc, char** argv )
{
  /// Establish the number of bins
  int histSize = 256;
double* paverage_r = new double[histSize];
double* paverage_g = new double[histSize];
double* paverage_b = new double[histSize];

double* baverage_r = new double[histSize];
double* baverage_g = new double[histSize];
double* baverage_b = new double[histSize];

int pcount = 4;
int bcount = 6;
  for(int i = 0 ; i < histSize ; i++) {
    paverage_r[i]=0;
    paverage_g[i]=0;
    paverage_b[i]=0;
    baverage_r[i]=0;
    baverage_g[i]=0;
    baverage_b[i]=0;
  }

   printf("nathan1\n");
for(int p = 1 ; p < 1+pcount; p++) {
    char str[5];
    sprintf(str, "po%d.jpg", p);

  Mat src, dst;

  /// Load image
  src = imread( str, 1 );

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
    paverage_r[i]+=r_hist.at<float>(i);
    paverage_g[i]+=g_hist.at<float>(i);
    paverage_b[i]+=b_hist.at<float>(i);
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
  namedWindow(str, CV_WINDOW_AUTOSIZE );
  imshow(str, histImage );
   }
   printf("nathan\n");

for(int p = 1 ; p < 1+bcount; p++) {
    char str[5];
    sprintf(str, "bo%d.jpg", p);

  Mat src, dst;

  /// Load image
  src = imread( str, 1 );

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
    baverage_r[i]+=r_hist.at<float>(i);
    baverage_g[i]+=g_hist.at<float>(i);
    baverage_b[i]+=b_hist.at<float>(i);
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
  namedWindow(str, CV_WINDOW_AUTOSIZE );
  imshow(str, histImage );
   }


  int hist_w = 512; int hist_h = 400;
  int bin_w = cvRound( (double) hist_w/histSize );

  Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

     /// Draw for each channel
     for( int i = 1; i < histSize; i++ )
     {
         line( histImage, Point( bin_w*(i-1), hist_h - cvRound(paverage_b[i-1]/pcount) ) ,
                          Point( bin_w*(i), hist_h - cvRound(paverage_b[i]/pcount) ),
                          Scalar( 255, 0, 0), 2, 8, 0  );
         line( histImage, Point( bin_w*(i-1), hist_h - cvRound(paverage_g[i-1]/pcount) ) ,
                          Point( bin_w*(i), hist_h - cvRound(paverage_g[i]/pcount) ),
                          Scalar( 0, 255, 0), 2, 8, 0  );
         line( histImage, Point( bin_w*(i-1), hist_h - cvRound(paverage_r[i-1]/pcount) ) ,
                          Point( bin_w*(i), hist_h - cvRound(paverage_r[i]/pcount) ),
                          Scalar( 0, 0, 255), 2, 8, 0  );
     }

     /// Display
     namedWindow("paverage", CV_WINDOW_AUTOSIZE );
     imshow("paverage", histImage );

  Mat histImage2( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

     /// Draw for each channel
     for( int i = 1; i < histSize; i++ )
     {
         line( histImage2, Point( bin_w*(i-1), hist_h - cvRound(baverage_b[i-1]/bcount) ) ,
                          Point( bin_w*(i), hist_h - cvRound(baverage_b[i]/bcount) ),
                          Scalar( 255, 0, 0), 2, 8, 0  );
         line( histImage2, Point( bin_w*(i-1), hist_h - cvRound(baverage_g[i-1]/bcount) ) ,
                          Point( bin_w*(i), hist_h - cvRound(baverage_g[i]/bcount) ),
                          Scalar( 0, 255, 0), 2, 8, 0  );
         line( histImage2, Point( bin_w*(i-1), hist_h - cvRound(baverage_r[i-1]/bcount) ) ,
                          Point( bin_w*(i), hist_h - cvRound(baverage_r[i]/bcount) ),
                          Scalar( 0, 0, 255), 2, 8, 0  );
     }

     /// Display
     namedWindow("baverage", CV_WINDOW_AUTOSIZE );
     imshow("baverage", histImage2 );


     printf("going through them all again\n");


  for(int i = 0 ; i < histSize ; i++) {
    paverage_r[i]=paverage_r[i]/pcount;
    paverage_g[i]=paverage_g[i]/pcount;
    paverage_b[i]=paverage_b[i]/pcount;
    baverage_r[i]=baverage_r[i]/bcount;
    baverage_g[i]=baverage_g[i]/bcount;
    baverage_b[i]=baverage_b[i]/bcount;
  }



for(int p = 1 ; p < 1+pcount; p++) {
    char str[5];
    sprintf(str, "po%d.jpg", p);

  Mat src, dst;

  /// Load image
  src = imread( str, 1 );

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


    double pdifference = 0;
    double bdifference = 0;
  for(int i = 0 ; i < histSize ; i++) {
  pdifference+= abs(paverage_r[i]-r_hist.at<float>(i));
  pdifference+= abs(paverage_g[i]-g_hist.at<float>(i));
  pdifference+= abs(paverage_b[i]-b_hist.at<float>(i));
  bdifference+= abs(baverage_r[i]-r_hist.at<float>(i));
  bdifference+= abs(baverage_g[i]-g_hist.at<float>(i));
  bdifference+= abs(baverage_b[i]-b_hist.at<float>(i));
  }
    printf("%s\n", str);
    printf("b dif: %f\n", bdifference);
    printf("p dif: %f\n", pdifference);
   }
   printf("nathan\n");

for(int p = 1 ; p < 1+bcount; p++) {
    char str[5];
    sprintf(str, "bo%d.jpg", p);

  Mat src, dst;

  /// Load image
  src = imread( str, 1 );

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

 double pdifference = 0;
    double bdifference = 0;
  for(int i = 0 ; i < histSize ; i++) {
  pdifference+= abs(paverage_r[i]-r_hist.at<float>(i));
  pdifference+= abs(paverage_g[i]-g_hist.at<float>(i));
  pdifference+= abs(paverage_b[i]-b_hist.at<float>(i));
  bdifference+= abs(baverage_r[i]-r_hist.at<float>(i));
  bdifference+= abs(baverage_g[i]-g_hist.at<float>(i));
  bdifference+= abs(baverage_b[i]-b_hist.at<float>(i));
  }
    printf("%s\n", str);
    printf("b dif: %f\n", bdifference);
    printf("p dif: %f\n", pdifference);

   }



  waitKey(0);

  return 0;
}
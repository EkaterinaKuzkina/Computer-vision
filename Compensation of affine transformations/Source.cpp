#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\imgcodecs.hpp"
#include <iostream>
#include <fstream>
#include <math.h>

using namespace cv;
using namespace std;

void affin(Mat &im, string filename1, string filename2, string filename3)
{
  double x_c = 0, y_c = 0;
  int count = 0;
  unsigned long int bright_sum = 0;
  for (int i = 0; i < im.rows; i++) {
    for (int j = 0; j < im.cols; j++) {
      if (im.at<unsigned char>(i, j) > 0) {
        count++;
      }
      bright_sum += im.at<unsigned char>(i, j);
      y_c += im.at<unsigned char>(i, j) * i;
      x_c += im.at<unsigned char>(i, j) * j;
    }
  }
  // абсцисса и ордината центра тяжести

  x_c /= bright_sum;
  y_c /= bright_sum; //центр 
  int min = 0, min1 = 0;


  Mat res(im.rows, im.cols, im.type());
  for (int i = 0; i < res.rows; i++) {
    for (int j = 0; j < res.cols; j++) {
      res.at<unsigned char>(i, j) = 0;
    }
  }

  for (int i = 0; i < res.rows; i++) {
    for (int j = 0; j < res.cols; j++) {
      if ((i - y_c + 250 >0) && (i - y_c + 250 < im.rows) && (j - x_c + 250 >0) && (j - x_c + 250 < im.cols))
        res.at<unsigned char>(i - y_c + 250, j - x_c + 250) = im.at<unsigned char>(i, j);
    }
  }
  imwrite(filename1, res);


  Mat res2(res.rows, res.cols, res.type());
  for (int i = 0; i < res.rows; i++) {
    for (int j = 0; j < res.cols; j++) {
      res2.at<unsigned char>(i, j) = 0;
    }
  }

  double b = 0, c = 0, d = 0;
  unsigned char bright;
  for (int i = 0; i < res.rows; i++) {
    for (int j = 0; j < res.cols; j++) {
      bright = im.at<unsigned char>(i, j);
      b += bright * (pow(j - x_c, 2) - pow(i - y_c, 2));
      c += bright * 2 * (j - x_c)*(i - y_c);
      d += bright * (pow(j - x_c, 2) + pow(i - y_c, 2));
    }
  }

  // величина сжатия
  double mu = 0;
  double s = sqrt(c*c + b*b);
  mu = sqrt((d + s) / (d - s));

  // направление сжатия
  double tetha = 0.5 * atan2(c, b);

  //коэффициент М равномерного масштабирования 
  double K = 10.0;
  double M = 0;
  double x_plus;
  double y_plus;
  unsigned long int bright_sum2 = 0;
  for (int i = 0; i < res.rows; i++)
    for (int j = 0; j < res.cols; j++) {
      x_plus = (1 / mu) * ((j - x_c)* cos(-tetha) - (i - y_c)* sin(-tetha)) * cos(tetha) - ((j - x_c)* sin(-tetha) + (i - y_c)* cos(-tetha)) * sin(tetha);
      y_plus = (1 / mu) * ((j - x_c)* cos(-tetha) - ((i - y_c)* sin(-tetha))) * sin(tetha) + ((j - x_c)* sin(-tetha) + (i - y_c)* cos(-tetha)) * cos(tetha);
      if (((x_plus + 250 > 0) && (y_plus + 250 > 0)) && ((x_plus + 250 < im.cols) && (y_plus + 250 < im.rows))) {
        res2.at<unsigned char>(y_plus + 250, x_plus + 250) = im.at<unsigned char>(i, j);
        M += im.at<unsigned char>(i, j) * sqrt(x_plus * x_plus + y_plus * y_plus);
        bright_sum2 += im.at<unsigned char>(i, j);
      }
    }
  imwrite(filename2, res2);

  Mat res3(res2.rows, res2.cols, res2.type());
  for (int i = 0; i < res.rows; i++) {
    for (int j = 0; j < res.cols; j++) {
      res3.at<unsigned char>(i, j) = 0;
    }
  }

  M /= (K * bright_sum2);
  for (int i = 0; i < res.rows; i++) {
    for (int j = 0; j < res.cols; j++) {
      if ((i * M + (1 - M)*res2.rows / 2 > 0) && (i * M + (1 - M)*res2.rows / 2 < res2.rows) && (j * M + (1 - M)*res2.cols / 2 > 0) && (j * M + (1 - M)*res2.cols / 2 < res2.cols))
        res3.at<unsigned char>(i, j) = res2.at<unsigned char>(i * M + (1 - M)*res2.rows / 2, j * M + (1 - M)*res2.cols / 2);
    }
  }
  Mat res4(res2.rows / M, res2.cols / M, res2.type());
  resize(res3, res4, Size(res2.rows / M, res2.cols / M), 1 / M, 1 / M, INTER_LINEAR);
  imwrite(filename3, res3);
  cout << " x_center = " << x_c << "\ny_center = " << y_c << "\ntetha = " << tetha * 180 / 3.1415926 << endl << "mu = " << mu << endl << "M = " << M << endl;
}

int main()
{
  Mat im1 = imread("image_1_5.jpg ", IMREAD_GRAYSCALE);
  Mat im2 = imread("image_2_5.png", IMREAD_GRAYSCALE);
  Mat im3 = imread("image_3_5.jpg", IMREAD_GRAYSCALE);

  affin(im1, "1_1.jpg", "1_2.jpg", "1_4.jpg");
  affin(im2, "2_1.jpg", "2_2.jpg", "2_4.jpg");
  affin(im3, "3_1.jpg", "3_2.jpg", "3_4.jpg");

  return 0;
}

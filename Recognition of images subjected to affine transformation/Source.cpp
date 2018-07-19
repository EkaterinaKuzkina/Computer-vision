#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include <iostream>
#include <math.h>


using namespace cv;
using namespace std;

Mat mag_fur(Mat I) {
  Mat padded;                            //расширяем входное изображение до оптимального размера
  int m = getOptimalDFTSize(I.rows);
  int n = getOptimalDFTSize(I.cols); // на границ заполняем нулями
  copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

  Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
  Mat complexI;
  merge(planes, 2, complexI);         // Добавить в расширенную другую плоскость с нулями
  dft(complexI, complexI);            // Сделать дискретное преобразование Фурье
                                      //Преобразование реальных и комплекснх значений в амплитудные
  split(complexI, planes);
  magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
  Mat magI = planes[0];
  return magI;  // возвращаем амплитудную часть пространственного Фурье-спектра
}

Mat get_vector(Mat im) {
  double azimut[256];
  for (int i = 0; i < 256; i++) {
      azimut[i] = 0;
   }
  double fi = 0;
  double sum = 0;
  double rx = im.cols / 2;
  double ry = im.rows / 2;
  for (int i = 0; i < im.rows; i++) {
    for (int j = 0; j < im.cols; j++) {
      if (pow(ry - i, 2) + pow(rx - j, 2) < rx*rx) {
        fi = atan2(j - rx, ry - i) + 3.14;
        for (int n = 0; n < 256; n++) {
          if ((fi >= (2 * 3.14*n / 256)) && (fi < (2 * 3.14*(n + 1) / 256))) {
            azimut[n] += im.at<uchar>(i, j);
          }
        }
      }
      sum += im.at<uchar>(i, j);
    }
  }
  for (int i = 0; i < 256; i++) {
    azimut[i] /= sum;
  }
  Mat I(256, 1, CV_8U, azimut);
  Mat mag_i = mag_fur(I);
  return mag_i;
}

Mat affin(Mat &im, string filename1, string filename2, string filename3)
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
  return res3;
}

double sub(Mat im1, Mat im2) {
  double s = 0;
  for (int i = 0; i < im1.rows; i++) {
    s += pow(im1.at<float>(i, 0) - im2.at<float>(i, 0), 2);
  }
  s = sqrt(s);
  return s;
}

int main()
{
 /* Mat im1 = imread("image_1_5.jpg ", IMREAD_GRAYSCALE);
  Mat im2 = imread("image_2_5.png", IMREAD_GRAYSCALE);
  Mat im3 = imread("image_3_5.png", IMREAD_GRAYSCALE);
  Mat im4 = imread("4.jpg", IMREAD_GRAYSCALE);*/

  /*Mat res1 = affin(im1, "1_1.jpg", "1_2.jpg", "1_4.jpg");
  Mat res2 = affin(im2, "2_1.jpg", "2_2.jpg", "2_4.jpg");
  Mat res3 = affin(im3, "3_1.jpg", "3_2.jpg", "3_4.jpg");
  Mat res4 = affin(im4, "4_1.jpg", "4_2.jpg", "4_4.jpg");*/

  Mat model1 = imread("1_4_big.jpg", IMREAD_GRAYSCALE);
  Mat model2 = imread("2_4_big.jpg", IMREAD_GRAYSCALE);
  Mat mag_1 = get_vector(model1);
  Mat mag_2 = get_vector(model2);
  double s = sub(mag_1, mag_2);
  cout << "s = " << s << endl;
  return 0;
}

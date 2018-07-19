#include <math.h>
#include <stdio.h>
#include <iostream>
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\core\core.hpp"
using namespace cv;
using namespace std;

Mat LogPolarTransf(Mat img) {
  //------------------------Преобразование картинки------------------------------------
  double M1 = (img.cols / log(img.cols / 2));  // вычисляем м для дня исходной картинки                                          
  Mat img_log_polar;
  Point2f center((img.cols - 1) / 2, (img.rows - 1) / 2);  //центр исходной картинки
  logPolar(img, img_log_polar, center, M1, CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS);  // получаем преобразованную в полярно-логарифмическую систему исходную кртинку

                                                                                      //---------------------Ресайз преобразованной картинки---------------
  Mat log_polar_img_res(512, log(img_log_polar.rows) * 50, img.type());
  resize(img_log_polar, log_polar_img_res, log_polar_img_res.size(), 0, 0, CV_INTER_LINEAR); //(src, dst, dst_size, fx, fy, interpolation)

  return log_polar_img_res;
}

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

double find_cor(Mat dis, Mat model) {
  cout << dis.rows << "  " << dis.cols << "  " << model.rows << "  " << model.cols << endl;
  double mean_b = cv::sum(dis)[0];
  double mean_b_m = cv::sum(model)[0];
  mean_b /= mean_b;
  mean_b_m /= mean_b_m;
  double stb = 0;
  double stb_b = 0;
  double stb_m = 0;
  for (int i = 0; i< dis.rows; i++) {
    for (int j = 0; j < dis.cols; j++) {
      stb += (dis.at<float>(i, j) - mean_b)*(model.at<float>(i, j) - mean_b_m);
      stb_b += (dis.at<float>(i, j) - mean_b)*(dis.at<float>(i, j) - mean_b);
      stb_m += (model.at<float>(i, j) - mean_b_m)*(model.at<float>(i, j) - mean_b_m);
    }
  }
  double coef = stb / sqrt(stb_b*stb_m);
  return coef;
}


int main() {
  const char* filename = "512-0.jpg";
  Mat I = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
  Mat magI = mag_fur(I);
  Mat magI_log_polar = LogPolarTransf(magI);
  Mat magI_l_f = mag_fur(magI_log_polar);

  const char* filename2 = "512-60.jpg";
  Mat I_2 = imread(filename2, CV_LOAD_IMAGE_GRAYSCALE);
  Mat magI_2 = mag_fur(I_2);
  Mat magI_log_polar_2 = LogPolarTransf(magI_2);
  Mat magI_l_f_2 = mag_fur(magI_log_polar_2);

  double c = find_cor(magI_l_f, magI_l_f_2);
  cout << "c = " << c << endl;

  return 0;
}
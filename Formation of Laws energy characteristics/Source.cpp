#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\imgcodecs.hpp"
#include <iostream>
#include <fstream>
#include <math.h>

using namespace cv;
using namespace std;

//сегментация изображения
void segment(Mat &img, Mat t1, Mat t2) {
  float a1 = 0;
  float a2 = 0;
  for (int i = 0; i < t1.rows; i++)
    for (int j = 0; j < t1.cols; j++)
      a1 += img.at<float>(i, j);
  for (int i = 0; i < t2.rows; i++)
    for (int j = t1.cols; j < 2 * t2.cols; j++) {
      a2 += img.at<float>(i, j);
    }
  a1 /= t1.rows * t1.cols;
  a2 /= t2.rows * t2.cols;

  for (int i = 0; i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++) {
      float tmp = img.at<float>(i, j);

      float e1 = sqrt((tmp - a1)*(tmp - a1));
      float e2 = sqrt((tmp - a2)*(tmp - a2));

      if (e2 > e1)
        img.at<float>(i, j) = 0;
      else
        img.at<float>(i, j) = 255;
    }
  }
}

//высчитываем энергетическую карту по фильтрованному маской изображению
Mat eCard(Mat card) {
  Mat energCard(card.size(), CV_32FC1, Scalar::all(0));
  float av = 0;
  for (int i = 0; i < card.rows - 15; i++) {
    for (int j = 0; j < card.cols - 15; j++) {
      for (int k = 0; k < 15; k++) {
        for (int y = 0; y < 15; y++) {
          av += abs(card.at<float>(i + k, j + y));
        }
      }
      energCard.at<float>(i + 7, j + 7) = av;
      av = 0;
    }
  }
  return energCard;
}

Mat norm(Mat a) {
  float max = 0;
  for (int i = 0; i < a.rows; i++) {
    for (int j = 0; j < a.cols; j++) {
      if (a.at<float>(i, j) > max) max = a.at<float>(i, j);
    }
  }
  a = (a / max) * 255;
  return a;
}

//фильтрация изображения маской
void filter(Mat img, Mat tex1, Mat tex2, Mat mask, string filename, string F_filename)
{
  Mat res;
  filter2D(img, res, -1, mask);
  imwrite(F_filename, res);
  Mat resCard(res.size(), CV_32F, Scalar::all(0));
  resCard = eCard(res);
  Mat result(resCard.size(), CV_32F, Scalar::all(0));
  result = norm(resCard);
  segment(result, tex1, tex2);
  imwrite(filename, result);
}

//фильтрация изображения симметричными масками
void filter_sim(Mat img, Mat tex1, Mat tex2, Mat mask, Mat mask2, string filename, string F_filename)
{
  Mat res;
  filter2D(img, res, -1, mask);
  Mat res2;
  filter2D(img, res2, -1, mask2);
  Mat r = (res + res2) / 2;
  imwrite(F_filename, r);
  Mat resCard(res.size(), CV_32F, Scalar::all(0));
  resCard = eCard(res);
  Mat resCard2(res2.size(), CV_32F, Scalar::all(0));
  resCard2 = eCard(res2);
  Mat res_del = (resCard + resCard2) / 2;
  Mat result(res_del.size(), CV_32F, Scalar::all(0));
  result = norm(res_del);
  segment(result, tex1, tex2);
  imwrite(filename, result);
}

//устранение влияния интенствности яркоти освещения
Mat delIntens(Mat im) {
  Mat res(im.size(), CV_32FC1, Scalar::all(0));
  float avr = 0;
  for (int i = 0; i < im.rows - 15; i++) {
    for (int j = 0; j < im.cols - 15; j++) {
      for (int k = 0; k < 15; k++) {
        for (int y = 0; y < 15; y++) {
          avr += im.at<float>(i + k, j + y);
        }
      }

      avr /= 15 * 15;

      for (int k = 0; k < 15; k++) {
        for (int y = 0; y < 15; y++) {
          res.at<float>(i + k, j + y) = im.at<float>(i + k, j + y) - avr;
        }
      }
      avr = 0;
    }
  }
  return res;
}

//мера фишера
double fisher(Mat mask1, Mat mask2, string name) {
  Scalar mean1, mean2, stddev1, stddev2;
  meanStdDev(mask1, mean1, stddev1);
  meanStdDev(mask2, mean2, stddev2);
  printf("Fisher mask %s = %f \n", name.c_str(), abs((mean2[0] - mean1[0]) / (stddev1[0] + stddev2[0])));
  return abs((mean2[0] - mean1[0]) / (stddev1[0] + stddev2[0]));
}


int main()
{
  float data[5] = { 1,4,6,4,1 };
  float data1[5] = { -1,-2,0,2,1 };
  float data2[5] = { -1,0,2,0,-1 };
  float data3[5] = { 1,-4,6,-4,1 };

  //-----------Вектора для создания масок-------------
  Mat L5(5, 1, CV_32FC1, &data);
  Mat E5(5, 1, CV_32FC1, &data1);
  Mat S5(5, 1, CV_32FC1, &data2);
  Mat R5(5, 1, CV_32FC1, &data3);

  //----------------Создаем 16 масок---------------------
  Mat L5E5 = L5*E5.t();
  Mat E5L5 = E5*L5.t();
  Mat L5S5 = L5*S5.t();
  Mat S5L5 = S5*L5.t();

  Mat L5R5 = L5*R5.t();
  Mat R5L5 = R5*L5.t();
  Mat E5S5 = E5*S5.t();
  Mat S5E5 = S5*E5.t();

  Mat E5R5 = E5*R5.t();
  Mat R5E5 = R5*E5.t();
  Mat S5R5 = S5*R5.t();
  Mat R5S5 = R5*S5.t();

  Mat E5E5 = E5*E5.t();
  Mat R5R5 = R5*R5.t();
  Mat S5S5 = S5*S5.t();
  Mat L5L5 = L5*L5.t();

  //------------Считываем такстуры------------------------------------
  Mat text1 = imread("mask1.jpg", IMREAD_GRAYSCALE);
  Mat text2 = imread("mask2.jpg", IMREAD_GRAYSCALE);

  //------------Конвертим в float-------------------------------------
  text1.convertTo(text1, CV_32FC1);
  text2.convertTo(text2, CV_32FC1);

  //------------Устраняем влияние интенсивности освещения-------------
  text1 = delIntens(text1);
  text2 = delIntens(text2);

  //------------Получаем свертку 1 текстуры с 16 масками-------------- 
  Mat text1_L5E5, text1_E5L5, text1_L5S5, text1_S5L5;
  Mat text1_L5R5, text1_R5L5, text1_E5S5, text1_S5E5;
  Mat text1_E5R5, text1_R5E5, text1_S5R5, text1_R5S5;
  Mat text1_E5E5, text1_R5R5, text1_S5S5, text1_L5L5;

  filter2D(text1, text1_L5E5, -1, L5E5);
  filter2D(text1, text1_E5L5, -1, E5L5);
  filter2D(text1, text1_L5S5, -1, L5S5);
  filter2D(text1, text1_S5L5, -1, S5L5);

  filter2D(text1, text1_L5R5, -1, L5R5);
  filter2D(text1, text1_R5L5, -1, R5L5);
  filter2D(text1, text1_E5S5, -1, E5S5);
  filter2D(text1, text1_S5E5, -1, S5E5);

  filter2D(text1, text1_E5R5, -1, E5R5);
  filter2D(text1, text1_R5E5, -1, R5E5);
  filter2D(text1, text1_S5R5, -1, S5R5);
  filter2D(text1, text1_R5S5, -1, R5S5);

  filter2D(text1, text1_E5E5, -1, E5E5);
  filter2D(text1, text1_R5R5, -1, R5R5);
  filter2D(text1, text1_S5S5, -1, S5S5);
  filter2D(text1, text1_L5L5, -1, L5L5);

  //--------получаем энергетические текстурные карты для 1 текстуры------------
  Mat e_text1_R5R5 = eCard(text1_R5R5);
  Mat e_text1_S5S5 = eCard(text1_S5S5);
  Mat e_text1_E5E5 = eCard(text1_E5E5);
  Mat e_text1_L5E5 = eCard(text1_L5E5);
  Mat e_text1_E5L5 = eCard(text1_E5L5);
  Mat e_text1_L5S5 = eCard(text1_L5S5);
  Mat e_text1_S5L5 = eCard(text1_S5L5);
  Mat e_text1_L5R5 = eCard(text1_L5R5);
  Mat e_text1_R5L5 = eCard(text1_R5L5);
  Mat e_text1_E5S5 = eCard(text1_E5S5);
  Mat e_text1_S5E5 = eCard(text1_S5E5);
  Mat e_text1_R5E5 = eCard(text1_R5E5);
  Mat e_text1_E5R5 = eCard(text1_E5R5);
  Mat e_text1_S5R5 = eCard(text1_S5R5);
  Mat e_text1_R5S5 = eCard(text1_R5S5);


  //---------усреднили симметричные карты--------------------
  Mat e_text1_L5R5_R5L5 = (e_text1_L5R5 + e_text1_R5L5) / 2;
  Mat e_text1_E5S5_S5E5 = (e_text1_E5S5 + e_text1_S5E5) / 2;
  Mat e_text1_L5E5_E5L5 = (e_text1_L5E5 + e_text1_E5L5) / 2;
  Mat e_text1_L5S5_S5L5 = (e_text1_L5S5 + e_text1_S5L5) / 2;
  Mat e_text1_E5R5_R5E5 = (e_text1_E5R5 + e_text1_R5E5) / 2;
  Mat e_text1_S5R5_R5S5 = (e_text1_S5R5 + e_text1_R5S5) / 2;


  //запись
  Mat text1_L5R5_R5L5 = (text1_L5R5 + text1_R5L5) / 2;
  Mat text1_E5S5_S5E5 = (text1_E5S5 + text1_S5E5) / 2;
  Mat text1_L5E5_E5L5 = (text1_L5E5 + text1_E5L5) / 2;
  Mat text1_L5S5_S5L5 = (text1_L5S5 + text1_S5L5) / 2;
  Mat text1_E5R5_R5E5 = (text1_E5R5 + text1_R5E5) / 2;
  Mat text1_S5R5_R5S5 = (text1_S5R5 + text1_R5S5) / 2;
  imwrite("text1_L5E5_E5L5.jpg", text1_L5E5_E5L5);
  imwrite("text1_L5R5_R5L5.jpg", text1_L5R5_R5L5);
  imwrite("text1_E5S5_S5E5.jpg", text1_E5S5_S5E5);
  imwrite("text1_L5S5_S5L5.jpg", text1_L5S5_S5L5);
  imwrite("text1_E5R5_R5E5.jpg", text1_E5R5_R5E5);
  imwrite("text1_S5R5_R5S5.jpg", text1_S5R5_R5S5);
  imwrite("text1_E5E5.jpg", text1_E5E5);
  imwrite("text1_S5S5.jpg", text1_S5S5);
  imwrite("text1_R5R5.jpg", text1_R5R5);

  //---------------сделали номирование энергетических карт для 1 текстуры-------------
  norm(e_text1_L5E5_E5L5);
  norm(e_text1_L5R5_R5L5);
  norm(e_text1_E5S5_S5E5);
  norm(e_text1_L5S5_S5L5);
  norm(e_text1_E5R5_R5E5);
  norm(e_text1_S5R5_R5S5);
  norm(e_text1_E5E5);
  norm(e_text1_S5S5);
  norm(e_text1_R5R5);


  //-----------------------Получаем свертку 2 текстуры с 16 масками--------------------
  Mat text2_L5E5, text2_E5L5, text2_L5S5, text2_S5L5;
  Mat text2_L5R5, text2_R5L5, text2_E5S5, text2_S5E5;
  Mat text2_E5R5, text2_R5E5, text2_S5R5, text2_R5S5;
  Mat text2_E5E5, text2_R5R5, text2_S5S5, text2_L5L5;

  filter2D(text2, text2_L5E5, -1, L5E5);
  filter2D(text2, text2_E5L5, -1, E5L5);
  filter2D(text2, text2_L5S5, -1, L5S5);
  filter2D(text2, text2_S5L5, -1, S5L5);

  filter2D(text2, text2_L5R5, -1, L5R5);
  filter2D(text2, text2_R5L5, -1, R5L5);
  filter2D(text2, text2_E5S5, -1, E5S5);
  filter2D(text2, text2_S5E5, -1, S5E5);

  filter2D(text2, text2_E5R5, -1, E5R5);
  filter2D(text2, text2_R5E5, -1, R5E5);
  filter2D(text2, text2_S5R5, -1, S5R5);
  filter2D(text2, text2_R5S5, -1, R5S5);

  filter2D(text2, text2_E5E5, -1, E5E5);
  filter2D(text2, text2_R5R5, -1, R5R5);
  filter2D(text2, text2_S5S5, -1, S5S5);
  filter2D(text2, text2_L5L5, -1, L5L5);

  //--------получаем энергетические текстурные карты для 2 текстуры------------
  Mat e_text2_E5E5 = eCard(text2_E5E5);
  Mat e_text2_R5R5 = eCard(text2_R5R5);
  Mat e_text2_S5S5 = eCard(text2_S5S5);
  Mat e_text2_L5E5 = eCard(text2_L5E5);
  Mat e_text2_E5L5 = eCard(text2_E5L5);
  Mat e_text2_L5S5 = eCard(text2_L5S5);
  Mat e_text2_S5L5 = eCard(text2_S5L5);
  Mat e_text2_L5R5 = eCard(text2_L5R5);
  Mat e_text2_R5L5 = eCard(text2_R5L5);
  Mat e_text2_E5S5 = eCard(text2_E5S5);
  Mat e_text2_S5E5 = eCard(text2_S5E5);
  Mat e_text2_E5R5 = eCard(text2_E5R5);
  Mat e_text2_R5E5 = eCard(text2_R5E5);
  Mat e_text2_S5R5 = eCard(text2_S5R5);
  Mat e_text2_R5S5 = eCard(text2_R5S5);

  //---------усреднили симметричные карты-------------------------------------
  Mat e_text2_L5R5_R5L5 = (e_text2_L5R5 + e_text2_R5L5) / 2;
  Mat e_text2_E5S5_S5E5 = (e_text2_E5S5 + e_text2_S5E5) / 2;
  Mat e_text2_L5E5_E5L5 = (e_text2_L5E5 + e_text2_E5L5) / 2;
  Mat e_text2_L5S5_S5L5 = (e_text2_L5S5 + e_text2_S5L5) / 2;
  Mat e_text2_E5R5_R5E5 = (e_text2_E5R5 + e_text2_R5E5) / 2;
  Mat e_text2_S5R5_R5S5 = (e_text2_S5R5 + e_text2_R5S5) / 2;


  //запись
  Mat text2_L5R5_R5L5 = (text2_L5R5 + text2_R5L5) / 2;
  Mat text2_E5S5_S5E5 = (text2_E5S5 + text2_S5E5) / 2;
  Mat text2_L5E5_E5L5 = (text2_L5E5 + text2_E5L5) / 2;
  Mat text2_L5S5_S5L5 = (text2_L5S5 + text2_S5L5) / 2;
  Mat text2_E5R5_R5E5 = (text2_E5R5 + text2_R5E5) / 2;
  Mat text2_S5R5_R5S5 = (text2_S5R5 + text2_R5S5) / 2;

  imwrite("text2_L5E5_E5L5.jpg", text2_L5E5_E5L5);
  imwrite("text2_L5R5_R5L5.jpg", text2_L5R5_R5L5);
  imwrite("text2_E5S5_S5E5.jpg", text2_E5S5_S5E5);
  imwrite("text2_L5S5_S5L5.jpg", text2_L5S5_S5L5);
  imwrite("text2_E5R5_R5E5.jpg", text2_E5R5_R5E5);
  imwrite("text2_S5R5_R5S5.jpg", text2_S5R5_R5S5);
  imwrite("text2_E5E5.jpg", text2_E5E5);
  imwrite("text2_S5S5.jpg", text2_S5S5);
  imwrite("text2_R5R5.jpg", text2_R5R5);

  //---------------сделали номирование энергетических карт для 2 текстуры-------------
  norm(e_text2_L5E5_E5L5);
  norm(e_text2_L5R5_R5L5);
  norm(e_text2_E5S5_S5E5);
  norm(e_text2_L5S5_S5L5);
  norm(e_text2_E5R5_R5E5);
  norm(e_text2_S5R5_R5S5);
  norm(e_text2_E5E5);
  norm(e_text2_S5S5);
  norm(e_text2_R5R5);

  //-------------Fisher-----------------находим наиболее подходящие маски-------------
  double f_L5E5_E5L5 = fisher(e_text1_L5E5_E5L5, e_text2_L5E5_E5L5, "e_L5E5_E5L5");
  double f_L5R5_R5L5 = fisher(e_text1_L5R5_R5L5, e_text2_L5R5_R5L5, "e_L5R5_R5L5");
  double f_E5S5_S5E5 = fisher(e_text1_E5S5_S5E5, e_text2_E5S5_S5E5, "e_E5S5_S5E5");
  double f_L5S5_S5L5 = fisher(e_text1_L5S5_S5L5, e_text2_L5S5_S5L5, "e_L5S5_S5L5");
  double f_E5R5_R5E5 = fisher(e_text1_E5R5_R5E5, e_text2_E5R5_R5E5, "e_E5R5_R5E5");
  double f_S5R5_R5S5 = fisher(e_text1_S5R5_R5S5, e_text2_S5R5_R5S5, "e_S5R5_R5S5");
  double f_E5E5 = fisher(e_text1_E5E5, e_text2_E5E5, "e_E5E5");
  double f_S5S5 = fisher(e_text1_S5S5, e_text2_S5S5, "e_S5S5");
  double f_R5R5 = fisher(e_text1_R5R5, e_text2_R5R5, "e_R5R5");

  //------------------Считали мозаику--------------------------------------------------
  Mat src = imread("test_test.jpg", IMREAD_GRAYSCALE);
  src.convertTo(src, CV_32FC1);

  //----------------Сделали ее фильтрацию->сегментацию с масками------------------------
  filter_sim(src, text2, text2, L5R5, R5L5, "res_L5R5.jpg", "f_L5R5.jpg");
  filter_sim(src, text2, text2, L5E5, E5L5, "res_L5E5.jpg", "f_L5E5.jpg");
  filter_sim(src, text2, text2, E5S5, S5E5, "res_E5S5.jpg", "f_E5S5.jpg");
  filter_sim(src, text2, text2, L5S5, S5L5, "res_L5S5.jpg", "f_L5S5.jpg");
  filter_sim(src, text2, text2, E5R5, R5E5, "res_E5R5.jpg", "f_E5R5.jpg");
  filter_sim(src, text2, text2, S5R5, R5S5, "res_S5R5.jpg", "f_S5R5.jpg");

  filter(src, text2, text2, E5E5, "res_E5E5.jpg", "f_E5E5.jpg");
  filter(src, text2, text2, S5S5, "res_S5S5.jpg", "f_S5S5.jpg");
  filter(src, text2, text2, R5R5, "res_R5R5.jpg", "f_R5R5.jpg");



  return 0;
}
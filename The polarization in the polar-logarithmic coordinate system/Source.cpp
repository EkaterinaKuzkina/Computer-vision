#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\imgcodecs.hpp"
#include <iostream>
#include <fstream>
#include <math.h>

using namespace cv;
using namespace std;

int match_method = 3;  //CV_TM_CCORR_NORMED

void MatchingMethod(Mat dst, Mat dst2)
{
  Mat img_display;
  Mat result;
  vconcat(dst, dst, img_display);                      // соедин€ем 2 картинки в одну vconcat (src, src, dst) исследуемое изображение
  imwrite("main_result_23__lab2.jpg", img_display);    // сохран€ем смержиную картинку
  int result_cols = img_display.cols - dst2.cols + 1;  // высчитываем кол-во столбцов в коррелл€ционной матрице
  int result_rows = img_display.rows - dst2.rows + 1;  // кол-во в ней столбцов 
  //printf("%i %i \n", result_cols, result_rows); 
  result.create(result_rows, result_cols, CV_8U);
  matchTemplate(img_display, dst2, result, match_method);  // запускаем метод поиска объекта по шаблону (src, template, result, method)
  normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat()); // нормализууем результат (src, dst, от скольки, до скольки, normalization type (Min = от скольки, max = до скольки), имеет такой же тип как src, маска)
  double minVal; double maxVal; // значени€
  Point minLoc; Point maxLoc;   // координаты
  Point matchLoc;
  minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
  matchLoc = maxLoc;
  printf("%5.3f \n", maxVal);
  printf("abscissa = %d ordina = %d \n", maxLoc.x, maxLoc.y);
  printf("scale = %5.2f \n", exp((float)maxLoc.x / 50));
  printf("angle = %d \n", maxLoc.y);
  result.convertTo(result, CV_8UC3, 255.0);
  imwrite("result_23__lab2.jpg", result);
  waitKey(0);
  return;
}

//----------------функци€ дл€ уничтожени€ перефирийной области-----------------
Mat unbright(Mat &im)
{
  int rows = im.rows, cols = im.cols;
  int r = rows / 2;  // узнали радиус
  unsigned long long bright_sum = 0;
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
    {
      bright_sum += im.at<unsigned char>(i, j);  // посчитали сумарную €ркость всей картинки
    }
  bright_sum /= (rows * cols);  // нашли среднюю €ркость всейй картинки
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++)
    {
      if (((i - r)*(i - r) + (j - r)*(j - r)) >= r*r) {  
        im.at<uchar>(Point(i, j)) = bright_sum;  // всем пиксел€м за радиусом присвоили это значение
      }
    }
  }
  return im;
}

Mat LogPolarTransf(Mat img) {
  //------------------------ѕреобразование картинки------------------------------------
  double M1 = (img.cols / log(img.cols / 2));  // вычисл€ем м дл€ дн€ исходной картинки                                          
  Mat img_log_polar;
  Point2f center((img.cols - 1) / 2, (img.rows - 1) / 2);  //центр исходной картинки
  logPolar(img, img_log_polar, center, M1, CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS);  // получаем преобразованную в пол€рно-логарифмическую систему исходную кртинку
 
  //---------------------–есайз преобразованной картинки---------------
  Mat log_polar_img_res(360, log(img_log_polar.rows) * 50, img.type());
  resize(img_log_polar, log_polar_img_res, log_polar_img_res.size(), 0, 0, CV_INTER_LINEAR); //(src, dst, dst_size, fx, fy, interpolation)

  return log_polar_img_res;
}

int main()
{
  Mat img = imread("main_s.jpg", IMREAD_GRAYSCALE);
  Mat img_ch = imread("main_s_rot_sc.jpg", IMREAD_GRAYSCALE);
  img = unbright(img);
  img_ch = unbright(img_ch);
  img = LogPolarTransf(img);
  img_ch = LogPolarTransf(img_ch);
  imwrite("1.jpg", img);
  imwrite("2.jpg", img_ch);
  MatchingMethod(img_ch, img);
  return 0;
}

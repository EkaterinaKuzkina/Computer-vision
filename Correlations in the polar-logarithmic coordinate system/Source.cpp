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
  vconcat(dst, dst, img_display);                      // ñîåäèíÿåì 2 êàðòèíêè â îäíó vconcat (src, src, dst) èññëåäóåìîå èçîáðàæåíèå
  imwrite("main_result_23__lab2.jpg", img_display);    // ñîõðàíÿåì ñìåðæèíóþ êàðòèíêó
  int result_cols = img_display.cols - dst2.cols + 1;  // âûñ÷èòûâàåì êîë-âî ñòîëáöîâ â êîððåëëÿöèîííîé ìàòðèöå
  int result_rows = img_display.rows - dst2.rows + 1;  // êîë-âî â íåé ñòîëáöîâ 
  //printf("%i %i \n", result_cols, result_rows); 
  result.create(result_rows, result_cols, CV_8U);
  matchTemplate(img_display, dst2, result, match_method);  // çàïóñêàåì ìåòîä ïîèñêà îáúåêòà ïî øàáëîíó (src, template, result, method)
  normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat()); // íîðìàëèçóóåì ðåçóëüòàò (src, dst, îò ñêîëüêè, äî ñêîëüêè, normalization type (Min = îò ñêîëüêè, max = äî ñêîëüêè), èìååò òàêîé æå òèï êàê src, ìàñêà)
  double minVal; double maxVal; // çíà÷åíèÿ
  Point minLoc; Point maxLoc;   // êîîðäèíàòû
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

//----------------ôóíêöèÿ äëÿ óíè÷òîæåíèÿ ïåðåôèðèéíîé îáëàñòè-----------------
Mat unbright(Mat &im)
{
  int rows = im.rows, cols = im.cols;
  int r = rows / 2;  // óçíàëè ðàäèóñ
  unsigned long long bright_sum = 0;
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
    {
      bright_sum += im.at<unsigned char>(i, j);  // ïîñ÷èòàëè ñóìàðíóþ ÿðêîñòü âñåé êàðòèíêè
    }
  bright_sum /= (rows * cols);  // íàøëè ñðåäíþþ ÿðêîñòü âñåéé êàðòèíêè
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++)
    {
      if (((i - r)*(i - r) + (j - r)*(j - r)) >= r*r) {  
        im.at<uchar>(Point(i, j)) = bright_sum;  // âñåì ïèêñåëÿì çà ðàäèóñîì ïðèñâîèëè ýòî çíà÷åíèå
      }
    }
  }
  return im;
}

Mat LogPolarTransf(Mat img) {
  //------------------------Ïðåîáðàçîâàíèå êàðòèíêè------------------------------------
  double M1 = (img.cols / log(img.cols / 2));  // âû÷èñëÿåì ì äëÿ äíÿ èñõîäíîé êàðòèíêè                                          
  Mat img_log_polar;
  Point2f center((img.cols - 1) / 2, (img.rows - 1) / 2);  //öåíòð èñõîäíîé êàðòèíêè
  logPolar(img, img_log_polar, center, M1, CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS);  // ïîëó÷àåì ïðåîáðàçîâàííóþ â ïîëÿðíî-ëîãàðèôìè÷åñêóþ ñèñòåìó èñõîäíóþ êðòèíêó
 
  //---------------------Ðåñàéç ïðåîáðàçîâàííîé êàðòèíêè---------------
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

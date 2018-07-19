#define _CRT_SECURE_NO_WARNINGS
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <fstream>
#define _USE_MATH_DEFINES
#include <list>
#include <vector>
#include <math.h>

using namespace std;
using namespace cv;

int main()
{
IplImage *image = NULL, *gauss_d1 = NULL,
*gauss_d2 = NULL, *ganglionic = NULL, *res = NULL, *res_c = NULL;
image = cvCreateImage(cvSize(256, 256), IPL_DEPTH_32F, 1);
gauss_d1 = cvCreateImage(cvSize(256, 256), IPL_DEPTH_32F, 1);
gauss_d2 = cvCreateImage(cvSize(256, 256), IPL_DEPTH_32F, 1);
ganglionic = cvCreateImage(cvSize(256, 256), IPL_DEPTH_32F, 1);
res = cvCreateImage(cvSize(256, 256), IPL_DEPTH_8U, 1);
res_c = cvCreateImage(cvSize(256, 256), IPL_DEPTH_8U, 1);

//point
//cvZero(res);
//for (int i = 0; i < 13; i++) {
//  for (int j = 0; j < 13; j++) {
//    cvZero(image);
//    cvCircle(image, cvPoint(j + 128 - 6, i + 128 - 6), 3,
//      CV_RGB(127, 127, 127), -1, 8, 0);
//    cvSmooth(image, gauss_d1, CV_GAUSSIAN, 5, 0, 0, 0);
//    cvSmooth(image, gauss_d2, CV_GAUSSIAN, 11, 0, 0, 0);
//    cvSub(gauss_d1, gauss_d2, ganglionic, NULL);
//    float v = (((short *)
//      (ganglionic->imageData + (128)*ganglionic->widthStep))[128]) *3 + 128;
//  
//    cvCircle(res, cvPoint(j * 16 + 32, i * 16 + 32), 2, CV_RGB(v, v, v), 3, 8, 0);
//  }
//}



  //cvZero(res);
  //int v_prev = 0;
  //for (int i = 0; i < 30; i++) {
  //  cvZero(image);
  //circle
  //  cvCircle(image, cvPoint(128, 128), i, CV_RGB(1000, 1000, 1000), -1, 8, 0);
  //ring
  //  cvCircle(image, cvPoint(128, 128), i, CV_RGB(1000, 1000, 1000), 3, 8, 0);
  //line
 /* cvLine(image, cvPoint(128 + 100 * cos(i / 57.2), 128 + 100 * sin(i / 57.2)),
    cvPoint(128 - 100 * cos(i / 57.2), 128 - 100 * sin(i / 57.2)),
    CV_RGB(1000, 1000, 1000), 1, 8, 0);*/
  //  cvSmooth(image, gauss_d1, CV_GAUSSIAN, 15, 0, 0, 0);
  //  cvSmooth(image, gauss_d2, CV_GAUSSIAN, 31, 0, 0, 0);
  //  cvSub(gauss_d1, gauss_d2, ganglionic, NULL);
  //  int v = (int)(((short *)(ganglionic->imageData +
  //    (128)*ganglionic->widthStep))[128]);
  //  printf("%d\n", v);
  //  cvLine(res, cvPoint(i * 8, 128 - v_prev / 5),
  //    cvPoint((i + 1) * 8, 128 - v / 5), CV_RGB(255, 255, 255), 1, 8, 0);
  //  v_prev = v;
  //}

 

//++++++++++++++++++++Простая клетка++++++++++++++++++++++
//cv::Mat m = cvarrToMat(image);
//
//int pre = res->height / 2;
//cvZero(res);
//cvZero(res_c);
//int num = 0;
//int v_prev = 0;
//int y = 0;
//for (int i = 0; i < 30; i++) {
//    cvZero(image);
//    //линия поворачиваемая
//   /* cvLine(image, cvPoint(128 + 100 * cos(i / 57.2), 128 + 100 * sin(i / 57.2)),
//      cvPoint(128 - 100 * cos(i / 57.2), 128 - 100 * sin(i / 57.2)),
//      CV_RGB(1000, 1000, 1000), 1, 8, 0); */
//    //линия: перемещаемая горизонтально
//   //  cvLine(image, cvPoint(40, y+8*i), cvPoint(216, y + 8 * i), CV_RGB(1000, 1000, 1000), 1, 8, 0);
//    //cvCircle(image, cvPoint(j + 128 - 6, i + 128 - 6), 3, CV_RGB(127, 127, 127), -1, 8, 0);
//    //круг
//   /* cvCircle(image, cvPoint(128, 128), i, CV_RGB(1000, 1000, 1000), -1, 8, 0);*/
//   //кольцо
//    cvCircle(image, cvPoint(128, 128), i, CV_RGB(1000, 1000, 1000), 3, 8, 0);
//    Mat g_1(256, 256, CV_32F, Scalar::all(0));
//    Mat g_2(256, 256, CV_32F, Scalar::all(0));
//    Mat gang_1(256, 256, CV_32F, Scalar::all(0));
//    Rect region_1(70, image->height / 2 , 20, 20);
//
//    Rect region_2(95, image->height / 2 ,20, 20);
//    Mat g_2_1(256, 256, CV_32F, Scalar::all(0));
//    Mat g_2_2(256, 256, CV_32F, Scalar::all(0));
//    Mat gang_2(256, 256, CV_32F, Scalar::all(0));
//
//    Rect region_3(125, image->height / 2 , 20, 20);
//    Mat g_3_1(256, 256, CV_32F, Scalar::all(0));
//    Mat g_3_2(256, 256, CV_32F, Scalar::all(0));
//    Mat gang_3(256, 256, CV_32F, Scalar::all(0));
//
//    Rect region_4(155, image->height / 2 , 20, 20);
//    Mat g_4_1(256, 256, CV_32F, Scalar::all(0));
//    Mat g_4_2(256, 256, CV_32F, Scalar::all(0));
//    Mat gang_4(256, 256, CV_32F, Scalar::all(0));
//
//    Rect region_5(188, image->height / 2 , 20, 20);
//    Mat g_5_1(256, 256, CV_32F, Scalar::all(0));
//    Mat g_5_2(256, 256, CV_32F, Scalar::all(0));
//    Mat gang_5(256, 256, CV_32F, Scalar::all(0));
//
//
//    GaussianBlur(m(region_1), g_1(region_1), Size(11, 11), 0);
//    GaussianBlur(m(region_1), g_2(region_1), Size(31, 31), 0);
//    subtract(g_1, g_2, gang_1);
//
//    GaussianBlur(m(region_2), g_2_1(region_2), Size(17, 17), 0);
//    GaussianBlur(m(region_2), g_2_2(region_2), Size(45, 45), 0);
//    subtract(g_2_2, g_2_1, gang_2);
//
//    GaussianBlur(m(region_3), g_3_1(region_3), Size(13, 13), 0);
//    GaussianBlur(m(region_3), g_3_2(region_3), Size(27, 27), 0);
//    subtract(g_3_2, g_3_1, gang_3);
//
//    GaussianBlur(m(region_4), g_4_1(region_4), Size(11, 11), 0);
//    GaussianBlur(m(region_4), g_4_2(region_4), Size(33, 33), 0);
//    subtract(g_4_2, g_4_1, gang_4);
//
//    GaussianBlur(m(region_5), g_5_1(region_5), Size(15, 15), 0);
//    GaussianBlur(m(region_5), g_5_2(region_5), Size(31, 31), 0);
//    subtract(g_5_2, g_5_1, gang_5);
//
//    /*for (int k = 0; k < gang_5.rows; k++) {
//      for (int u = 0; u < gang_5.cols; u++) {
//        if (gang_1.at<float>(k, u) < 0) gang_1.at<float>(k, u) = 0;
//        if (gang_2.at<float>(k, u) < 0) gang_2.at<float>(k, u) = 0;
//        if (gang_3.at<float>(k, u) < 0) gang_3.at<float>(k, u) = 0;
//        if (gang_4.at<float>(k, u) < 0) gang_4.at<float>(k, u) = 0;
//        if (gang_5.at<float>(k, u) < 0) gang_5.at<float>(k, u) = 0;
//      }
//    }*/
//    Mat m_res = gang_1 + gang_2 + gang_3 + gang_4 + gang_5;
//    IplImage* image_res = cvCloneImage(&(IplImage)m_res);
//    double s = cv::sum(m_res)[0];
//    int k = res->height / 2 - s / 1000 ;
//     cvLine(res, cvPoint(i * 8, pre),
//       cvPoint((i + 1) * 8, k), CV_RGB(255, 255, 255), 1, 8, 0);
//    pre = k;
//    //для отрисовки поля для точки
//    //cvCircle(res, cvPoint(j * 16 + 32, i * 16 + 32), 2, CV_RGB(s - 1250, s - 1250, s - 1250), 3, 8, 0);
//  
//}


//+++++++++++сложная++++++++++++++++++++++++++
cv::Mat m = cvarrToMat(image);

int pre = res->height / 2;
cvZero(res);
cvZero(res_c);
int num = 0;
int v_prev = 0;
int y = 0;
for (int i = 0; i < 30; i++) {
  /*for (int j = 0; j < 13; j++) {*/
    cvZero(image);
    //линия вращаемая
   /* cvLine(image, cvPoint(128 + 100 * cos(i / 57.2), 128 + 100 * sin(i / 57.2)),
      cvPoint(128 - 100 * cos(i / 57.2), 128 - 100 * sin(i / 57.2)),
      CV_RGB(1000, 1000, 1000), 1, 8, 0); */
      //линия: перемещаемая горизонтально
    cvLine(image, cvPoint(40, y + 8 * i), cvPoint(216, y + 8 * i), CV_RGB(1000, 1000, 1000), 1, 8, 0);
   // cvCircle(image, cvPoint(j + 128 - 6, i + 128 - 6), 3, CV_RGB(127, 127, 127), -1, 8, 0);
   //circle
   //  cvCircle(image, cvPoint(128, 128), i, CV_RGB(1000, 1000, 1000), -1, 8, 0);
   //ring
   // cvCircle(image, cvPoint(128, 128), i, CV_RGB(1000, 1000, 1000), 3, 8, 0);

    //-------------Клетка 1---------------- 
    Mat g_1(256, 256, CV_32F, Scalar::all(0));
    Mat g_2(256, 256, CV_32F, Scalar::all(0));
    Mat gang_1(256, 256, CV_32F, Scalar::all(0));
    Rect region_1(70, image->height / 2 - 40, 50, 20);

    Rect region_2(95, image->height / 2 - 40, 50, 20);
    Mat g_2_1(256, 256, CV_32F, Scalar::all(0));
    Mat g_2_2(256, 256, CV_32F, Scalar::all(0));
    Mat gang_2(256, 256, CV_32F, Scalar::all(0));

    Rect region_3(125, image->height / 2 - 40, 40, 20);
    Mat g_3_1(256, 256, CV_32F, Scalar::all(0));
    Mat g_3_2(256, 256, CV_32F, Scalar::all(0));
    Mat gang_3(256, 256, CV_32F, Scalar::all(0));

    Rect region_4(155, image->height / 2 - 40, 20, 20);
    Mat g_4_1(256, 256, CV_32F, Scalar::all(0));
    Mat g_4_2(256, 256, CV_32F, Scalar::all(0));
    Mat gang_4(256, 256, CV_32F, Scalar::all(0));

    Rect region_5(188, image->height / 2 - 40, 40, 20);
    Mat g_5_1(256, 256, CV_32F, Scalar::all(0));
    Mat g_5_2(256, 256, CV_32F, Scalar::all(0));
    Mat gang_5(256, 256, CV_32F, Scalar::all(0));


    GaussianBlur(m(region_1), g_1(region_1), Size(11, 11), 0);
    GaussianBlur(m(region_1), g_2(region_1), Size(31, 31), 0);
    subtract(g_1, g_2, gang_1);

    GaussianBlur(m(region_2), g_2_1(region_2), Size(17, 17), 0);
    GaussianBlur(m(region_2), g_2_2(region_2), Size(45, 45), 0);
    subtract(g_2_2, g_2_1, gang_2);

    GaussianBlur(m(region_3), g_3_1(region_3), Size(13, 13), 0);
    GaussianBlur(m(region_3), g_3_2(region_3), Size(27, 27), 0);
    subtract(g_3_2, g_3_1, gang_3);

    GaussianBlur(m(region_4), g_4_1(region_4), Size(11, 11), 0);
    GaussianBlur(m(region_4), g_4_2(region_4), Size(33, 33), 0);
    subtract(g_4_2, g_4_1, gang_4);

    GaussianBlur(m(region_5), g_5_1(region_5), Size(15, 15), 0);
    GaussianBlur(m(region_5), g_5_2(region_5), Size(31, 31), 0);
    subtract(g_5_2, g_5_1, gang_5);

    //for (int k = 0; k < gang_5.rows; k++) {
    //  for (int u = 0; u < gang_5.cols; u++) {
    //    if (gang_1.at<float>(k, u) < 0) gang_1.at<float>(k, u) = 0;
    //    if (gang_2.at<float>(k, u) < 0) gang_2.at<float>(k, u) = 0;
    //    if (gang_3.at<float>(k, u) < 0) gang_3.at<float>(k, u) = 0;
    //    if (gang_4.at<float>(k, u) < 0) gang_4.at<float>(k, u) = 0;
    //    if (gang_5.at<float>(k, u) < 0) gang_5.at<float>(k, u) = 0;
    //  }
    //}
    //для 1 клетки
    //Mat m_res = gang_1 + gang_2 + gang_3 + gang_4 + gang_5;
    Mat m_res_k1 = gang_1 + gang_2 + gang_3 + gang_4 + gang_5;

    //-------------Клетка 2----------------
    Mat g_1_k2(256, 256, CV_32F, Scalar::all(0));
    Mat g_2_k2(256, 256, CV_32F, Scalar::all(0));
    Mat gang_1_k2(256, 256, CV_32F, Scalar::all(0));
    Rect region_1_k2(70, image->height / 2+20 , 20, 20);

    Rect region_2_k2(95, image->height / 2 + 20, 40, 20);
    Mat g_2_1_k2(256, 256, CV_32F, Scalar::all(0));
    Mat g_2_2_k2(256, 256, CV_32F, Scalar::all(0));
    Mat gang_2_k2(256, 256, CV_32F, Scalar::all(0));

    Rect region_3_k2(125, image->height / 2 + 20, 20, 20);
    Mat g_3_1_k2(256, 256, CV_32F, Scalar::all(0));
    Mat g_3_2_k2(256, 256, CV_32F, Scalar::all(0));
    Mat gang_3_k2(256, 256, CV_32F, Scalar::all(0));

    Rect region_4_k2(155, image->height / 2 + 20, 40, 20);
    Mat g_4_1_k2(256, 256, CV_32F, Scalar::all(0));
    Mat g_4_2_k2(256, 256, CV_32F, Scalar::all(0));
    Mat gang_4_k2(256, 256, CV_32F, Scalar::all(0));

    Rect region_5_k2(188, image->height / 2 + 20, 50, 20);
    Mat g_5_1_k2(256, 256, CV_32F, Scalar::all(0));
    Mat g_5_2_k2(256, 256, CV_32F, Scalar::all(0));
    Mat gang_5_k2(256, 256, CV_32F, Scalar::all(0));


    GaussianBlur(m(region_1_k2), g_1_k2(region_1_k2), Size(11, 11), 0);
    GaussianBlur(m(region_1_k2), g_2_k2(region_1_k2), Size(31, 31), 0);
    subtract(g_1_k2, g_2_k2, gang_1_k2);

    GaussianBlur(m(region_2_k2), g_2_1_k2(region_2_k2), Size(17, 17), 0);
    GaussianBlur(m(region_2_k2), g_2_2_k2(region_2_k2), Size(45, 45), 0);
    subtract(g_2_2_k2, g_2_1_k2, gang_2_k2);

    GaussianBlur(m(region_3_k2), g_3_1_k2(region_3_k2), Size(13, 13), 0);
    GaussianBlur(m(region_3_k2), g_3_2_k2(region_3_k2), Size(27, 27), 0);
    subtract(g_3_2_k2, g_3_1_k2, gang_3_k2);

    GaussianBlur(m(region_4_k2), g_4_1_k2(region_4_k2), Size(11, 11), 0);
    GaussianBlur(m(region_4_k2), g_4_2_k2(region_4_k2), Size(33, 33), 0);
    subtract(g_4_2_k2, g_4_1_k2, gang_4_k2);

    GaussianBlur(m(region_5_k2), g_5_1_k2(region_5_k2), Size(15, 15), 0);
    GaussianBlur(m(region_5_k2), g_5_2_k2(region_5_k2), Size(31, 31), 0);
    subtract(g_5_2_k2, g_5_1_k2, gang_5_k2);

   /* for (int k = 0; k < gang_5_k2.rows; k++) {
      for (int u = 0; u < gang_5_k2.cols; u++) {
        if (gang_1_k2.at<float>(k, u) < 0) gang_1_k2.at<float>(k, u) = 0;
        if (gang_2_k2.at<float>(k, u) < 0) gang_2_k2.at<float>(k, u) = 0;
        if (gang_3_k2.at<float>(k, u) < 0) gang_3_k2.at<float>(k, u) = 0;
        if (gang_4_k2.at<float>(k, u) < 0) gang_4_k2.at<float>(k, u) = 0;
        if (gang_5_k2.at<float>(k, u) < 0) gang_5_k2.at<float>(k, u) = 0;
      }
    }*/
    Mat m_res_k2 = gang_1_k2 + gang_2_k2 + gang_3_k2 + gang_4_k2 + gang_5_k2;
    Mat m_res =0.5* m_res_k1 + 0.2*m_res_k2;

    IplImage* image_res = cvCloneImage(&(IplImage)m_res);
    double s = cv::sum(m_res)[0];
    //cout << s << " " << endl;
    int k = res->height / 2 - s / 1000;
    cout << k << "  ";
     cvLine(res, cvPoint(i * 8, pre), cvPoint((i + 1) * 8, k), CV_RGB(255, 255, 255), 1, 8, 0);
    pre = k;
    //  для отрисовки поля для точки
   // cvCircle(res, cvPoint(j * 16 + 32, i * 16 + 32), 2, CV_RGB(127 - (s - 1400), 127 - (s - 1400), 127 - (s - 1400)), 3, 8, 0);
 // }
}

  

cvNamedWindow("Test OpenCV", CV_WINDOW_AUTOSIZE);
cvShowImage("Test OpenCV", res);
cvSaveImage("field.png", res);
cvWaitKey(0);
return 0;
}

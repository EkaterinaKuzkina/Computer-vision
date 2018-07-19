#include <math.h>
#include <stdio.h>
#include <iostream>
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\core\core.hpp"

//≈сли включен флаг PHASE_ONLY, то осуществл€етс€ подавление амплитудной информации, 
//что позвол€ет выполн€ть фазовую коррел€цию
#define PHASE_ONLY
using namespace std;

//подготавливаетс€ изображение к вычислению преобразовани€ ‘урье через вызов
void GetComplDFT(IplImage *image, IplImage *&image_f, int dftX, int dftY) {
  int i, j;
  image_f = cvCreateImage(cvSize(dftX, dftY), IPL_DEPTH_32F, 2);
  cvZero(image_f);
  for (i = 0; i < image->height; i++) {
    unsigned char *pc = (unsigned char *)
      (image->imageData + i * image->widthStep);
    float *pf = (float *)(image_f->imageData + i * image_f->widthStep);
    for (j = 0; j < image->width; j++) {
      pf[j * 2] = pc[j];
      pf[j * 2 + 1] = 0;
    }
  }
  cvDFT(image_f, image_f, CV_DXT_FORWARD, 0);
#ifdef PHASE_ONLY
  for (i = 0; i < image_f->height; i++) {
    float *pf = (float *)(image_f->imageData + i * image_f->widthStep);
    for (j = 0; j < image_f->width; j++) {
      float v = sqrt(pf[j * 2] * pf[j * 2] + pf[j * 2 + 1] * pf[j * 2 + 1]);
      if (v > 1e-8) {
        pf[j * 2] /= v;
        pf[j * 2 + 1] /= v;
      }
    }
  }
#endif
}

int main() {
  int i, j, n, dftX, dftY;
  int shx, shy;
  double min_v = 1e+12, max_v = 0;
  //загрузили изображени€
  IplImage *image1, *image2, *image_f1, *image_f2, *dst, *res;
  image1 = cvLoadImage("map11.jpg", CV_LOAD_IMAGE_GRAYSCALE);
  image2 = cvLoadImage("map33.jpg", CV_LOAD_IMAGE_GRAYSCALE);
  if (image1 == NULL || image2 == NULL)
    return -2;
  dftX = cvGetOptimalDFTSize(image1->width); //Returns the optimal DFT size for a given vector size.
  n = cvGetOptimalDFTSize(image2->width);
  if (n > dftX) dftX = n;
  dftY = cvGetOptimalDFTSize(image1->height);
  n = cvGetOptimalDFTSize(image2->height);
  if (n > dftY) dftY = n;
  cout << "dftY = " << dftY << " dftX = " << dftX << endl;
  GetComplDFT(image1, image_f1, dftX, dftY);
  GetComplDFT(image2, image_f2, dftX, dftY);
  res = cvCreateImage(cvSize(dftX, dftY), IPL_DEPTH_8U, 1);
  dst = cvCreateImage(cvSize(dftX, dftY), IPL_DEPTH_32F, 2);
  //вычисл€етс€ произведение двух спектров
  cvMulSpectrums(image_f1, image_f2, dst, CV_DXT_MUL_CONJ);
  //void cvMulSpectrums(const CvArr* src1, const CvArr* src2, CvArr* dst, int flags)
  //вызов cvDFT с флагом CV_DXT_INV_SCALE позвол€ет получить кросскоррел€ционное поле
  cvDFT(dst, dst, CV_DXT_INV_SCALE, 0); //void cvDFT(const CvArr* src, CvArr* dst, int flags, int nonzero_rows=0 )
  //поиск максимума на кросскоррел€ционном поле
  for (i = 0; i < res->height; i++) {
    float *pf = (float *)(dst->imageData + i * dst->widthStep);
    for (j = 0; j < res->width; j++) {
      double v = sqrt((pf[j * 2])*(pf[j * 2]) + (pf[j * 2 + 1])*(pf[j * 2 + 1]));
      if (v > max_v) {
        shx = j;
        shy = i;
        max_v = v;
      }
      if (v < min_v) min_v = v;
    }
  }
  if (shx > dftX / 2) {
    shx -= dftX;
  }
  if (shy > dftY / 2) {
    shy -= dftY;
  }
  //нормировка этого пол€ дл€ вывода в файл и на экран
  for (i = 0; i < res->height; i++) {
    unsigned char *pc = (unsigned char *)
      (res->imageData + i * res->widthStep);
    float *pf = (float *)(dst->imageData + i * dst->widthStep);
    for (j = 0; j < res->width; j++) {
      double v = sqrt((pf[j * 2])*(pf[j * 2]) + (pf[j * 2 + 1])*(pf[j * 2 + 1]));
      v = (v - min_v) * 255 / (max_v - min_v);
      pc[j] = (v > 255 ? 255 : (unsigned char)v);
    }
  }
  cvNamedWindow("Fourier", CV_WINDOW_AUTOSIZE);
  cvShowImage("Fourier", res);
  cvSaveImage("corr_field.png", res);
  printf("Found shift: %d, %d\n", shx, shy);
  cvWaitKey(0);
  return 0;
}

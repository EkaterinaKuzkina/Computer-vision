#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
using namespace cv;

//отвечает за вычисление коррел€ции ѕирсона
double correlation(IplImage* imgL, IplImage* imgR, int x, int y, int dx,
  int w_size) {
  //w_size задает размер коррел€ционного окна в пиксел€х (нечетный)
  unsigned char* a = (unsigned char*)imgL->imageData;
  unsigned char* b = (unsigned char*)imgR->imageData;
  double mean1 = 0, mean2 = 0, covxy = 0, std1 = 0, std2 = 0;
  double sq1, sq2, corr, ela, elb;
  int tx, ty, tdx, n = 0;
  int s = w_size / 2;
  int w = imgL->width;
  int h = imgL->height;
  for (int j = -s; j <= s; j++) {
    for (int i = -s; i <= s; i++) {
      tx = x + i;
      tdx = tx + dx;
      ty = y + j;
      if (tx < 0 || tx >= w || tdx < 0 || tdx >= w || ty < 0 || ty >= h) continue;
      mean1 += (double)(a[ty*imgL->widthStep + tx]);
      mean2 += (double)(b[ty*imgL->widthStep + tdx]);
      n++;
    }
  }
  if (n<w_size*w_size / 2) {
    return -1;
  }
  mean1 /= n;
  mean2 /= n;
  for (int j = -s; j <= s; j++) {
    for (int i = -s; i <= s; i++) {
      tx = x + i;
      tdx = tx + dx;
      ty = y + j;
      if (tx < 0 || tx >= w || tdx < 0 || tdx >= w || ty < 0 || ty >= h)  continue;
      ela = (double)a[ty*imgL->widthStep + tx];
      elb = (double)b[ty*imgL->widthStep + tdx];
      sq1 = (ela - mean1);
      sq2 = (elb - mean2);
      covxy += sq1*sq2;
      std1 += sq1*sq1;
      std2 += sq2*sq2;
    }
  }
  corr = covxy / (sqrt(std1*std2));
  return corr;
}

//вычисл€ет матрицу коррел€ционной энергии
int compute_cmatrix(IplImage* imgL, IplImage* imgR, double** cor_matr,
  int  dMin, int dMax) {
  double* cor_matrix = *cor_matr;
  int D = dMax - dMin + 1;
  int W = imgL->width;
  for (int i = 0; i<imgL->height; i++) {
    for (int j = 0; j<imgL->width; j++) {
      for (int d = 0; d<D; d++) {
        cor_matrix[(i*W + j)*D + d] = correlation(imgL, imgR, j, i, -(d + dMin), 43);
      }
    }
  }
  return 1;
}

//поиск окон с максимальным коэффициентом коррел€ции
//ищет смещение окна, которому соответствует максимальное значение 
//в матрице коррел€ционной энергии
int findMax(double* srcAry, int aryLth) {
  int maxIdx = 0;
  double maxVal = -1000;
  for (int i = 0; i<aryLth; i++) {
    if (srcAry[i] > maxVal) {
      maxVal = srcAry[i];
      maxIdx = i;
    }
  }
  return maxIdx;
}

int main() {
  //IplImage *imgL = 0;
  //IplImage *imgR = 0;
  ////диапазон допустимых диспаратностей
  //int dMin = 0, dMax = 50;
  //int D = dMax - dMin + 1;
  ////используетс€ дл€ приведени€ карты диспаратности dispimgL к изображению с интенсивност€ми пикселей от 0 до 255
  //double scale = 0;
  //double* cor_matrix;
  ////загрузка изображений
  //imgL = cvLoadImage("im3.ppm", 0);
  //imgR = cvLoadImage("im8 (2).ppm", 0);
  //cor_matrix = (double*)malloc(imgL->width*imgL->height*D
  //  * sizeof(double));
  //compute_cmatrix(imgL, imgR, &cor_matrix, dMin, dMax);
  ////карта диспаратности
  //IplImage* dispimgL = cvCreateImage(cvSize(imgL->width, imgL->height),
  //  8, 1);
  //scale = 255.0 / D;
  //for (int i = 0; i<imgL->height; i++) {
  //  for (int j = 0; j<imgL->width; j++) {
  //    int max = findMax(cor_matrix + (i*imgL->width + j)*D, D);
  //    dispimgL->imageData[i*imgL->widthStep + j] = (max)*scale;
  //  }
  //}
  ////сглаживание полученной карты глубины 
  //cvSmooth(dispimgL, dispimgL, CV_MEDIAN, 3, 3, 0, 0);
  //cvSaveImage("disp.png", dispimgL);
  //cvReleaseImage(&imgL);
  //cvReleaseImage(&imgR);
  //free(cor_matrix);
	
	uchar red[64] = { 255, 248, 194 ,148, 169, 215 ,255, 255,
		255, 238, 172 ,115, 130, 178 ,255, 255,
		255, 208, 127 ,59, 64, 112 ,208, 255,
		255, 223 ,143, 74 ,77, 120, 211 ,255,
		237, 192, 133 ,83 ,85, 118, 184, 222,
		177, 161, 146 ,132 ,145 ,162 ,201 ,217,
		56, 73, 101 ,126 ,144 ,147 ,147, 141,
		0 ,17 ,76 ,126 ,153, 146, 127 ,108 };
	uchar green[64] = { 231, 185, 117, 72 ,67 ,113, 171, 217,
		229, 175, 95 ,39 ,28, 76 ,139 ,189,
		254, 192, 100, 31, 15, 63, 131,185,
		255, 207, 115, 46, 28, 71, 134, 185,
		255, 241, 175, 125, 112, 145, 193, 230,
		226, 210, 187, 173, 172, 189, 209, 225,
		149, 166, 191, 216, 229, 232, 225, 220,
		72 ,110 ,166 ,216, 238, 231 ,206, 186};
	uchar blue[64] = { 255, 255, 249 ,203, 178 ,224 ,255, 255,
		255, 255, 226, 170, 140, 187, 224, 255,
		255, 255, 192, 123, 91 ,138 ,184 ,238 ,
		255, 255, 208, 139, 103, 146, 188, 239,
		255, 255, 202, 152, 128, 161, 194, 232,
		255, 244, 215, 200, 188, 205, 210, 227,
		108, 125, 148, 172, 182, 184, 172, 167,
		31 ,69, 122, 172, 191, 183, 153, 134 };

	Mat outputMat = Mat(8, 8, CV_8UC3);

	// use outputMat to draw some stuff

	Mat channelR = Mat(8, 8, CV_8UC1, red );
	Mat channelG = Mat(8, 8, CV_8UC1, green);
	Mat channelB = Mat(8, 8, CV_8UC1, blue);

	std::vector<Mat> channels{ channelB, channelG, channelR };


	
	merge(channels, outputMat);
	imwrite("output.bmp", outputMat);
	/*int *data = [];
	cv::Mat image = cv::Mat(width, height, CV_8UC4, (unsigned*)data);*/
  return 0;
}
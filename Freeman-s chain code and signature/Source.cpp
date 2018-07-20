#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\imgcodecs.hpp"
#include <iostream>
#include <fstream>
#define _USE_MATH_DEFINES
#include <list>
#include <vector>
#include <math.h>

using namespace cv;
using namespace std;

void main() {
  Mat im = imread("lil.jpg", CV_LOAD_IMAGE_GRAYSCALE);
  Mat img;
  blur(im, img, Size(3, 3));
  Canny(img, img, 255, 255, 3);
  imwrite("1.jpg", img);

  vector <int> fl(img.total(), 255);
  int curI = 0;
  int curJ = 0;
  int Istart, Jstart;
  list <int> state;
  list <int> dif;
  list <int> minDif;
  list <int> minState;
  int tempI;
  int tempJ;
  bool flag = false;
  //ищем первый попавшийся элемент границы
  for (int i = 0; i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++)
      if (img.at<unsigned char>(i, j) == 255) {
        Istart = i;
        Jstart = j;
        curI = i;
        curJ = j;
        flag = true;
        break;
      }
    if (flag) break;
  }
  //cout <<"Istart = "<< Istart << "  Jstart = " << Jstart << endl;
  do {
    tempI = curI;
    tempJ = curJ;
    fl[curI*img.cols + curJ] = 0;
    fl[Istart*img.cols + Jstart] = 255;
    //cout << curI << " " << curJ << endl ;
    if ((curJ + 1 < img.cols) && (img.at<unsigned char>(curI, curJ + 1) > 0) && fl[curI*img.cols + curJ + 1] == 255){
      cout << "0";
      curJ++;
      state.push_back(0);
    }
    else if ((curJ + 1 < img.cols) && (curI + 1 < img.rows) && (img.at<unsigned char>(curI + 1, curJ + 1) > 0) && fl[(curI + 1)*img.cols + curJ + 1] == 255) {
        cout << "7" ;
        curI++;
        curJ++;
        state.push_back(7);
    }
    else if ((curI + 1 < img.rows) && (img.at<unsigned char>(curI + 1, curJ) > 0) && fl[(curI + 1)*img.cols + curJ] == 255) {
          cout << "6";
          curI++;
          state.push_back(6);
    }
    else if ((curJ - 1 >= 0) && (curI + 1 < img.rows) && (img.at<unsigned char>(curI + 1, curJ - 1) > 0) && fl[(curI + 1)*img.cols + curJ - 1] == 255) {
            cout << "5" ;
            curI++;
            curJ--;
            state.push_back(5);
    }
    else if ((curJ - 1 >= 0) && (img.at<unsigned char>(curI, curJ - 1) > 0) && fl[curI*img.cols + curJ - 1] == 255) {
              cout << "4";
              curJ--;
              state.push_back(4);
    }
    else if ((curI - 1 >= 0) && (curJ - 1 >= 0) && (img.at<unsigned char>(curI - 1, curJ - 1)> 0) && fl[(curI - 1)*img.cols + curJ - 1] == 255) {
                cout << "3" ;
                curI--;
                curJ--;
                state.push_back(3);
      }
      else if ((curI - 1 >= 0) && (img.at<unsigned char>(curI - 1, curJ) > 0) && fl[(curI - 1)*img.cols + curJ + 1] == 255) {
                  cout << "2" ;
                  curI--;
                  state.push_back(2); 
      }
      else if ((curI - 1 >= 0) && (curJ + 1 < img.cols) && (img.at<unsigned char>(curI - 1, curJ + 1) > 0) && fl[(curI - 1)*img.cols + curJ + 1] == 255) {
                    cout << "1" ;
                    curI--;
                    curJ++;
                    state.push_back(1);
      }
     if ((tempI == curI) && (tempJ == curJ)) {
      break;
    }
    //cout <<"curI = "<< curI << " curJ = "<< curJ << endl;
  } while (curI != Istart || curJ != Jstart);
  cout << endl;
  cout << endl;
  for (int q = 0; q < state.size(); q++) {
    int temp, diff;
    temp = state.front();
    list<int>::iterator i = state.begin();
    i++;
    for (; i != state.end(); i++) {
      diff = *i - temp;
      temp = *i;
      if (diff < 0) diff += 8;
      dif.push_back(diff);
    }

    diff = state.front() - state.back();
    if (diff < 0) diff += 8;
    dif.push_back(diff);

    if (minDif.empty()) {
      for (list<int>::iterator v = dif.begin(); v != dif.end(); v++)
        minDif.push_back(*v);
      if (minState.empty()) {
        for (list<int>::iterator v = state.begin(); v != state.end(); v++)
          minState.push_back(*v);
      }
    }
    else {
      for (list<int>::iterator g = dif.begin(), j = minDif.begin(); g != dif.end(); g++, j++)
        if (*g < *j) {
          minState.clear();
          minDif.clear();
          for (list<int>::iterator v = dif.begin(); v != dif.end(); v++)
            minDif.push_back(*v);
          for (list<int>::iterator v = state.begin(); v != state.end(); v++)
            minState.push_back(*v);
          break;
        }
        else if (*g > *j)
          break;
    }
    int front = state.front();
    state.pop_front();
    state.push_back(front);
    dif.clear();
  }

  for (list<int>::iterator i = minDif.begin(); i != minDif.end(); i++)
    cout << *i;
  cout << endl;
  cout << endl;
  for (list<int>::iterator i = minState.begin(); i != minState.end(); i++)
    cout << *i;
  cout << endl;

  //_____________строим сигнатуру__________________
  Mat im3 = imread("lil.jpg", CV_LOAD_IMAGE_GRAYSCALE);
  // считаем центр тяжести
  long long a = 0;
  long long b = 0;
  long long c = 0;
  for (int y = 0; y < im3.rows; y++) {
    for (int x = 0; x < im3.cols; x++) {
      a += y * im3.at<unsigned char>(y, x);
      b += x * im3.at<unsigned char>(y, x);
      c += im3.at<unsigned char>(y, x);
    }
  }
  long long yC = a / c;  //центр для у
  long long xC = b / c;  //центр для х
  
  double maxF = -7564467;
  double maxD = -1547566;
  vector <pair<double, double>> vec;

  for (int i = 0; i < im3.rows; i++) {
    for (int j = 0; j < im3.cols; j++) {
      if (im3.at<unsigned char>(i, j) < 100) {  //если контур
        double r = sqrt(double((i - yC)*(i - yC) + (j - xC)*(j - xC)));  //растояние от центра до точки
        double f = atan2(double(j - xC), double(i - yC));  //угол наклона
        vec.push_back(make_pair(r, f)); //запомнили точку
        if (r > maxD) maxD = r;   //ищем максимальны радиус (высота)
        if (abs(f) > maxF) maxF = abs(f);  //ищем максимальный радиус (ширина)
      }
    }
  }
  Mat res(Size(int(maxF * 100 + 500), int(maxD )), CV_8U, Scalar(255));
  cout << "res.rows = " << res.rows << " res.cols = " << res.cols << endl;
  //для вывода
  for (int i = 0; i < vec.size(); i++) {
    cout << "res.rows - vec[i].first  = " << res.rows - vec[i].first  << " (vec[i].second + maxF) * 100 = " << (vec[i].second + maxF) * 100 << endl;
    res.at<unsigned char>(res.rows - vec[i].first , (vec[i].second + maxF) * 100) = 0;
  }
  imwrite("result.png", res);
}

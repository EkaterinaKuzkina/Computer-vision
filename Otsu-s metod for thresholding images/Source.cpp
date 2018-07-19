#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\imgcodecs.hpp"
#include <iostream>
#include <fstream>
#include <math.h>

using namespace cv;
using namespace std;


int m = 4;
string filename = "2.jpg";
vector<double> p;
int L = 0;
string outputname = "result2.jpg";
double variance_max = 0;
vector<int> ft_max(m, 0);

int findMaxBright(Mat& src)
{
  int max_L = 0;
  int rows = src.rows,
    cols = src.cols;
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      if (src.at<unsigned char>(i, j) > max_L)
        max_L = src.at<unsigned char>(i, j);
  return max_L;
}

void getVarParams(Mat& src)
{
  p.resize(L + 1);
  vector<unsigned int> brightFrequency(L + 1);
  int rows = src.rows,
    cols = src.cols;
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      brightFrequency[(int)src.at<unsigned char>(i, j)]++;
  for (int i = 0; i <= L; ++i)
    p[i] = (double)brightFrequency[i] / (rows * cols);
}

void check_thresholds(vector<int>& ft)
{
  vector<double> q(m, 0);
  vector<double> mu(m, 0);
  double mu_ = 0;
  double varianceb = 0;

  ft.push_back(L);
  ft.insert(ft.begin(), 0);
  int m_temp = ft.size();
  for (int i = 0; i < m; ++i)
  {
    for (int j = ft[i]; j < ft[i + 1]; ++j)
      q[i] += p[j];
  }
  for (int i = 0; i < m; ++i)
  {
    for (int j = ft[i]; j < ft[i + 1]; ++j)
      mu[i] += (double)j * p[j];
    mu[i] /= q[i];
  }
  for (int i = 0; i < m; ++i)
    varianceb += q[i] * mu[i] * mu[i];
  if (varianceb > variance_max)
  {
    variance_max = varianceb;
    ft_max = ft;
  }
  ft.pop_back();
  ft.erase(ft.begin());
}

void get_thresholds(vector<int>& ft, int left, int right, int counter) //counter = M - 1
{
  if (!counter)
  {
    check_thresholds(ft);
  }
  else
  {
    for (int i = left; i <= right; ++i)
    {
      ft.push_back(i);
      get_thresholds(ft, i + 1, right, counter - 1);
      ft.pop_back();
    }
  }
}


int get_threshold()
{
  int ft_max = 1;
  double q1 = 0;
  double q2 = 0;
  double mu1 = 0,
    mu2 = 0,
    mu = 0;
  double variance1 = 0,
    variance2 = 0,
    varianceW = 0,
    varianceB = 0,
    variance_max = 0;

  for (int ft = 1; ft <= L; ++ft)
  {
    variance1 = 0;
    variance2 = 0;
    varianceW = 0;
    varianceB = 0;
    mu1 = 0;
    q1 = 0;
    q2 = 0;
    mu2 = 0;
    for (int i = 0; i < ft; ++i)
      q1 += p[i];
    for (int i = ft; i <= L; ++i)
      q2 += p[i];
    //считаем mu - средние значения кластеров
    for (int i = 0; i < ft; ++i)
      mu1 += (double)(i)* p[i] / q1;
    for (int i = ft; i <= L; ++i)
      mu2 += (double)i * p[i] / q2;
    mu = q1 * mu1 + q2 * mu2;
    //внутрикластерные дисперсии
    for (int i = 0; i < ft; ++i)
      variance1 += ((double)i - mu1) * ((double)i - mu1) * p[i];
    variance1 /= q1;
    for (int i = ft; i <= L; ++i)
      variance2 += ((double)i - mu2) * ((double)i - mu2) * p[i];
    variance2 /= q2;
    varianceW = q1 * variance1 + q2 * variance2;
    //межкластерная дисперсия
    varianceB = q1 * (mu1 - mu) * (mu1 - mu) + q2 * (mu2 - mu) * (mu2 - mu);
    if (varianceB > variance_max)
    {
      variance_max = varianceB;
      ft_max = ft;
    }

  }
  return ft_max;
}

void show_clusters(Mat& image, int m)
{
  Mat result(image.rows, image.cols, CV_32FC1);
  getVarParams(image);
  if (m == 2)
  {
    int t = get_threshold();
    for (int i = 0; i < image.rows; ++i)
      for (int j = 0; j < image.cols; ++j)
        if (image.at<unsigned char>(i, j) < t)
          result.at<float>(i, j) = 0;
        else
          result.at<float>(i, j) = 255;
  }
  else
  {
    vector<int> t;
    get_thresholds(t, 1, L - 1, m);
    for (int i = 0; i < image.rows; ++i)
      for (int j = 0; j < image.cols; ++j)
        for (int k = 1; k < m + 2; ++k)
          if (image.at<unsigned char>(i, j) < ft_max[k] && image.at<unsigned char>(i, j) > ft_max[k - 1])
          {
            result.at<float>(i, j) = (k - 1) * (255 / m);
            break;
          }
  }
  imwrite(outputname, result);
}

int main()
{
  Mat image = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
  //blur(image, image, Size(3, 3));
  L = findMaxBright(image);
  show_clusters(image, m);
  return 0;
}

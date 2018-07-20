#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\imgcodecs.hpp"
#include <iostream>
#include <fstream>
#include <math.h>

using namespace cv;
using namespace std;

Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Edge Map";

/**
* @function CannyThreshold
* @brief Trackbar callback - Canny thresholds input with a ratio 1:3
*/
void CannyThreshold(int, void*)
{
  /// Reduce noise with a kernel 3x3
  blur(src, detected_edges, Size(3, 3));
  imwrite("1.jpg", detected_edges);
  /// Canny detector
  Canny(detected_edges, detected_edges, 255, 255, kernel_size);
  /// display our result
  imshow(window_name, detected_edges);
}


/** @function main */
int main(int argc)
{
  /// Load an image
  src = imread("ננ.jpg", IMREAD_GRAYSCALE);
  imwrite("fig1.jpg", src);
  if (!src.data)
  {
    return -1;
  }

  /// Create a matrix of the same type and size as src (for dst)
  dst.create(src.size(), src.type());

  /// Create a window
  namedWindow(window_name, CV_WINDOW_AUTOSIZE);

  /// Create a Trackbar for user to enter threshold
  createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);

  /// Show the image
  CannyThreshold(0, 0);

  /// Wait until user exit program by pressing a key
  waitKey(0);

  return 0;
}


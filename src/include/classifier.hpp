#ifndef CLASSIFIER_HPP_
#define CLASSIFIER_HPP_

#define THRESHOLD_METHOD_KITTLER 1
#define THRESHOLD_METHOD_KAPUR 2
#define THRESHOLD_METHOD_YEN 4
#define THRESHOLD_METHOD_OTSU 8
#define THRESHOLD_METHOD_PAL_1 16
#define THRESHOLD_METHOD_PAL_2 32
#define THRESHOLD_TYPE_BINARY 64
#define THRESHOLD_TYPE_BINARY_INV 128

#define BOOST_PYTHON_MAX_ARITY 25
#define M_LOG2E 1.44269504088896340736 //log2(e)

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <cmath>
#include <limits>
#include <boost/python.hpp>
#include <boost/numpy.hpp>

using namespace cv;
using namespace std;
using namespace boost::python;

namespace p = boost::python;
namespace np = boost::numpy;


class Classifier
{
private:
  double hist[256*256];
  double prob[256*256];
  double acc[256*256];

  int getKittlerThreshold(unsigned char* image,int cols, int rows);
  int getOtsuThreshold(unsigned char* image,int cols, int rows);
  int getKapurThreshold(unsigned char* image,int cols, int rows);
  int getYenThreshold(unsigned char* image,int cols, int rows);
  int getSpatialPal1Threshold(unsigned char* image,int cols, int rows);
  int getSpatialPal2Threshold(unsigned char* image,int cols, int rows);

public:
  void threshold(unsigned char* image, unsigned char* output, int threshold_flag, int cols, int rows);
  void thresholdMatrix(unsigned char* image, unsigned char* thresh, unsigned char* output, int threshold_flag, int cols, int rows);

  void wrapThreshold(np::ndarray & image, np::ndarray & output, int threshold_flag);
  void wrapThresholdMatrix(np::ndarray & image, np::ndarray & thresh, np::ndarray & output, int threshold_flag);

};

#endif

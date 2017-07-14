#ifndef FILTERING_HPP_
#define FILTERING_HPP_

#define MEDIAN_FILTER 1
#define GAUSSIAN_FILTER 2
#define MEAN_FILTER 3

#define BOOST_PYTHON_MAX_ARITY 25

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <cmath>
#include <limits>
#include <boost/python/numpy.hpp>

using namespace cv;
using namespace std;
using namespace boost::python;

namespace p = boost::python;
namespace np = boost::python::numpy;


class Filtering
{
private:
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

public:
  Filtering();

  void filter(unsigned char* image, unsigned char* output, int filter_flag, int cols, int rows);
  void fillContours(unsigned char* image, unsigned char* output, int value, int cols, int rows);
  void fillConvexHull(unsigned char* image, unsigned char* output, int value, int cols, int rows);
  void wrapFilter(np::ndarray & image, np::ndarray & output, int filter_flag);
  void wrapFillContours(np::ndarray & image, np::ndarray & output, int value);
  void wrapFillConvexHull(np::ndarray & image, np::ndarray & output, int value);
};

#endif

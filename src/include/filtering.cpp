extern "C"

#include "filtering.hpp"

BOOST_PYTHON_MODULE(Filtering)
{
    // Create the Python type object for our extension class and define __init__ function.
    class_<Filtering>("Filtering",init<>())
        .def("filter", &Filtering::filter)  // Add a regular member function.
        .def("wrapFilter", &Filtering::wrapFilter)
        .def("fillContours", &Filtering::fillContours)
        .def("wrapFillContours", &Filtering::wrapFillContours)
        .def("fillConvexHull", &Filtering::fillConvexHull)
        .def("wrapFillConvexHull", &Filtering::wrapFillConvexHull)
        ;
}

Filtering::Filtering()
{

}

void Filtering::filter(unsigned char* image, unsigned char* output, int filter_flag, int cols, int rows)
{
  cv::Mat imageMat = cv::Mat(rows,cols,CV_8UC1,image);
  cv::Mat outputMat = cv::Mat(rows,cols,CV_8UC1,output);

  cv::medianBlur(imageMat, outputMat, 11);
}

void Filtering::fillContours(unsigned char* image, unsigned char* output, int value, int cols, int rows)
{
  cv::Mat imageMat = cv::Mat(rows,cols,CV_8UC1,image);
  cv::Mat outputMat = cv::Mat(rows,cols,CV_8UC1,output);

  findContours(imageMat,contours,hierarchy,CV_RETR_EXTERNAL,CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

  for (unsigned int i=0;i<contours.size();i++) {
    int area = contourArea(contours[i]);
    int areaImage = cols*rows;
    if (area>areaImage*0.001){// && area<areaImage*0.05){
      drawContours(outputMat, contours, i, Scalar(value),CV_FILLED);
    }
  }
}

void Filtering::fillConvexHull(unsigned char* image, unsigned char* output, int value, int cols, int rows)
{
  cv::Mat imageMat = cv::Mat(rows,cols,CV_8UC1,image);
  cv::Mat outputMat = cv::Mat(rows,cols,CV_8UC1,output);

  findContours(imageMat,contours,hierarchy,CV_RETR_EXTERNAL,CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
  vector<vector<Point> >hull( contours.size() );
  for(unsigned int i = 0; i < contours.size(); i++ ){
    convexHull(Mat(contours[i]), hull[i], false );
  }
  for (unsigned int i=0;i<hull.size();i++) {
    int area = contourArea(hull[i]);
    int areaImage = cols*rows;
    if (area>areaImage*0.001){//&& area<areaImage*0.05){
      drawContours(outputMat, hull, i, Scalar(value),CV_FILLED);
    }
  }
}

void Filtering::wrapFilter(np::ndarray & image, np::ndarray & output, int filter_flag)
{
  this->filter(
    reinterpret_cast<unsigned char*>(image.get_data()),
    reinterpret_cast<unsigned char*>(output.get_data()),
    filter_flag,
    image.shape(1),
    image.shape(0));
}

void Filtering::wrapFillContours(np::ndarray & image, np::ndarray & output, int value)
{
  this->fillContours(
    reinterpret_cast<unsigned char*>(image.get_data()),
    reinterpret_cast<unsigned char*>(output.get_data()),
    value,
    image.shape(1),
    image.shape(0));
}

void Filtering::wrapFillConvexHull(np::ndarray & image, np::ndarray & output, int value)
{
  this->fillConvexHull(
    reinterpret_cast<unsigned char*>(image.get_data()),
    reinterpret_cast<unsigned char*>(output.get_data()),
    value,
    image.shape(1),
    image.shape(0));
}

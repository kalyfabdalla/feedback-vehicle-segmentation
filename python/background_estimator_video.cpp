#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include "include/background_estimator.hpp"

using namespace cv;

int main()
{
  cv::VideoCapture cap(0);
  Mat frame;
  cap >> frame;

  PixelBackgroundModel model(0.001f,40,9.0f,0.97f,11.0f,0.05f,7.0f,4,1,0.5f,3,640,480,640*480,0);

  while(waitKey(1)!=27)
  {
    cap >> frame;
    imshow("camera",frame);
  }

  return 0;
}

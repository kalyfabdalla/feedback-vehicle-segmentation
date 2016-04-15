/*
 * threshold.hpp
 *
 *  Created on: 07/03/2016
 *      Author: kalyfabdalla
 */

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <cmath>
#include <limits>

#define M_LOG2E 1.44269504088896340736 //log2(e)

using namespace cv;
using namespace std;

#ifndef THRESHOLD_HPP_
#define THRESHOLD_HPP_

int getKittlerThreshold(Mat image);
int getOtsuThreshold(Mat image);
int getKapurThreshold(Mat image);
int getYenThreshold(Mat image);
int getSpatialPal1Threshold(Mat image);
int getSpatialPal2Threshold(Mat image);


#endif /* THRESHOLD_HPP_ */

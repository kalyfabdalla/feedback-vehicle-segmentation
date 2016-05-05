#include <stdlib.h>
#include <memory.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
//#include "pointer.hpp"

using namespace cv;

typedef struct PixelGMMZ
{
	float sigma;
	float muR;
	float muG;
	float muB;
	float weight;
}PixelGMMZ;

//*/
class PixelBackgroundModel : public cv::ParallelLoopBody
{
private:
	float fAlphaT;
	float fTb;
	float fTg;
	float fTB;
	float fSigma;
	float fCT;
	float fDnorm;
	int nM;
	bool bShadowDetection;
	float fTau;
	int nNBands;
	int nWidth;
	int nHeight;
	int nSize;
	PixelGMMZ* rGMM;
	unsigned char* rnUsedModes;
	bool bRemoveForeground;
	unsigned char * data;
	unsigned char * output;
	float * values;

	int line;

public:
	int roi_x;
	int roi_y;
	int roi_width;
	int roi_height;

	PixelBackgroundModel();

	PixelBackgroundModel(float fAlphaT, float fTb, float fTg, float fTB, float fSigma, float fCT, float fDnorm, int nM,
							bool bShadowDetection, float fTau, int nNBands, int nWidth, int nHeight, int nSize,
							bool bRemoveForeground );

	void setRoi(int x, int y, int width, int height) ;

	void backgroundModel(long data, long output, long values);

  void operator ()(const cv::Range& range) const;

};

#include <stdlib.h>
#include <memory.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include <boost/python.hpp>

using namespace boost::python;

typedef struct PixelGMMZ
{
	float sigma;
	float muR;
	float muG;
	float muB;
	float weight;
}PixelGMMZ;

//*/

int _backgroundModel(	long posPixel, float red, float green, float blue,  float * distance, float dnorm, unsigned char* pModesUsed, PixelGMMZ* m_aGaussians,
									int m_nM, float m_fAlphaT, float m_fTb, float m_fTB, float m_fTg, float m_fSigma, float m_fPrune) {

		long pos;
		bool bFitsPDF=0;
		bool bBackground=0;
		float m_fOneMinAlpha=1-m_fAlphaT;
		int nModes=*pModesUsed;
		float totalWeight=0.0f;
		float dstFinal = dnorm;
		for (int iModes=0;iModes<nModes;iModes++)
		{
			pos=posPixel+iModes;
			float weight = m_aGaussians[pos].weight;

			if (!bFitsPDF)
			{
				float var = m_aGaussians[pos].sigma;
				float muR = m_aGaussians[pos].muR;
				float muG = m_aGaussians[pos].muG;
				float muB = m_aGaussians[pos].muB;

				float dR=muR - red;
				float dG=muG - green;
				float dB=muB - blue;

				float dist=(dR*dR+dG*dG+dB*dB);
				float mahala = dist/var;
				//background? - m_fTb
				if ((totalWeight<m_fTB)&&(dist<m_fTb*var))
						bBackground=1;		
				if ((totalWeight<m_fTB)&&(mahala<dstFinal))
					dstFinal=mahala;
				//check fit
				if (dist<m_fTg*var)
				{
					/////
					//belongs to the mode
					bFitsPDF=1;

					//update distribution
					float k = m_fAlphaT/weight;
					weight=m_fOneMinAlpha*weight+m_fPrune;
					weight+=m_fAlphaT;
					m_aGaussians[pos].muR = muR - k*(dR);
					m_aGaussians[pos].muG = muG - k*(dG);
					m_aGaussians[pos].muB = muB - k*(dB);

					float sigmanew = var + k*(dist-var);

					m_aGaussians[pos].sigma =sigmanew< 4 ? 4 : sigmanew>5*m_fSigma?5*m_fSigma:sigmanew;

					for (int iLocal = iModes;iLocal>0;iLocal--)
					{
						long posLocal=posPixel + iLocal;
						if (weight < (m_aGaussians[posLocal-1].weight))
						{
							break;
						}
						else
						{
							//swap
							PixelGMMZ temp = m_aGaussians[posLocal];
							m_aGaussians[posLocal] = m_aGaussians[posLocal-1];
							m_aGaussians[posLocal-1] = temp;
						}
					}

					//belongs to the mode
					/////
				}
				else
				{
					weight=m_fOneMinAlpha*weight+m_fPrune;
					//check prune
					if (weight<-m_fPrune)
					{
						weight=0.0;
						nModes--;
					//	bPrune=1;
						//break;//the components are sorted so we can skip the rest
					}
				}
				//check if it fits the current mode (2.5 sigma)
				///////
			}
			//fit not found yet
			/////
			else
			{
					weight=m_fOneMinAlpha*weight+m_fPrune;
					//check prune
					if (weight<-m_fPrune)
					{
						weight=0.0;
						nModes--;
						//bPrune=1;
						//break;//the components are sorted so we can skip the rest
					}
			}
			totalWeight+=weight;
			m_aGaussians[pos].weight=weight;
		}
		//go through all modes
		//////

		//renormalize weights
		for (int iLocal = 0; iLocal < nModes; iLocal++)
		{
			m_aGaussians[posPixel+ iLocal].weight = m_aGaussians[posPixel+ iLocal].weight/totalWeight;
		}

		//make new mode if needed and exit
		if (!bFitsPDF)
		{
			if (nModes==m_nM)
			{
			   //replace the weakest
			}
			else
			{
			   //add a new one
				//totalWeight+=m_fAlphaT;
				//pos++;
				nModes++;
			}
			pos=posPixel+nModes-1;

      		if (nModes==1)
				m_aGaussians[pos].weight=1;
			else
				m_aGaussians[pos].weight=m_fAlphaT;

			//renormalize weights
			int iLocal;
			for (iLocal = 0; iLocal < nModes-1; iLocal++)
			{
				m_aGaussians[posPixel+ iLocal].weight *=m_fOneMinAlpha;
			}

			m_aGaussians[pos].muR=red;
			m_aGaussians[pos].muG=green;
			m_aGaussians[pos].muB=blue;
			m_aGaussians[pos].sigma=m_fSigma;

			//sort
			//find the new place for it
			for (iLocal = nModes-1;iLocal>0;iLocal--)
			{
				long posLocal=posPixel + iLocal;
				if (m_fAlphaT < (m_aGaussians[posLocal-1].weight))
				{
							break;
				}
				else
				{
					//swap
					PixelGMMZ temp = m_aGaussians[posLocal];
					m_aGaussians[posLocal] = m_aGaussians[posLocal-1];
					m_aGaussians[posLocal-1] = temp;
				}
			}
		}

		//set the number of modes
		*pModesUsed=nModes;

		*distance = dstFinal/dnorm;
		return bBackground;
	}

	int _removeShadow (	long posPixel, float red, float green, float blue, float * chroma, float * lumina, unsigned char nModes, PixelGMMZ* m_aGaussians,
						int m_nM, float m_fTb, float m_fTB, float m_fTg, float m_fTau) {
		//calculate distances to the modes (+ sort???)
		//here we need to go in descending order!!!
	//	long posPixel = pixel * m_nM;
		long pos;
		float tWeight = 0;
		float numerator, denominator;
		// check all the distributions, marked as background:
		for (int iModes=0;iModes<nModes;iModes++)
		{
			pos=posPixel+iModes;
			float var = m_aGaussians[pos].sigma;
			float muR = m_aGaussians[pos].muR;
			float muG = m_aGaussians[pos].muG;
			float muB = m_aGaussians[pos].muB;
			float weight = m_aGaussians[pos].weight;
			tWeight += weight;

			numerator = red * muR + green * muG + blue * muB;
			denominator = muR * muR + muG * muG + muB * muB;
			// no division by zero allowed
			if (denominator == 0)
			{
					break;
			};
			float a = numerator / denominator;
			*lumina = a;
			// if tau < a < 1 then also check the color distortion
			if ((a <= 1) && (a >= m_fTau))//m_nBeta=1
			{
				float dR=a * muR - red;
				float dG=a * muG - green;
				float dB=a * muB - blue;
				float dist=(dR*dR+dG*dG+dB*dB);
				*chroma = (dist/(var*a*a))/dnorm;
				if (dist<m_fTb*var*a*a)
				{
					return 2;
				}
			};
			if (tWeight > m_fTB)
			{
					break;
			};
		};
		return 0;
	}

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

	PixelBackgroundModel(	float fAlphaT, float fTb, float fTg, float fTB, float fSigma, float fCT, int nM,
							bool bShadowDetection, float fTau, int nNBands, int nWidth, int nHeight, int nSize,
							bool bRemoveForeground ) :
							fAlphaT(fAlphaT), fTb(fTb), fTg(fTg), fTB(fTB), fSigma(fSigma), fCT(fCT), nM(nM),
							bShadowDetection(bShadowDetection), fTau(fTau), nNBands(nNBands), nWidth(nWidth), nHeight(nHeight),
							nSize(nSize), bRemoveForeground(bRemoveForeground) {
								rGMM=(PixelGMMZ*) malloc(nSize * nM * sizeof(PixelGMMZ));
								rnUsedModes = (unsigned char* ) malloc(nSize);
								memset(rnUsedModes,0,nSize);//no modes used
							}

	void setRoi(int x, int y, int width, int height) {
		roi_x = x;
		roi_y = y;
		roi_width = width;
		roi_height = height;
	}

	void backgroundModel(unsigned char* data, unsigned char* output, float * values) {
		this->data = data;
		this->output = output;
		this->values = values;
		
		for (line=roi_y;line<roi_y+roi_height;line++) {
			cv::parallel_for_(cv::Range(roi_x,roi_x+roi_width), *this);
		}

		
	}

    void operator ()(const cv::Range& range) const
    {
		float fPrune=-fAlphaT*fCT;
		unsigned char* pUsedModes;
        int i,idx;
		for (idx = range.start; idx < range.end; ++idx)
		{
			i = line*nWidth+idx;
			// retrieve the colors
			float red = data[i*3+0];
			float green = data[i*3+1];
			float blue = data[i*3+2];

			pUsedModes = rnUsedModes+i;

			int posPixel=i*nM;
			int result = _backgroundModel(	posPixel, red, green, blue, &(values[i*3+0]), pUsedModes,rGMM,
													nM,fAlphaT, fTb, fTB, fTg, fSigma, fPrune);
			int nMLocal=*pUsedModes;
			
			if (bShadowDetection) {
					int resultShadow = _removeShadow(	posPixel, red, green, blue, &(values[i*3+1]), &(values[i*3+2]), nMLocal,rGMM, nM, fTb, fTB, fTg, fTau);
					if (!result)
					{
						result= resultShadow;
					}

			}
			switch (result)
			{
				case 0:
					//foreground
					output[i]=255;
					break;
				case 1:
					//background
					output[i]=0;
					break;
				case 2:
					//shadow
					output[i]=125;
					break;
			}
		}
    }
};


BOOST_PYTHON_MODULE(ZivkovicDescriptor)
{
    // Create the Python type object for our extension class and define __init__ function.
    class_<ZivkovicBackgroundModel>("ZivkovicBackgroundModel", init<float,float,float,float,float,float,int,bool,float,int,int,int,int,bool>())
        .def("backgroundModel", &ZivkovicBackgroundModel::backgroundModel)  // Add a regular member function.
    ;
}
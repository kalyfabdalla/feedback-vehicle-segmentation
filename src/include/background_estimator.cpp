extern "C"

#include "background_estimator.hpp"

BOOST_PYTHON_MODULE(BackgroundModel)
{
    // Create the Python type object for our extension class and define __init__ function.
    class_<PixelBackgroundModel>("PixelBackgroundModel",init<float,float,float,float,float,float,float,float,float,int,bool,float,int,int,int,int,bool>())
        .def("backgroundModel", &PixelBackgroundModel::backgroundModel)  // Add a regular member function.
        .def("wrapBackgroundModel", &PixelBackgroundModel::wrapBackgroundModel);
}

int _backgroundModel(	long posPixel, float red, float green, float blue,  float * distance, float dnorm, unsigned char* pModesUsed, PixelGMMZ* m_aGaussians,
								int m_nM, float m_fAlphaT, float m_fTb, float m_fTB, float m_fTg, float m_fSigma, float m_fMinSigma, float m_fMaxSigma, float m_fPrune) {


  long pos;
	bool bFitsPDF=0;
	bool bBackground=0;
	float m_fOneMinAlpha=1-m_fAlphaT;
	int nModes=*pModesUsed;
	float totalWeight=0.0f;
	float dstFinal = dnorm;
  ////std::cout << "entrou 4" << std::endl;
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

				m_aGaussians[pos].sigma =sigmanew< m_fMinSigma ? m_fMinSigma : sigmanew>m_fMaxSigma?m_fMaxSigma:sigmanew;

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

  ////std::cout << "saiu 4" << std::endl;
	//go through all modes
	//////

	//renormalize weights

  ////std::cout << "entrou 5" << std::endl;
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

  ////std::cout << "saiu 5" << std::endl;

	//set the number of modes

	*pModesUsed=nModes;

  ////std::cout << "entrou 6" << std::endl;
	*distance = (dstFinal/dnorm);
  ////std::cout << "saiu 6" << std::endl;
	return bBackground;
}

int _removeShadow (	long posPixel, float red, float green, float blue, float * chroma, float * lumina, float dnorm, unsigned char nModes, PixelGMMZ* m_aGaussians,
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
			*chroma = ((dist/(var*a*a))/dnorm);
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

PixelBackgroundModel::PixelBackgroundModel()
{

}

PixelBackgroundModel::PixelBackgroundModel(	float fAlphaT, float fTb, float fTg, float fTB, float fSigma, float fMinSigma, float fMaxSigma, float fCT, float fDnorm, int nM,
						bool bShadowDetection, float fTau, int nNBands, int nWidth, int nHeight, int nSize,
						bool bRemoveForeground  ) :
						fAlphaT(fAlphaT), fTb(fTb), fTg(fTg), fTB(fTB), fSigma(fSigma), fMinSigma(fMinSigma), fMaxSigma(fMaxSigma), fCT(fCT), fDnorm(fDnorm), nM(nM),
						bShadowDetection(bShadowDetection), fTau(fTau), nNBands(nNBands), nWidth(nWidth), nHeight(nHeight),
						nSize(nSize), bRemoveForeground(bRemoveForeground) {
							rGMM=(PixelGMMZ*) malloc(nSize * nM * sizeof(PixelGMMZ));
							rnUsedModes = (unsigned char* ) malloc(nSize);
							memset(rnUsedModes,0,nSize);//no modes used
              roi_x = 0;
              roi_y = 0;
              roi_width = nWidth;
              roi_height = nHeight;

              Py_Initialize();
			  np::initialize();
			}

void PixelBackgroundModel::setRoi(int x, int y, int width, int height) {
	roi_x = x;
	roi_y = y;
	roi_width = width;
	roi_height = height;
}



void PixelBackgroundModel::backgroundModel(void* data, void* output, void* values) {
	this->data = (unsigned char*)data;
	this->output = (unsigned char*)output;
	this->values = (float*)values;
  	for (line=roi_y;line<roi_y+roi_height;line++) {
		cv::parallel_for_(cv::Range(roi_x,roi_x+roi_width), *this);
	}
}

void PixelBackgroundModel::wrapBackgroundModel(np::ndarray & data, np::ndarray & output, np::ndarray & values) {
	//std::cout << "entrou no wrap" << std::endl;
	backgroundModel(reinterpret_cast<unsigned char*>(data.get_data()),reinterpret_cast<unsigned char*>(output.get_data()),reinterpret_cast<float *>(values.get_data()));
}

void PixelBackgroundModel::operator ()(const cv::Range& range) const
{
  float fPrune=-fAlphaT*fCT;
  unsigned char* pUsedModes;
  int i,idx;
  ////std::cout << "entrou loop" << std::endl;
  for (idx = range.start; idx < range.end; ++idx)
  {
    //std::cout << "1" << std::endl;
  	i = line*nWidth+idx;

  	float red = data[i*3+0];
    //std::cout << "111" << std::endl;
  	float green = data[i*3+1];
    //std::cout << "112" << std::endl;
  	float blue = data[i*3+2];
    //std::cout << "1132" << std::endl;
  	pUsedModes = rnUsedModes+i;

  	int posPixel=i*nM;
    //std::cout << "11" << std::endl;
  	int result = _backgroundModel(	posPixel, red, green, blue, &(values[i*3+0]), fDnorm, pUsedModes,rGMM,
  											nM,fAlphaT, fTb, fTB, fTg, fSigma, fMinSigma, fMaxSigma, fPrune);

    //std::cout << "12" << std::endl;
  	int nMLocal=*pUsedModes;
    ////std::cout << "entrou 2" << std::endl;
  	if (bShadowDetection) {
  			int resultShadow = _removeShadow(	posPixel, red, green, blue, &(values[i*3+1]), &(values[i*3+2]), fDnorm, nMLocal,rGMM, nM, fTb, fTB, fTg, fTau);
  			if (result && resultShadow)
  			{
  				result= resultShadow;
  			}

  	}
    //std::cout << "13" << std::endl;
    ////std::cout << "saiu 2" << std::endl;
    ////std::cout << "entrou 3" << std::endl;
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
    //std::cout << "2" << std::endl;
    ////std::cout << "saiu 3" << std::endl;
  }
  ////std::cout << "saiu loop" << std::endl;
}

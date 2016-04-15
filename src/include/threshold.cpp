#include "threshold.hpp"

double hist[256*256];
double prob[256*256];
double acc[256*256];

int getKittlerThreshold(Mat image)
{
	int threshold, hist[256];
	double prob[256], acc[256], mu0[256], mu1[256], std0[256], std1[256], sum_gl = 0, sum_lG = 0;
	double thresh = DBL_MAX, th;
	int iSize = image.rows*image.cols;

	for(int i=0;i<256;i++)
		hist[i] = 0;

	//calc histogram
	for(int i=0;i<iSize;i++) {
		hist[image.data[i]] = hist[image.data[i]] + 1;
	}

	//normalize
	prob[0] = hist[0]*(1./iSize);
	acc[0] = prob[0];
	for(int i=1;i<256;i++) {
		prob[i] = hist[i]*(1./iSize);
		acc[i] = prob[i] + acc[i-1];
	}

	//cakc threshold
	for(int l=0;l<255;l++)
	{
		sum_gl = 0;
		sum_lG = 0;
		for(int j=0;j<=l;j++)
		{
			sum_gl += ((j+1)*prob[j]);
		}

		for(int k=l+1;k<256;k++)
		{
			sum_lG += ((k+1)*prob[k]);
		}

		mu0[l] = sum_gl/acc[l];
		mu1[l] = sum_lG/(1-acc[l]);
	}

	for(int l=0;l<255;l++)
	{
		sum_gl = 0;
		sum_lG = 0;
		double err;

		for(int j=0;j<=l;j++)
		{
			err = (j+1)-mu0[l];
			err = err*err;
			sum_gl += err*prob[j];
		}

		for(int k=l+1;k<256;k++)
		{
			err = (k+1)-mu1[l];
			err = err*err;
			sum_lG += err*prob[k];
		}

		std0[l] = sqrt(sum_gl/acc[l]);
		std1[l] = sqrt(sum_lG/(1-acc[l]));
	}


	for(int l=0;l<255;l++)
	{
		double aux = 0;
		th = 1;

		if (std0[l]>1E-3) {
			aux = 2*acc[l]*log(std0[l]);
			th += aux;
		}
		if (std1[l]>1E-3) {
			aux = 2*(1-acc[l])*log(std1[l]);
			th += aux;
		}
		if (acc[l]>1E-3) {
			aux = -2*acc[l]*log2(acc[l]);
			th += aux;
		}
		if ((1-acc[l])>1E-3) {
			aux = -2*(1-acc[l])*log2(1-acc[l]);
			th += aux;
		}


		if (th < thresh)
		{
			thresh = th;
			threshold = l;
		}

	}

	return threshold;
}

int getOtsuThreshold(Mat image)
{
	int threshold, hist[256];
	double prob[256], acc[256], mu0[256], mu1[256], std0[256], std1[256], sum_gl = 0, sum_lG = 0;
	double thresh = DBL_MAX, th;
	int iSize = image.rows*image.cols;

	for(int i=0;i<256;i++)
		hist[i] = 0;

	//calc histogram
	for(int i=0;i<iSize;i++) {
		hist[image.data[i]] = hist[image.data[i]] + 1;
	}

	//normalize
	prob[0] = hist[0]*(1./iSize);
	acc[0] = prob[0];
	for(int i=1;i<256;i++) {
		prob[i] = hist[i]*(1./iSize);
		acc[i] = prob[i] + acc[i-1];
	}

	//cakc threshold
	for(int l=0;l<256;l++)
	{
		sum_gl = 0;
		sum_lG = 0;
		for(int j=0;j<=l;j++)
		{
			sum_gl += ((j+1)*prob[j]);
		}

		for(int k=l+1;k<256;k++)
		{
			sum_lG += ((k+1)*prob[k]);
		}

		mu0[l] = sum_gl/acc[l];
		mu1[l] = sum_lG/(1-acc[l]);
	}

	for(int l=0;l<256;l++)
	{
		sum_gl = 0;
		sum_lG = 0;
		double err;
		for(int j=0;j<=l;j++)
		{
			err = (j+1)-mu0[l];
			err = err*err;
			sum_gl += err*prob[j];
		}

		for(int k=l+1;k<256;k++)
		{
			err = (k+1)-mu1[l];
			err = err*err;
			sum_lG += err*prob[k];
		}

		std0[l] = sqrt(sum_gl/acc[l]);
		std1[l] = sqrt(sum_lG/(1-acc[l]));
	}


	for(int l=0;l<256;l++)
	{
		th = 1;
		if (std0[l]!=0)
			th += 2*acc[l]*log(std0[l]);
		if (std1[l]!=0)
			th += 2*(1-acc[l])*log(std1[l]);
		if (acc[l]!=0)
			th += -2*acc[l]*log(acc[l]);
		if ((1-acc[l])!=0)
			th += -2*(1-acc[l])*log(1-acc[l]);


		if (th < thresh)
		{
			thresh = th;
			threshold = l;
		}

	}

	return threshold;
}

int getKapurThreshold(Mat image)
{
	int threshold, hist[256];
	double prob[256], acc[256], best_entropy, sum_gl = 0, sum_lG = 0;
	int iSize = image.rows*image.cols;

	for(int i=0;i<256;i++)
		hist[i] = 0;

	//calc histogram
	for(int i=0;i<iSize;i++) {
		hist[image.data[i]] = hist[image.data[i]] + 1;
	}

	//normalize
	prob[0] = hist[0]*(1./iSize);
	acc[0] = prob[0];
	for(int i=1;i<256;i++) {
		prob[i] = hist[i]*(1./iSize);
		acc[i] = prob[i] + acc[i-1];
	}

	//cakc threshold
	for(int l=0;l<255;l++)
	{
		double gpt,gpot,entropy;
		sum_gl = 0;
		sum_lG = 0;
		for(int j=0;j<=l;j++)
		{
			gpt = prob[j]/acc[l];
			if (gpt>1e-5)
				sum_gl += gpt*log2(gpt);
		}

		for(int k=l+1;k<256;k++)
		{
			gpot = prob[k]/(1-acc[l]);
			if (gpot>1e-5)
				sum_lG += gpot*log2(gpot);
		}
		entropy = -sum_gl-sum_lG;
		if (entropy > best_entropy)
		{
			best_entropy = entropy;
			threshold = l;
		}
	}

	return threshold;
}

int getYenThreshold(Mat image)
{
	int threshold, hist[256];
	double prob[256], acc[256], best_entropy, sum_gl = 0, sum_lG = 0;
	int iSize = image.rows*image.cols;

	for(int i=0;i<256;i++)
		hist[i] = 0;

	//calc histogram
	for(int i=0;i<iSize;i++) {
		hist[image.data[i]] = hist[image.data[i]] + 1;
	}

	//normalize
	prob[0] = hist[0]*(1./iSize);
	acc[0] = prob[0];
	for(int i=1;i<256;i++) {
		prob[i] = hist[i]*(1./iSize);
		acc[i] = prob[i] + acc[i-1];
	}

	//cakc threshold
	for(int l=0;l<255;l++)
	{
		double gpt,gpot,entropy;
		sum_gl = 0;
		sum_lG = 0;
		for(int j=0;j<=l;j++)
		{
			gpt = prob[j]/acc[l];
			sum_gl += (gpt*gpt);
		}

		for(int k=l+1;k<256;k++)
		{
			gpot = prob[k]/(1-acc[l]);
			sum_lG += (gpot*gpot);
		}
		entropy = 0;
		if(sum_gl>1e-5 && sum_lG>1e-5)
			entropy = -log2(sum_gl)-log2(sum_lG);
		if (entropy > best_entropy)
		{
			best_entropy = entropy;
			threshold = l;
		}
	}

	return threshold;
}


int getSpatialPal1Threshold(Mat image)
{
	int histNorm = 0;
	int thr = 0;
	double best_entropy = 0;



	for(int i=0;i<256;i++) {
		for(int j=0;j<256;j++) {
			hist[i*256+j] = 0;
		}
	}

	for(int i=0;i<image.rows-1;i++) {
		for(int j=0;j<image.cols-1;j++) {
			hist[image.data[i*image.cols+j]*256+image.data[i*image.cols+(j+1)]] = hist[image.data[i*image.cols+j]*256+image.data[i*image.cols+(j+1)]] + 1;
			hist[image.data[i*image.cols+j]*256+image.data[(i+1)*image.cols+j]] = hist[image.data[i*image.cols+j]*256+image.data[(i+1)*image.cols+j]] + 1;
			histNorm = histNorm+2;
		}
	}

	for(int i=0;i<256;i++) {
		for(int j=0;j<256;j++) {
			prob[i*256+j] = hist[i*256+j]/(double)histNorm;
			acc[i*256+j] = prob[i*256+j];
			if (i>0)
				acc[i*256+j] = acc[i*256+j]+acc[(i-1)*256+j];
			if (j>0)
				acc[i*256+j] = acc[i*256+j]+acc[i*256+(j-1)];
			if (i>0 && j>0)
				acc[i*256+j] = acc[i*256+j]-acc[(i-1)*256+(j-1)];
		}
	}




	for(int l=0;l<255;l++) {
		double Pa = acc[l*256+l];
		double Pb = acc[l*256+255]-acc[l*256+l];
		double Pd = acc[255*256+l]-acc[l*256+l];
		double Pc = acc[255*256+255]-Pa-Pb-Pd;
		double gpt,gpot,entropy,val;
		gpt=0.0;
		gpot=0.0;
		for(int i=0;i<=l;i++) {
			for(int j=0;j<=l;j++) {
				val = prob[i*256+j]/Pa;
				if (val>1e-5)
					gpt += val*log(val);
			}
		}

		for(int i=l+1;i<256;i++) {
			for(int j=l+1;j<256;j++) {
				val = prob[i*256+j]/Pc;
				if (val>1e-5)
					gpot += val*log(val);
			}
		}

		entropy = -gpt/2.0 - gpot/2.0;

		if (entropy>best_entropy) {
			best_entropy = entropy;
			thr = l;
		}

	}
//*/
	return thr;

}

int getSpatialPal2Threshold(Mat image)
{
	int histNorm = 0;
	int thr = 0;
	double best_entropy = 0;



	for(int i=0;i<256;i++) {
		for(int j=0;j<256;j++) {
			hist[i*256+j] = 0;
		}
	}

	for(int i=0;i<image.rows-1;i++) {
		for(int j=0;j<image.cols-1;j++) {
			hist[image.data[i*image.cols+j]*256+image.data[i*image.cols+(j+1)]] = hist[image.data[i*image.cols+j]*256+image.data[i*image.cols+(j+1)]] + 1;
			hist[image.data[i*image.cols+j]*256+image.data[(i+1)*image.cols+j]] = hist[image.data[i*image.cols+j]*256+image.data[(i+1)*image.cols+j]] + 1;
			histNorm = histNorm+2;
		}
	}

	for(int i=0;i<256;i++) {
		for(int j=0;j<256;j++) {
			prob[i*256+j] = hist[i*256+j]/(double)histNorm;
			acc[i*256+j] = prob[i*256+j];
			if (i>0)
				acc[i*256+j] = acc[i*256+j]+acc[(i-1)*256+j];
			if (j>0)
				acc[i*256+j] = acc[i*256+j]+acc[i*256+(j-1)];
			if (i>0 && j>0)
				acc[i*256+j] = acc[i*256+j]-acc[(i-1)*256+(j-1)];
		}
	}




	for(int l=0;l<255;l++) {
		//double Pa = acc[l*256+l];
		double Pb = acc[l*256+255]-acc[l*256+l];
		double Pd = acc[255*256+l]-acc[l*256+l];
		//double Pc = acc[255*256+255]-Pa-Pb-Pd;
		double gpt,gpot,entropy,val;
		gpt=0.0;
		gpot=0.0;
		for(int i=l+1;i<256;i++) {
			for(int j=0;j<=l;j++) {
				val = prob[i*256+j]/Pd;
				if (val>1e-5)
					gpt += val*log(val);
			}
		}

		for(int i=0;i<=l;i++) {
			for(int j=l+1;j<256;j++) {
				val = prob[i*256+j]/Pb;
				if (val>1e-5)
					gpot += val*log(val);
			}
		}

		entropy = -gpt/2.0 - gpot/2.0;

		if (entropy>best_entropy) {
			best_entropy = entropy;
			thr = l;
		}

	}
//*/
	return thr;

}

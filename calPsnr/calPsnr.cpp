#include "YUV.h"


using namespace std;

int main(int argc, char* argv[])
{
	int frame = atoi(argv[1]);
	string maskfileName = argv[2];
	string fileName1 = argv[3];
	string fileName2 = argv[4];
	uint16_t width = atoi(argv[5]);
	uint16_t height = atoi(argv[6]);
	uint8_t textureBitdepth = atoi(argv[7]);
	uint8_t maskfileBitdepth = atoi(argv[8]);
	string strOfFileFormat = argv[9];
	string outputPath = argv[10];

	YUVformat fileFormat = YUV420;
	if (strOfFileFormat == "YUV420") fileFormat = YUV420;
	else if (strOfFileFormat == "YUV444") fileFormat = YUV444;
	else {
		std::cout << "The fileformat:" << strOfFileFormat << "is not right. Use\"YUV420\" or \"YUV444\"" << endl;;
		return -1;
	}
	YUV mask(width, height, maskfileBitdepth, fileFormat);
	YUV yuv1(width, height, textureBitdepth, fileFormat);
	YUV yuv2(width, height, textureBitdepth, fileFormat);
	
	double sum_psnr = 0;

	

	ofstream fout(outputPath,ios::app);
	fout.setf(ios::fixed);
	fout.precision(4);
	for (int f = 0; f < frame; f++) {
		double sum_EY = 0;
		mask.YUVRead(maskfileName, f);
		yuv1.YUVRead(fileName1, f);
		yuv2.YUVRead(fileName2, f);
		int pixcelCount=0;
		for (int m = 0; m < height; m++)
		{
			for (int n = 0; n < width; n++)
			{
				if (mask[0][m][n] != 0) {
					sum_EY += pow((yuv1[0][m][n] - yuv2[0][m][n]), 2);
					pixcelCount++;
				}
				
			}
		}
		sum_EY /= pixcelCount;//mse
		//cout << pixcelCount << endl;
		double psnr = 10 * log((pow(2, textureBitdepth) - 1)* (pow(2, textureBitdepth) - 1) / sum_EY) / log(10);
		sum_psnr += psnr;
		fout << "frame" << f <<"\t mask_YPSNR:" <<psnr << endl;
	}
	double mean_psnr = sum_psnr/ frame;
	fout << fileName2<<"\tmask Y_PSNR:\t" << mean_psnr << endl;
	fout.close();
	cout << "[done]" << endl;
	return 0;
}

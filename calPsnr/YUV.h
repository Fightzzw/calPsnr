#ifndef YUV_h
#define YUV_h

#include <vector>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <math.h>
#include <assert.h>



enum YUVformat
{
	YUV444,
	YUV420
};

class YUV
{
private:
	std::vector< std::vector <uint16_t> >* Y;
	std::vector< std::vector <uint16_t> >* U;
	std::vector< std::vector <uint16_t> >* V;
	uint16_t W;
	uint16_t H;
	uint8_t bitDepth;
	YUVformat cs;
public:
	std::vector< std::vector <uint16_t> >& operator[](size_t i)
	{
		assert(i <= 2);
		switch (i)
		{
		case 0: return *Y;
		case 1: return *U;
		case 2: return *V;
		}
	}
	YUV()
	{
		W = 0; H = 0; bitDepth = 0;
		Y = NULL; U = NULL; V = NULL;
	}
	YUV(uint16_t pW, uint16_t pH, uint8_t pbitDepth, YUVformat pcs) :W(pW), H(pH), bitDepth(pbitDepth), cs(pcs)
	{
		assert(bitDepth == 16 || bitDepth == 10);
		uint16_t emptyVal = 1 << (bitDepth - 1);
		Y = new std::vector< std::vector <uint16_t> >(H, std::vector <uint16_t>(W, emptyVal));
		U = new std::vector< std::vector <uint16_t> >(H, std::vector <uint16_t>(W, emptyVal));
		V = new std::vector< std::vector <uint16_t> >(H, std::vector <uint16_t>(W, emptyVal));
	}
	~YUV()
	{
		if (Y) delete Y;
		if (U) delete U;
		if (V) delete V;
	}
	void YUVRead(std::string fileName, size_t frameCounter)
	{
		assert(Y && U && V);
		if (cs == YUV420)
		{
			if (bitDepth == 8) {
				uint8_t* FileBuffer = (uint8_t*)malloc(H * W * sizeof(uint8_t) * 3 / 2);
				long long offset = H * W * 3 / 2 * frameCounter;

				FILE* Fp = fopen(fileName.c_str(), "rb");
				_fseeki64(Fp, offset, SEEK_SET);
				//if (fread(FileBuffer, W * H * 3, 1, Fp) != 1)
				if (fread(FileBuffer, W * H * 3 / 2, 1, Fp) != 1)
				{
					printf("can't open the YUV File %s \n", fileName.c_str());
					exit(1);
				}
				fclose(Fp);

				for (int j = 0; j < H; j++)
				{
					for (int i = 0; i < W; i++)
					{
						int address_y = j * W + i;
						int address_u = H * W + (int)(j / 2) * (W / 2) + (int)(i / 2);
						int address_v = H * W * 5 / 4 + (int)(j / 2) * (W / 2) + (int)(i / 2);

						(*Y)[j][i] = FileBuffer[address_y];
						(*U)[j][i] = FileBuffer[address_u];
						(*V)[j][i] = FileBuffer[address_v];

					}
				}
				free(FileBuffer);
				return;
			}
			else {
				uint16_t* FileBuffer = (uint16_t*)malloc(H * W * sizeof(uint16_t) * 3 / 2);
				long long offset = H * W * 3 / 2 * sizeof(uint16_t) * frameCounter;

				FILE* Fp = fopen(fileName.c_str(), "rb");
				_fseeki64(Fp, offset, SEEK_SET);
				//if (fread(FileBuffer, W * H * 3, 1, Fp) != 1)
				if (fread(FileBuffer, W * H * sizeof(uint16_t) * 3 / 2, 1, Fp) != 1)
				{
					printf("can't open the YUV File %s \n", fileName.c_str());
					exit(1);
				}
				fclose(Fp);

				for (int j = 0; j < H; j++)
				{
					for (int i = 0; i < W; i++)
					{
						int address_y = j * W + i;
						int address_u = H * W + (int)(j / 2) * (W / 2) + (int)(i / 2);
						int address_v = H * W * 5 / 4 + (int)(j / 2) * (W / 2) + (int)(i / 2);

						(*Y)[j][i] = FileBuffer[address_y];
						(*U)[j][i] = FileBuffer[address_u];
						(*V)[j][i] = FileBuffer[address_v];

					}
				}
				free(FileBuffer);
				return;
			}


		}
		else
		{
			std::cout << "Haven't relized reading such format YUV File!" << std::endl;
			exit(1);
		}
	}
	void YUVWrite(std::string fileName)
	{
		assert(Y && U && V);
		FILE* Fp = fopen(fileName.c_str(), "ab");

		for (int i = 0; i < H; i++)
		{
			for (int j = 0; j < W; j++)
			{
				fwrite(&((*Y)[i][j]), sizeof(uint16_t), 1, Fp);
			}
		}
		for (int i = 0; i < H; i += 2)
		{
			for (int j = 0; j < W; j += 2)
			{
				fwrite(&((*U)[i][j]), sizeof(uint16_t), 1, Fp);
			}
		}
		for (int i = 0; i < H; i += 2)
		{
			for (int j = 0; j < W; j += 2)
			{
				fwrite(&((*V)[i][j]), sizeof(uint16_t), 1, Fp);
			}
		}
		fclose(Fp);

		return;

	}
	bool YUV444To420() { return true; }
	bool YUV420To444() { return true; }
	void SetY(uint16_t m, uint16_t n, uint16_t val) { assert(m < H&& n < W); (*Y)[m][n] = val; }
	void SetU(uint16_t m, uint16_t n, uint16_t val) { assert(m < H&& n < W); (*U)[m][n] = val; }
	void SetV(uint16_t m, uint16_t n, uint16_t val) { assert(m < H&& n < W); (*V)[m][n] = val; }
	uint16_t GetY(uint16_t m, uint16_t n) { return (*Y)[m][n]; }
	uint16_t GetU(uint16_t m, uint16_t n) { return (*U)[m][n]; }
	uint16_t GetV(uint16_t m, uint16_t n) { return (*V)[m][n]; }

}; 
#endif 

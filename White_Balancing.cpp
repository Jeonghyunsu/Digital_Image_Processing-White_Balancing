#include <iostream>

using namespace std;

#define HEIGHT 512
#define WIDTH 512

void MemFree2D(unsigned char **Mem, int nHeight);
unsigned char** MemAlloc2D(int nHeight, int nWidth, unsigned char nInitVal);
void WhiteBalancing(unsigned char **R, unsigned char **G, unsigned char **B, double* wR_ptr, double* wG_ptr, double* wB_ptr, int nHeight, int nWidth);
void RGB_input(unsigned char* RGB, unsigned char** R, unsigned char** G, unsigned char** B, int nHeight, int nWidth);
void RGB_output(unsigned char* RGB, unsigned char** R, unsigned char** G, unsigned char** B, double* wR_ptr, double* wG_ptr, double* wB_ptr, int nHeight, int nWidth);

int main()
{
	int nHeight = 512;
	int nWidth = 512;
	unsigned char* header = (unsigned char*)malloc(sizeof(unsigned char) * 54);
	unsigned char* RGB = (unsigned char*)malloc(sizeof(unsigned char)*(nHeight*nWidth * 3));

	FILE* input;
	fopen_s(&input, "test.bmp", "rb");
	fread(header, 54, sizeof(unsigned char), input);
	fread(RGB, nHeight*nWidth * 3, sizeof(unsigned char), input);
	fclose(input);

	unsigned char** R = MemAlloc2D(nHeight, nWidth, 0);
	unsigned char** G = MemAlloc2D(nHeight, nWidth, 0);
	unsigned char** B = MemAlloc2D(nHeight, nWidth, 0);

	double wR = 0; double wG = 0; double wB = 0;
	double* wR_ptr = &wR; double* wG_ptr = &wG; double* wB_ptr = &wB;
	
	RGB_input(RGB, R, G, B, nHeight, nWidth);
	WhiteBalancing(R, G, B, wR_ptr, wG_ptr, wB_ptr, nHeight, nWidth);
	RGB_output(RGB, R, G, B, wR_ptr, wG_ptr, wB_ptr, nHeight, nWidth);
	
	FILE* Output;
	fopen_s(&Output, "result.raw", "wb");
	fwrite(RGB, nHeight*nWidth * 3, sizeof(unsigned char), Output);
	fclose(Output);
	
	return 0;
}

void MemFree2D(unsigned char **Mem, int nHeight)
{
	for (int n = 0; n < nHeight; n++)
	{
		delete[] Mem[n];
	}
	delete[] Mem;
}

unsigned char** MemAlloc2D(int nHeight, int nWidth, unsigned char nInitVal)
{
	unsigned char** rtn = new unsigned char*[nHeight];
	for (int n = 0; n < nHeight; n++)
	{
		rtn[n] = new unsigned char[nWidth];
		memset(rtn[n], nInitVal, sizeof(unsigned char) * nWidth);
	}
	return rtn;
}

void WhiteBalancing(unsigned char **R, unsigned char **G, unsigned char **B, double* wR_ptr, double* wG_ptr, double* wB_ptr, int nHeight, int nWidth)
{
	double r = 0, g = 0, b = 0;

	int temp_sum = 0, result_sum = 0;

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			temp_sum = R[h][w] + G[h][w] + B[h][w];
			if (temp_sum > result_sum)
			{
				result_sum = temp_sum;

				r = R[h][w];
				g = G[h][w];
				b = B[h][w];
			}
		}
	}

	*wR_ptr = (double)255 / r;
	*wG_ptr = (double)255 / g;
	*wB_ptr = (double)255 / b;
}

void RGB_input(unsigned char* RGB, unsigned char** R, unsigned char** G, unsigned char** B, int nHeight, int nWidth)
{
	int rgb_index = 0;
	int h = 0, w = 0;

	for (rgb_index = 0; rgb_index < nHeight*nWidth * 3; rgb_index++)
	{
		if (rgb_index % 3 == 0)
		{
			B[h][w++] = RGB[rgb_index];
			if (w % nWidth == 0)
			{
				w = 0;
				h++;
			}
		}
	}

	h = 0, w = 0;
	for (rgb_index = 0; rgb_index < nHeight*nWidth * 3; rgb_index++)
	{
		if (rgb_index % 3 == 1)
		{
			G[h][w++] = RGB[rgb_index];
			if (w % nWidth == 0)
			{
				w = 0;
				h++;
			}
		}
	}

	h = 0, w = 0;
	for (rgb_index = 0; rgb_index < nHeight*nWidth * 3; rgb_index++)
	{
		if (rgb_index % 3 == 2)
		{
			R[h][w++] = RGB[rgb_index];
			if (w % nWidth == 0)
			{
				w = 0;
				h++;
			}
		}
	}

}

void RGB_output(unsigned char* RGB, unsigned char** R, unsigned char** G, unsigned char** B, double* wR_ptr, double* wG_ptr, double* wB_ptr, int nHeight, int nWidth)
{
	int rgb_index;
	int	h = nHeight - 1, w = 0;

	for (rgb_index = 0; rgb_index < nHeight*nWidth * 3; rgb_index++)
	{
		if (rgb_index % 3 == 0)
		{
			RGB[rgb_index] = R[h][w++]*(*wR_ptr);
			if (w == nWidth)
			{
				w = 0;
				h--;
			}
		}
	}

	h = nHeight-1, w = 0;
	for (rgb_index = 0; rgb_index < nHeight*nWidth * 3; rgb_index++)
	{
		if (rgb_index % 3 == 1)
		{
			RGB[rgb_index] = G[h][w++] * (*wG_ptr);
			if (w == nWidth)
			{
				w = 0;
				h--;
			}
		}
	}
	h = nHeight-1, w = 0;
	for (rgb_index = 0; rgb_index < nHeight*nWidth * 3; rgb_index++)
	{
		if (rgb_index % 3 == 2)
		{
			RGB[rgb_index] = B[h][w++] * (*wB_ptr);
			if (w == nWidth)
			{
				w = 0;
				h--;
			}
		}
	}
}

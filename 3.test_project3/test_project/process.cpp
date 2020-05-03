#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

//完成代码基础自测题3
//要求：在提供的工程的process.cpp里自行设计函数接口，实现：
//将图像图片分成像素8 * 8（数值可调，图片不被8整除就舍弃最下最右边缘部分）的小块，
//计算【每一块最黑的5%到10%灰度均值（数值可调）】和【最白的5%到10%灰度均值（数值可调）】的差，
//最后得出最黑最白差大于50（可调）的小块占总小块的百分比。做到代码规范
//工程路径：\\192.168.1.45\file - input\常用软件 + 培训资料\【培训文档】\测试题目\3.test_project3.rar
#define PARAM_ERROR  -1		//入参错误
#define MALLOC_ERROR -2     //空间申请失败
#define OTHER_ERROR  -3     //其他错误
#define PERCENT 100

int GetThreshold(unsigned int *histogram, int imageArea, int min, int max, int imageSum)
{
	int scale;
	int sum = 0;
	int count = 0;
	int minSum = 0;
	int maxSum = 0;

	min = (min * imageArea + PERCENT/2) / PERCENT;//四舍五入
	max = (max * imageArea + PERCENT/2) / PERCENT;

	for (scale = 255; scale >= 0; scale--)
	{
		if (histogram[scale] == 0)
		{
			continue;
		}
		if ((count + histogram[scale]) > min)
		{
			minSum += (min - count) * scale;
			break;
		}
		else
		{
			minSum += histogram[scale] * scale;
			count += histogram[scale];
		}
	}
	count = 0;

	for (scale = 255; scale >= 0; scale--)
	{
		if (histogram[scale] == 0)
		{
			continue;
		}
		if ((count + histogram[scale]) > max)
		{
			maxSum += (max - count) * scale;
			break;
		}
		else
		{
			maxSum += histogram[scale] * scale;
			count += histogram[scale];
		}
	}

	return ((maxSum - minSum) / (max - min));
}


int Histogram(unsigned char *image, unsigned int * histogram, int imgSize){
	int i;
	int blockSum = 0;
	memset(histogram, 0, sizeof(unsigned int)* 256);
	if (histogram == NULL)
	{
		printf("--- MALLOC_ERROR! --- \nFile : %s,\nFunction :  %s,\nLine[%d],\nTimestemp :  %s\n", __FILE__, __FUNCTION__, __LINE__, __TIMESTAMP__);
		return MALLOC_ERROR;
	}
	for (i = 0; i < imgSize; i++)
	{
		histogram[image[i]]++;
		blockSum += image[i];
	}
	return blockSum;
}


int GetDvalueimage(
	unsigned char * image,
	int width,
	int height,
	image_parameter &imagepar,
	int * percent)
{
	int i, j, bi;// 图像行列，块行列 
	int dcount = 0;//dvalue计数
	int blackMean = 0;//黑域均值
	int whiteMean = 0;//白域均值

	int imageArea = width * height;//图片面积
	int iblockwidth = imagepar.blockwidth;
	int iblockheight = imagepar.blockheight;
	int iblackmax = imagepar.blackmax;
	int iblackmin = imagepar.blackmin;
	int iwhitemax = imagepar.whitemax;
	int iwhitemin = imagepar.whitemin;
	if (iblockwidth == 0 || iblockheight == 0 || width == 0 || height == 0 || width < iblockwidth || height < iblockheight || iblackmin == iblackmax || iwhitemax == iwhitemin)
	{
		printf("--- PARAM_ERROR! --- \nFile : %s,\nFunction :  %s,\nLine[%d],\nTimestemp :  %s\n", __FILE__, __FUNCTION__, __LINE__, __TIMESTAMP__);
		return PARAM_ERROR;
	}

	int widthIndex = width / iblockwidth;//每行block个数
	int heightIndex = height / iblockheight;//每列block个数
	int blockIndex = widthIndex * heightIndex;//总block个数
	int blockArea = iblockheight * iblockwidth;//block面积

	unsigned char *blockImage = (unsigned char *)malloc(sizeof(unsigned char) * imageArea);//块图像临时存储
	unsigned int *imgHistogram =(unsigned int *)malloc(sizeof(unsigned int) * 256);
	if (blockImage == NULL || imgHistogram == NULL)
	{
		printf("--- MALLOC_ERROR! --- \nFile : %s,\nFunction :  %s,\nLine[%d],\nTimestemp :  %s\n", __FILE__, __FUNCTION__,__LINE__,__TIMESTAMP__);
		return MALLOC_ERROR;
	}

	for (i = 0; i < heightIndex; i++){
		for (j = 0; j < widthIndex; j++){
			memset(blockImage, 0, sizeof(unsigned char)*imageArea);
			for (bi = 0; bi < iblockheight * iblockwidth; bi++){
				blockImage[bi] = image[i * widthIndex * blockArea + j * iblockwidth + bi / iblockwidth * width + bi % iblockwidth];
			}

			int blockSum = Histogram(blockImage, imgHistogram,blockArea);//直方图写入imgHistogram
			whiteMean = GetThreshold(imgHistogram, blockArea, iblackmin, iblackmax, blockSum);//白域均值
			blackMean = GetThreshold(imgHistogram, blockArea, PERCENT - iwhitemax, PERCENT - iwhitemin,blockSum);//黑域

			if (blackMean < 0 || whiteMean < 0)
			{
				printf("--- OTHER_ERROR! --- \nFile : %s,\nFunction :  %s,\nLine[%d],\nTimestemp :  %s\n", __FILE__, __FUNCTION__, __LINE__, __TIMESTAMP__);
				return OTHER_ERROR;
			}
			if ((whiteMean - blackMean) >= imagepar.dvalue)
			{
				dcount++;
			}
		}
	}
		*percent = dcount * PERCENT / blockIndex;
		free(blockImage);
		free(imgHistogram);
		return 0;
} 


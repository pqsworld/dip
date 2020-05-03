#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

//��ɴ�������Բ���3
//Ҫ�����ṩ�Ĺ��̵�process.cpp��������ƺ����ӿڣ�ʵ�֣�
//��ͼ��ͼƬ�ֳ�����8 * 8����ֵ�ɵ���ͼƬ����8�����������������ұ�Ե���֣���С�飬
//���㡾ÿһ����ڵ�5%��10%�ҶȾ�ֵ����ֵ�ɵ������͡���׵�5%��10%�ҶȾ�ֵ����ֵ�ɵ������Ĳ
//���ó������ײ����50���ɵ�����С��ռ��С��İٷֱȡ���������淶
//����·����\\192.168.1.45\file - input\������� + ��ѵ����\����ѵ�ĵ���\������Ŀ\3.test_project3.rar
#define PARAM_ERROR  -1		//��δ���
#define MALLOC_ERROR -2     //�ռ�����ʧ��
#define OTHER_ERROR  -3     //��������
#define PERCENT 100

int GetThreshold(unsigned int *histogram, int imageArea, int min, int max, int imageSum)
{
	int scale;
	int sum = 0;
	int count = 0;
	int minSum = 0;
	int maxSum = 0;

	min = (min * imageArea + PERCENT/2) / PERCENT;//��������
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
	int i, j, bi;// ͼ�����У������� 
	int dcount = 0;//dvalue����
	int blackMean = 0;//�����ֵ
	int whiteMean = 0;//�����ֵ

	int imageArea = width * height;//ͼƬ���
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

	int widthIndex = width / iblockwidth;//ÿ��block����
	int heightIndex = height / iblockheight;//ÿ��block����
	int blockIndex = widthIndex * heightIndex;//��block����
	int blockArea = iblockheight * iblockwidth;//block���

	unsigned char *blockImage = (unsigned char *)malloc(sizeof(unsigned char) * imageArea);//��ͼ����ʱ�洢
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

			int blockSum = Histogram(blockImage, imgHistogram,blockArea);//ֱ��ͼд��imgHistogram
			whiteMean = GetThreshold(imgHistogram, blockArea, iblackmin, iblackmax, blockSum);//�����ֵ
			blackMean = GetThreshold(imgHistogram, blockArea, PERCENT - iwhitemax, PERCENT - iwhitemin,blockSum);//����

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


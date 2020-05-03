#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

#define PARAM_ERROR -1		//入参错误
#define MALLOC_ERROR -2    //空间申请失败
int ImageCut(unsigned char * image,
	unsigned int width,
	unsigned int height,
	unsigned int left_cut,
	unsigned int right_cut,
	unsigned int up_cut,
	unsigned int down_cut)
{
	if (image == NULL || width == 0 || height == 0 || width <= (left_cut	+ right_cut) || height <= (up_cut + down_cut)) //入参判断必须在最前面
	{
		printf("PARAM_ERROR\n"); //错误return前加log
		return PARAM_ERROR;
	}
	int width_cut = width - left_cut - right_cut;//由于存在此行代码，前面入参必须判断width 和 （left_cut + right_cut）的大小
	int height_cut = height - up_cut - down_cut;
	unsigned char * image_tmp = (unsigned char *)malloc(sizeof(unsigned char)* width_cut * height_cut); //空间申请尽量做到用多少申请多少
	if (image_tmp == NULL) // 申请空间需要判断是否申请成功
	{
		//SL_LOGE("MALLOC_ERROR");
		return MALLOC_ERROR;
	}
	memset(image_tmp, 0, sizeof(unsigned char)* width_cut * height_cut); //申请空间后初始化
	int count = 0;
	for (unsigned int i = up_cut; i < height_cut + up_cut; i++)
	{
		for (unsigned int j = left_cut; j < width_cut + left_cut; j++)
		{
			image_tmp[count++] = image[i * width + j];
		}
	}
	memset(image, 0, sizeof(unsigned char)* width * height);
	memcpy(image, image_tmp, sizeof(unsigned char)* width_cut * height_cut);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "imageOperation.h"

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
	/****************************************
	在此实现图像切边功能

	image存的图像数据
	width 图像宽
	height 图像高
	
	********************************************/

	if (height <= 0 || width <= 0 || left_cut < 0 || right_cut < 0 || up_cut < 0 || down_cut < 0
		|| (height - up_cut - down_cut) <= 0 || (width - left_cut - right_cut) <= 0 || image == NULL)
	{
		printf("Parameter input is invalid.");
		return PARAM_ERROR;
	}


	//int k = 0;
	//存放转移
	unsigned char * image_buf = (unsigned char *)malloc(sizeof(unsigned char)* width * height );
	if (image_buf == NULL)
	{
		printf("Imagebuf malloc failed.");
		///*	throw "Image input failed.";*/
		return MALLOC_ERROR;
	}
	unsigned int cutHeight = height - up_cut - down_cut;
	unsigned int cutWidth = width - left_cut - right_cut;

	unsigned int i = 0;
	unsigned int j = 0;
	for (i = 0; i < (cutHeight); i++){
		for (j = 0; j < (cutWidth); j++){
			//*(image_path + i *lineByte2 + j * 3 + k) = *(image + i * bmpWidth * 3 + j * 3 + k + Rleftdownx * 3);
		//	*(image_buf + i + j ) = *(image + i * width  + j + left_cut );//地址代换

			*(image_buf + i*cutWidth + j ) = *(image + (i+up_cut) * width  + j + left_cut );//地址代换

				//printf("image[%d][%d] cut done.%d th\n", i, j,k++);
		}
	}

	//memset(image, 0, sizeof(unsigned char)*width*height);
	memcpy(image, image_buf, cutHeight*cutWidth);
	
	free(image_buf);

	return 0;
}

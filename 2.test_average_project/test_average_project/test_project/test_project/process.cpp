#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include <cmath>
//完成代码基础自测题2
//计算图像行列平均值
//要求：在提供的工程的process.cpp里按照给定的函数接口实现计算图像行列平均灰度功能，做到代码规范
//工程路径：\\192.168.1.45\file - input\常用软件 + 培训资料\【培训文档】\测试题目\2.test_average_project.rar
#define PARAM_ERROR -1		//入参错误
#define MALLOC_ERROR -2    //空间申请失败

int GetRowAvearge(unsigned char * image, unsigned char * row_average, int width, int height)
{
	/************************
	在此实现求图像每行灰度平均，结果存入row_average
	结果请四舍五入
	*****************************/
	//for (int i = 0; i < width * height; i++)
	//{
	//	printf("image[%d] : %d\n", i, image[i]);
	//}
	if (image == 0 || row_average == 0 || width <= 0 || height <= 0)
	{
		printf("Parameter Input Invalid\n");
		return PARAM_ERROR;
	}

	//double * round_buf = (double *)malloc(sizeof(double)* MAX_HEIGHT);//四舍五入缓冲器
	int * average_buf = ( int * )malloc(sizeof(int) * (MAX_HEIGHT + MAX_WIDTH));//平均数缓冲器
	//char * image_cpy = ( char * )malloc(sizeof(char) * MAX_HEIGHT * MAX_WIDTH);//image转换器
	//char * row_cpy = (char *)malloc(sizeof(char)* MAX_HEIGHT );//image转换器

	//memcpy(image_cpy, image, width * height); //image to image_cpy
	memset(average_buf, 0, (MAX_HEIGHT + MAX_WIDTH));
	//memset(round_buf, 0, MAX_HEIGHT);

	//for (int i = 0; i < MAX_HEIGHT; i++)
	//{
	//	printf(" imnage[%d]: %d\n", i, image[i]);
	//	printf(" imagecpy[%d]: %d\n", i, image_cpy[i]);

	//}
	int i, j;//C98的支持
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			//average_buf[i] += image[i * width + j];
			//average_buf[i] += atoi((const char*)image_cpy[i * width + j]);
			average_buf[i] += (int)image[i * width + j];

			//printf("rage[%d]: %d\n\n", i * width + j, average_buf[i * width + j]);
			//printf("i : %d j : %d  row_average[%d]: %d\n\n", i,j,i+j,average_buf[i]);
		}
		//printf("row_average[%d]: %d\n\n", i, row_average[i]);
		//memcpy(row_cpy, row_average, MAX_HEIGHT);
		//round_buf[i] = (double)(average_buf[i])/ (double)width;
		//printf(" averbuf[%d]: %d       round_buf[%d] :  %d\n\n", i, average_buf[i], width, round_buf[i]);

		row_average[i] = (unsigned char)(int((double)(average_buf[i]) / (double)width + 0.5));
		//int ret = itoa((average_buf[i] / width), row_cpy[i],2);
		//printf(" averbuf[%d]: %d       row_average[%d] :  %d\n\n",i,average_buf[i], i, row_average[i]);

	}

	if (image == NULL )
	{
		printf("Malloc Failed\n");
		return MALLOC_ERROR;
	}
	if (row_average == NULL)
	{
		printf("Row_average Malloc Failed\n");
		return MALLOC_ERROR;
	}

	free(average_buf);
	return 0;
}
int GetColAvearge(unsigned char * image, unsigned char * col_average, int width, int height)
{
	/************************
	在此实现求图像每列灰度平均，结果存入col_average
	结果请四舍五入
	*****************************/
	if (image == 0 || col_average == 0 || width <= 0 || height <= 0)
	{
		printf("Parameter Input Invalid\n");
		return PARAM_ERROR;
	}

	int * average_buf = (int *)malloc(sizeof(int)* (MAX_HEIGHT + MAX_WIDTH));//平均数缓冲器

	memset(average_buf, 0, (MAX_HEIGHT + MAX_WIDTH));
	int i, j;
	for (j = 0; j < width; j++)
	{
		for (i = 0; i < height; i++)
		{
			average_buf[j] += (int)image[i * width + j];
		}
		col_average[j] = (unsigned char)(int((average_buf[j]) / (double)height + 0.5));
		//printf(" averbuf[%d]: %d       row_average[%d] :  %d\n\n", i, average_buf[i], i, row_average[i]);
	}

	if (image == NULL)
	{
		printf("Malloc Failed\n");
		return MALLOC_ERROR;
	}
	if (col_average == NULL)
	{
		printf("Row_average Malloc Failed\n");
		return MALLOC_ERROR;
	}

	free(average_buf);
	return 0;
}
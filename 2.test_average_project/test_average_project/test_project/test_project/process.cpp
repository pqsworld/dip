#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include <cmath>
//��ɴ�������Բ���2
//����ͼ������ƽ��ֵ
//Ҫ�����ṩ�Ĺ��̵�process.cpp�ﰴ�ո����ĺ����ӿ�ʵ�ּ���ͼ������ƽ���Ҷȹ��ܣ���������淶
//����·����\\192.168.1.45\file - input\������� + ��ѵ����\����ѵ�ĵ���\������Ŀ\2.test_average_project.rar
#define PARAM_ERROR -1		//��δ���
#define MALLOC_ERROR -2    //�ռ�����ʧ��

int GetRowAvearge(unsigned char * image, unsigned char * row_average, int width, int height)
{
	/************************
	�ڴ�ʵ����ͼ��ÿ�лҶ�ƽ�����������row_average
	�������������
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

	//double * round_buf = (double *)malloc(sizeof(double)* MAX_HEIGHT);//�������뻺����
	int * average_buf = ( int * )malloc(sizeof(int) * (MAX_HEIGHT + MAX_WIDTH));//ƽ����������
	//char * image_cpy = ( char * )malloc(sizeof(char) * MAX_HEIGHT * MAX_WIDTH);//imageת����
	//char * row_cpy = (char *)malloc(sizeof(char)* MAX_HEIGHT );//imageת����

	//memcpy(image_cpy, image, width * height); //image to image_cpy
	memset(average_buf, 0, (MAX_HEIGHT + MAX_WIDTH));
	//memset(round_buf, 0, MAX_HEIGHT);

	//for (int i = 0; i < MAX_HEIGHT; i++)
	//{
	//	printf(" imnage[%d]: %d\n", i, image[i]);
	//	printf(" imagecpy[%d]: %d\n", i, image_cpy[i]);

	//}
	int i, j;//C98��֧��
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
	�ڴ�ʵ����ͼ��ÿ�лҶ�ƽ�����������col_average
	�������������
	*****************************/
	if (image == 0 || col_average == 0 || width <= 0 || height <= 0)
	{
		printf("Parameter Input Invalid\n");
		return PARAM_ERROR;
	}

	int * average_buf = (int *)malloc(sizeof(int)* (MAX_HEIGHT + MAX_WIDTH));//ƽ����������

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
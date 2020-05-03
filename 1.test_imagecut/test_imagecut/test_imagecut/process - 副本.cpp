#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

#define PARAM_ERROR -1		//��δ���
#define MALLOC_ERROR -2    //�ռ�����ʧ��
int ImageCut(unsigned char * image,
	unsigned int width,
	unsigned int height,
	unsigned int left_cut,
	unsigned int right_cut,
	unsigned int up_cut,
	unsigned int down_cut)
{
	if (image == NULL || width == 0 || height == 0 || width <= (left_cut	+ right_cut) || height <= (up_cut + down_cut)) //����жϱ�������ǰ��
	{
		printf("PARAM_ERROR\n"); //����returnǰ��log
		return PARAM_ERROR;
	}
	int width_cut = width - left_cut - right_cut;//���ڴ��ڴ��д��룬ǰ����α����ж�width �� ��left_cut + right_cut���Ĵ�С
	int height_cut = height - up_cut - down_cut;
	unsigned char * image_tmp = (unsigned char *)malloc(sizeof(unsigned char)* width_cut * height_cut); //�ռ����뾡�������ö����������
	if (image_tmp == NULL) // ����ռ���Ҫ�ж��Ƿ�����ɹ�
	{
		//SL_LOGE("MALLOC_ERROR");
		return MALLOC_ERROR;
	}
	memset(image_tmp, 0, sizeof(unsigned char)* width_cut * height_cut); //����ռ���ʼ��
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

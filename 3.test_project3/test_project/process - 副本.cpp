#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

#define PARAM_ERROR  -1		//��δ���
#define MALLOC_ERROR -2     //�ռ�����ʧ��
#define OTHER_ERROR  -3     //��������
/*������ƺ����ӿ�
1.���һ������������ͼƬ��0-255�ĻҶ�ֱ��ͼ��������ֱ��ͼ�ļ�ֵ�Ͷ�Ӧ�ĻҶȡ�ע�⼫ֵ���ܶ�Ӧ��ֹһ���Ҷ�
2.��ͼƬ�ֳ�8*8����ֵ�ɵ���ͼƬ����8�����������������ұ�Ե���֣���С�飬����ÿһ��ֱ��ͼ���ݣ���ͨ��ֱ��ͼ���ÿ��С����ڵ�5%��10%�ҶȾ�ֵ����ֵ�ɵ�����
3.ͼƬ�ֳ�8*8����ֵ�ɵ���ͼƬ����8�����������������ұ�Ե���֣���С�飬���㡾ÿһ����ڵ�5%��10%�ҶȾ�ֵ����ֵ�ɵ������͡���׵�5%��10%�ҶȾ�ֵ����ֵ�ɵ������Ĳ
���ó������ײ����50���ɵ�����С��ռ��С��İٷֱȡ�
*/

static int GetSectionvalue(unsigned int * grayscale, int sectionmin, int sectionmax, unsigned int * value)
{
	if (NULL == grayscale || sectionmin > sectionmax || NULL == value)
	{
		return PARAM_ERROR;
	}

	unsigned int section_count = 0;                                              // �ӻҶ���ڿ�ʼ�ĵ����ۼ�ֵ
	unsigned int section_graysum = 0;                                            // �ӻҶ���ڿ�ʼ 5%��10%�ĵ�ĻҶ��ۼ�ֵ
	unsigned int section_num = (sectionmax - sectionmin + 1);                    // �Ҷ���� 5%��10% �����ܵ���


	for (int j = 0; j < 255; j++)
	{
		section_count += grayscale[j];

		if (section_count < sectionmin && (section_count + grayscale[j + 1]) >= sectionmin)
		{
			if ((grayscale[j + 1] - (sectionmin - section_count) + 1) >= sectionmin)
			{
				section_graysum = section_num * (j + 1);
			}
			else
			{

				section_graysum = (grayscale[j + 1]- (sectionmin - section_count) + 1) * (j + 1);
				for (int l = j + 1; l < 255; l++)
				{
					section_count += grayscale[l];
					if (l != j + 1)
						section_graysum += grayscale[l] * l;

					if (section_count < sectionmax && (section_count + grayscale[l + 1]) >= sectionmax)
					{
						section_graysum += (sectionmax - section_count) * (l + 1);
						break;
					}

				}
				break;
			}
		}
	}


	*value = (section_graysum + section_num / 2) / section_num;
	return 0;
}

static int GetBlockdvalue(unsigned char * image, int width, int height, int blackmin, int blackmax, int whitemin, int whitemax)
{
	if (NULL == image)
	{
		printf("%s[%d]: PARAM_ERROR !\n", __FUNCTION__, __LINE__);
		return PARAM_ERROR;
	}

	char errnu = 0;
	int ret = 0;
	unsigned int blackvalue = 0;
	unsigned int whitevalue = 0;
	unsigned int blackgraymin = (width * height * blackmin + 50) / 100;        // �Ҷ���� 5%��10% ��ʼ������
	unsigned int blackgraymax = (width * height * blackmax + 50) / 100;        // �Ҷ���� 5%��10% ��ֹ������

	unsigned int whitegraymin = (width * height * (100 - whitemin) + 50) / 100;        // �Ҷ���� 5%��10% ��ʼ������
	unsigned int whitegraymax = (width * height * (100 - whitemax) + 50) / 100;        // �Ҷ���� 5%��10% ��ֹ������

	unsigned int * grayscale = (unsigned int *)malloc(sizeof(unsigned int)* 256);

	if (NULL == grayscale)
	{
		errno = MALLOC_ERROR;
		goto err;
	}
	memset(grayscale, 0, (sizeof(unsigned int)* 256));

	for (int i = 0; i < width * height; i++)
	{
		grayscale[image[i]] += 1;
	}

	/*
	int b = 0;
	for (int a = 0; a < 255; a++)
	{
	if (grayscale[a] != 0)
	{
	b++;
	printf("[%3d]=%3d ", a, grayscale[a]);
	if (b % 8 == 0)
	printf("\n");
	}
	}
	printf("\n");
	*/

	if (0 > GetSectionvalue(grayscale, blackgraymin, blackgraymax, &blackvalue))
	{
		errno = PARAM_ERROR;
		goto err;
	}

	if (0 > GetSectionvalue(grayscale, whitegraymax, whitegraymin, &whitevalue))
	{
		errno = PARAM_ERROR;
		goto err;
	}

	return (whitevalue - blackvalue);

err:
	if (NULL == grayscale)
	{
		free(grayscale);
	}
	return errnu;
}

int GetDvalueimage(unsigned char * image, int width, int height, image_parameter &imagepar, int * percent)

{

	//printf("%d %d %d %d %d %d %d \n", imagepar.blockwidth, imagepar.blockheight, imagepar.blackmin, imagepar.blackmax, imagepar.whitemin, imagepar.whitemax, imagepar.dvalue);
	if (NULL == image || NULL == percent || 0 >= width || 0 >= height || 0 >= imagepar.blockwidth ||
		0 >= imagepar.blockheight || 0 >= imagepar.blackmin || 0 >= imagepar.blackmax ||
		0 >= imagepar.whitemin || 0 >= imagepar.whitemax || 0 >= imagepar.blockheight ||
		width < imagepar.blockwidth || height < imagepar.blockwidth)
	{
		printf("%s[%d]: PARAM_ERROR !\n", __FUNCTION__, __LINE__);
		return PARAM_ERROR;
	}

	int errnu = 0;
	int blockdvalue = 0;                       // ÿ��ͼ �ֳ� ÿһ��������׵�����ҶȲ�
	int blockvaluenum = 0;                     // ÿ��ͼ������ֵ��ֵ�Ŀ���

	unsigned char * image_block = (unsigned char *)malloc(sizeof(unsigned char)* imagepar.blockwidth * imagepar.blockheight);
	if (NULL == image_block)
	{
		printf("%s[%d]: MALLOC_ERROR !\n", __FUNCTION__, __LINE__);
		errnu = MALLOC_ERROR;
		goto err;
	}
	memset(image_block, 0, sizeof(unsigned char)* imagepar.blockwidth * imagepar.blockheight);

	int wblocknum = width / imagepar.blockwidth;
	int hblocknum = height / imagepar.blockheight;
	//int blocknum = (width / imagepar.cutwidth)*(height / imagepar.cutheight);

	for (int i = 0; i < hblocknum; i++)
	{
		for (int j = 0; j < wblocknum; j++)
		{
			memset(image_block, 0, sizeof(unsigned char)* imagepar.blockwidth * imagepar.blockheight);

			for (int k = 0; k < imagepar.blockwidth * imagepar.blockheight; k++)
			{
				image_block[k] = image[i * width + j * imagepar.blockwidth + k / imagepar.blockwidth * width + k % imagepar.blockwidth];
			}

			blockdvalue = GetBlockdvalue(image_block, imagepar.blockwidth, imagepar.blockheight, imagepar.blackmin, imagepar.blackmax, imagepar.whitemin, imagepar.whitemax);
			//printf("%s[%d]: The blockdvalue = %4d  %3d \n", __FUNCTION__, __LINE__, blockdvalue, a++);
			if (blockdvalue < 0)
			{
				printf("%s[%d]: The blockdvalue error", __FUNCTION__, __LINE__);
				errnu = OTHER_ERROR;
				goto err;
			}
			else if (blockdvalue > imagepar.dvalue)
			{
				blockvaluenum++;
			}
		}
	}
	*percent = blockvaluenum * 10000 / (wblocknum * hblocknum);
	printf("%s[%d]:@@@ blocknum = %3d  blockvaluenum = %3d  percent = %3d\n", __FUNCTION__, __LINE__, (wblocknum * hblocknum), blockvaluenum, *percent);
	free(image_block);
	return 0;
err:
	if (NULL != image_block)
	{
		free(image_block);
	}
	return errnu;
}


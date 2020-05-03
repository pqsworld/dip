#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

#define PARAM_ERROR  -1		//入参错误
#define MALLOC_ERROR -2     //空间申请失败
#define OTHER_ERROR  -3     //其他错误
/*自行设计函数接口
1.设计一个函数：计算图片的0-255的灰度直方图，并传出直方图的极值和对应的灰度。注意极值可能对应不止一个灰度
2.把图片分成8*8（数值可调，图片不被8整除就舍弃最下最右边缘部分）的小块，计算每一块直方图数据，并通过直方图算出每个小块最黑的5%到10%灰度均值（数值可调）。
3.图片分成8*8（数值可调，图片不被8整除就舍弃最下最右边缘部分）的小块，计算【每一块最黑的5%到10%灰度均值（数值可调）】和【最白的5%到10%灰度均值（数值可调）】的差，
最后得出最黑最白差大于50（可调）的小块占总小块的百分比。
*/

static int GetSectionvalue(unsigned int * grayscale, int sectionmin, int sectionmax, unsigned int * value)
{
	if (NULL == grayscale || sectionmin > sectionmax || NULL == value)
	{
		return PARAM_ERROR;
	}

	unsigned int section_count = 0;                                              // 从灰度最黑开始的点数累加值
	unsigned int section_graysum = 0;                                            // 从灰度最黑开始 5%到10%的点的灰度累加值
	unsigned int section_num = (sectionmax - sectionmin + 1);                    // 灰度最黑 5%到10% 区间总点数


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
	unsigned int blackgraymin = (width * height * blackmin + 50) / 100;        // 灰度最黑 5%到10% 开始计数点
	unsigned int blackgraymax = (width * height * blackmax + 50) / 100;        // 灰度最黑 5%到10% 终止计数点

	unsigned int whitegraymin = (width * height * (100 - whitemin) + 50) / 100;        // 灰度最白 5%到10% 开始计数点
	unsigned int whitegraymax = (width * height * (100 - whitemax) + 50) / 100;        // 灰度最白 5%到10% 终止计数点

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
	int blockdvalue = 0;                       // 每张图 分成 每一块的最黑最白的区间灰度差
	int blockvaluenum = 0;                     // 每张图超过差值阈值的块数

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


//#include "afx.h"
#include<stdio.h>
#include<windows.h>
#include <stdlib.h> 
#include<string.h>
#include <WinBase.h>
#include "imageOperation.h"
#include "process.h"

char ** path = NULL;
unsigned int path_count = 0;

//image init
int pathinit()
{
	path_count = 100;
	return 0;
}
//image deinit
void path_deinit()
{
}

//fopen > image,width,height
int get_one_image(unsigned char * image,int* width,int* height)
{
	GFPLoadImage("./test/0000.bmp", image, height, width);
	return 0;
}

//image compare
int ImageCompare(unsigned char* image_ori, unsigned char* image_cmp, int height, int width)
{
	for (int i = 0; i < width * height; i++)
	{
		if (image_ori[i] != image_cmp[i])
		{
			return -1;
		}
	}
	return 0;
}
int maintest()
{
	//分配图片缓冲区内存
	unsigned char * image_buf = (unsigned char *)malloc(sizeof(unsigned char)* MAX_HEIGHT * MAX_WIDTH * 2);
	//异常处理
	if (image_buf == NULL) 
	{
		return -1;
	}
	//分配对比图片内存
	unsigned char * image_cmp = image_buf + sizeof(unsigned char)* MAX_HEIGHT * MAX_WIDTH;
	
	int height = 0;
	int width = 0;
	int width_cmp = 0;
	int height_cmp = 0;
	pathinit();
	char image_path[100] = {0};

	for (unsigned int i = 0; i < path_count; i++)
	{
		get_one_image(image_buf, &width, &height);
		//process
		//printf("width=%d, height=%d\n\n\n\n\n", width, height);
		int ret = ImageCut(image_buf, width, height, i + 1, i + 1, i + 1, i + 1);
		if (ret < 0)
		{
			break;
		}
		/*sprintf_s(image_path, "./result/asdasdasd0000-cut-_%d.bmp", i);
		GFP_Write_Bmp_8(image_path, image_buf, height - 2 * (i + 1), width - 2 * (i + 1));*/

		if ((2 * i + 2) >= (unsigned int)width || (2 * i + 2) >= (unsigned int)height)
		{
			printf("error out going wrong\n");
			goto err;
		}
		//check useful
		//sprintf_s 安全模式的Sprintf i按“”的格式存在imagepath中
		sprintf_s(image_path, "./result/0000_%d.bmp",i);
		ret = GFPLoadImage(image_path, image_cmp, &height_cmp, &width_cmp);

		if (ret == -1)
		{
			printf("check files has been changed ,please open the zip again\n");
			goto err;
		}
		//check TF
		ret = ImageCompare(image_cmp, image_buf, height_cmp, width_cmp);

		if (ret == -1)
		{
			printf("program run error\n");
			goto err;
		}
		memset(image_path, 0, 100);

	}	
	printf("test pass\n");
	path_deinit();
	free(image_buf);
	//GFPLoadImage()
	return 0;
err:
	path_deinit();
	free(image_buf);
	//GFPLoadImage()
	return -1;
}
int main()//maintest的测试结果冻结处理
{ 
	maintest();
	system("pause");
	return 0;
}
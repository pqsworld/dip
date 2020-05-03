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
int image_num = 0;
FILE* rst = NULL;
int pathinit()
{
	path_count = 100;
	return 0;
}
void path_deinit()
{
}
int get_one_image(unsigned char * image,int* width,int* height)//取test的编号imagenum图放到image中
{
	char filename[50] = { 0 };
	sprintf_s(filename, "./test/0000_%d.bmp", image_num);	//取位图文件到0000_i到filename中
	
	//filename传到image中 间杂有图像处理和参数处理
	int ret = GFPLoadImage(filename, image, height, width);
	if (ret == -1)
	{
		printf("check files has been changed ,please open the zip again\n");
		return -1;
	}
	return 0;
}
int ResultCompare(unsigned char* row_average, unsigned char* col_average, int height, int width)
{
	char Temp[50] = { 0 };
	unsigned char* row_result = (unsigned char *)malloc(sizeof(unsigned char) * height);
	unsigned char* col_result = (unsigned char *)malloc(sizeof(unsigned char)* width);
	if (row_result == NULL || col_result == NULL)
	{
		printf("malloc fail\n");
		goto err;
	}
	memset(row_result, 0, sizeof(unsigned char)* height);
	memset(col_result, 0, sizeof(unsigned char)* width);
	fscanf(rst, "%s", Temp);
	for (int i = 0; i < height; i++)
	{
		fscanf(rst, "%s", Temp);
		row_result[i] = (unsigned char)atol(Temp);
		if (row_average[i] != row_result[i])
		{
			printf("row average is wrong\n");
			goto err;
		}
		
	}
	for (int i = 0; i < width; i++)
	{
		fscanf(rst, "%s", Temp);
		col_result[i] = (unsigned char)atol(Temp);
		if (col_average[i] != col_result[i])
		{
			printf("row average is wrong\n");
			goto err;
		}	
	}
	free(row_result);
	free(col_result);
	//free();
	return 0;
err:
	if (row_result != NULL)
	{
		free(row_result);
	}
	if (col_result != NULL)
	{
		free(col_result);
	}
	return -1;
}
int SaveResult(unsigned char* row_average, unsigned char* col_average, int height, int width, int image_num)
{
	char filename[50] = { 0 };
	sprintf_s(filename, "./test/0000_%d.bmp", image_num);
	if (rst == NULL)
	{
		printf("create file failed\n");
		return -1;
	}
	fprintf(rst, "%s\r\n",filename);
	for (int i = 0; i < height; i++)
	{
		fprintf(rst, "%d\t", row_average[i]);
	}
	fprintf(rst,"\r\n");
	for (int i = 0; i < width; i++)
	{
		fprintf(rst, "%d\t", col_average[i]);
	}
	fprintf(rst, "\r\n");
	return 0;
}
int maintest()
{
	unsigned char * image_buf=  (unsigned char *)malloc(sizeof(unsigned char)* MAX_HEIGHT * MAX_WIDTH);
	if (image_buf == NULL )
	{
		printf("malloc fail\n");
		goto err;
	}
	fopen_s(&rst, "./result/result1.txt", "r");
	if (rst == NULL)
	{
		printf("open file failed\n");
		goto err;
	}
	int height = 0;
	int width = 0;
	pathinit();
	for (unsigned int i = 0; i < path_count; i++)
	{
		int ret1 = get_one_image(image_buf, &width, &height);//image_buf 获取的图像信息 width 图图像宽 heigth 图像高
		if (ret1 == -1)
		{
			goto err;
		}

		image_parameter imagepar = {8,8,5,10,5,10,100};//blockwidth;blockheight; blackmin;blackmax;whitemin;whitemax; dvalue;

		int retvalue;
		//float percent = 0.0;
		printf("%s[%d]: @@@ %3d %3d %3d  \n", __FUNCTION__, __LINE__, width, height, ++i);//预定义函数名	行数 图像的宽度和高度 i值（注意++i）
		int ret = GetDvalueimage(image_buf, width, height, imagepar, & retvalue);
		if (ret < 0)
		{
			goto err;
		}
		float percent = (float)retvalue / 100;
		printf("%s[%d]: @@@ percent = %.2f\n\n", __FUNCTION__, __LINE__, percent);
		//SaveResult(row_average, col_average, height, width, image_num);
		/*ret = ResultCompare(row_average, col_average, height, width);
		if (ret == -1)
		{
			printf("program run error\n");
			goto err;
		}
		image_num++;*/
		image_num++;
	}
	printf("test pass\n");
	path_deinit();
	fclose(rst);
	free(image_buf);
	//GFPLoadImage()
	return 0;

err:
	path_deinit();
	if (rst != NULL)
	{
		fclose(rst);
	}
	if (image_buf != NULL)
	{
		free(image_buf);
	}
	
	return -1;
}
int main()
{
	maintest();
	system("pause");
	return 0;
}
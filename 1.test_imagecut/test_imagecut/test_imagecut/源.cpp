#include <string.h>       
#include <malloc.h>  
#include<cstdlib>
#include<cstdio>
#include<cmath>
#include<iomanip>
#include<time.h>//时间相关头文件，可用其中函数计算图像处理速度  
#include"readbmp.h"
#include"savebmp.h"

void image_cut()
{
	unsigned char *imagedata = NULL; //动态分配存储原图片的像素信息的二维数组
	unsigned char *imagedataCut = NULL;//动态分配存储裁剪后的图片的像素信息的二维数组
	char readPath[] = "D:\\C++_file\\image_deal_C++\\IMAGE_JIEQU\\lunpan.bmp";
	readBmp(readPath);
	imagedata = pBmpBuf;
	//===========================================图片裁剪处理====================================================//
	int leftdownx, leftdowny, rightupx, rightupy;/////用户输入数值
	int Rleftdownx, Rleftdowny, Rrightupx, Rrightupy;/////转换成实际可以使用数值
	cout << "请输入要裁剪的矩形区域的左下角和右上角的坐标（连续四个整数值，如50 50 300 300):" << endl;
	cin >> leftdownx;
	cin >> leftdowny;
	cin >> rightupx;
	cin >> rightupy;
	//------------------------将用户输入的矩形框限定在原图像中--------------------------------//
	if (leftdownx < 0 && leftdowny < 0)
	{
		Rleftdownx = 0;
		Rleftdowny = 0;
	}
	else if (leftdownx <= 0 && leftdowny >= 0)
	{
		Rleftdownx = 0;
		Rleftdowny = leftdowny;
	}
	else if (leftdownx >= 0 && leftdowny <= 0)
	{
		Rleftdownx = leftdownx;
		Rleftdowny = 0;
	}
	else if (leftdownx > 0 && leftdowny > 0)
	{
		Rleftdownx = leftdownx;
		Rleftdowny = leftdowny;
	}

	if (rightupx >= bmpWidth)
	{
		Rrightupx = bmpWidth;
	}
	else
	{
		Rrightupx = rightupx;
	}
	if (rightupy >= bmpHeight)
	{
		Rrightupy = bmpHeight;
	}
	else
	{
		Rrightupy = rightupy;
	}


	int CutWidth, CutHeight;
	CutWidth = Rrightupx - Rleftdownx;
	CutHeight = Rrightupy - Rleftdowny;////矩形框实际高度

	int lineByte2 = (CutWidth * biBitCount / 8 + 3) / 4 * 4;//灰度图像有颜色表，且颜色表表项为256
	imagedataCut = new unsigned char[lineByte2 * CutHeight];
	//---------------------------原始图像数据数组指针移动到矩形框的左下角。-------------------------------------------//

	imagedata = imagedata + (Rleftdowny)*bmpWidth * 3;

	//----------------------------裁剪区域数据提取-------------------------------------------//
	for (int i = 0; i < CutHeight; i++)
	{
		for (int j = 0; j < CutWidth; j++)
		for (int k = 0; k < 3; k++)
			*(imagedataCut + i *lineByte2 + j * 3 + k) = *(imagedata + i * bmpWidth * 3 + j * 3 + k + Rleftdownx * 3);////此式子一定要注意写法。主要是注意二维数组指针的用法。

	}

	char writePath[] = "D:\\C++_file\\image_deal_C++\\IMAGE_JIEQU\\1.bmp";
	saveBmp(writePath, imagedataCut, CutWidth, CutHeight, biBitCount, pColorTable);
	printf("裁剪变换完成，请查看bmp文件。\n\n");

	//释放内存
	//delete[] imagedata;///不能释放imagedata，里面还有数据。
	delete[] imagedataCut;
}
int main()
{
	...

}
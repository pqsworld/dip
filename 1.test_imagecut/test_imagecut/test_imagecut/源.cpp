#include <string.h>       
#include <malloc.h>  
#include<cstdlib>
#include<cstdio>
#include<cmath>
#include<iomanip>
#include<time.h>//ʱ�����ͷ�ļ����������к�������ͼ�����ٶ�  
#include"readbmp.h"
#include"savebmp.h"

void image_cut()
{
	unsigned char *imagedata = NULL; //��̬����洢ԭͼƬ��������Ϣ�Ķ�ά����
	unsigned char *imagedataCut = NULL;//��̬����洢�ü����ͼƬ��������Ϣ�Ķ�ά����
	char readPath[] = "D:\\C++_file\\image_deal_C++\\IMAGE_JIEQU\\lunpan.bmp";
	readBmp(readPath);
	imagedata = pBmpBuf;
	//===========================================ͼƬ�ü�����====================================================//
	int leftdownx, leftdowny, rightupx, rightupy;/////�û�������ֵ
	int Rleftdownx, Rleftdowny, Rrightupx, Rrightupy;/////ת����ʵ�ʿ���ʹ����ֵ
	cout << "������Ҫ�ü��ľ�����������½Ǻ����Ͻǵ����꣨�����ĸ�����ֵ����50 50 300 300):" << endl;
	cin >> leftdownx;
	cin >> leftdowny;
	cin >> rightupx;
	cin >> rightupy;
	//------------------------���û�����ľ��ο��޶���ԭͼ����--------------------------------//
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
	CutHeight = Rrightupy - Rleftdowny;////���ο�ʵ�ʸ߶�

	int lineByte2 = (CutWidth * biBitCount / 8 + 3) / 4 * 4;//�Ҷ�ͼ������ɫ������ɫ�����Ϊ256
	imagedataCut = new unsigned char[lineByte2 * CutHeight];
	//---------------------------ԭʼͼ����������ָ���ƶ������ο�����½ǡ�-------------------------------------------//

	imagedata = imagedata + (Rleftdowny)*bmpWidth * 3;

	//----------------------------�ü�����������ȡ-------------------------------------------//
	for (int i = 0; i < CutHeight; i++)
	{
		for (int j = 0; j < CutWidth; j++)
		for (int k = 0; k < 3; k++)
			*(imagedataCut + i *lineByte2 + j * 3 + k) = *(imagedata + i * bmpWidth * 3 + j * 3 + k + Rleftdownx * 3);////��ʽ��һ��Ҫע��д������Ҫ��ע���ά����ָ����÷���

	}

	char writePath[] = "D:\\C++_file\\image_deal_C++\\IMAGE_JIEQU\\1.bmp";
	saveBmp(writePath, imagedataCut, CutWidth, CutHeight, biBitCount, pColorTable);
	printf("�ü��任��ɣ���鿴bmp�ļ���\n\n");

	//�ͷ��ڴ�
	//delete[] imagedata;///�����ͷ�imagedata�����滹�����ݡ�
	delete[] imagedataCut;
}
int main()
{
	...

}
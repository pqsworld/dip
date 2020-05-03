#ifndef _H_IMAGEOPERATION_H_
#define _H_IMAGEOPERATION_H_


#define BI_RGB              0L
#define BI_RLE8             1L
#define BI_RLE4             2L
#define BMP_TYPE            0x4d42
#define WIDTHBYTES(bits)    (((bits)+31)/32*4)
#define MIN(x,y)            (x < y ? x : y)



#pragma   pack(1)  // 让编译器按1字节对齐

//BITMAPFILEHEADER结构的长度是固定的14个字节，描述文件的有关信息
typedef struct _BITMAPFILEHEADER_
{

	unsigned short		bfType;//must be 0x4D42.
	unsigned long		bfSize;//the size of the whole bitmap file.
	unsigned short		bfReserved1;
	unsigned short		bfReserved2;
	unsigned long		bfOffBits;//the sum bits of BITMAPFILEHEADER,BITMAPINFOHEADER and RGBQUAD;the index byte of the image data.

} WL_BITMAPFILEHEADER, *LPWL_BITMAPFILEHEADER;

//BITMAPINFOHEADER结构的长度是固定的40个字节，描述图像的有关信息
typedef struct _BITMAPINFOHEADER_
{

	unsigned long		biSize;//the size of this struct.40 bytes.
	long				biWidth;//the width of image data. the unit is pixel.
	long				biHeight;//the height of image data. the unit is pixel
	unsigned short      biPlanes;//must be 1.
	unsigned short      biBitCount;;//the bit count of each pixel.usually be 1,4,8,or 24.
	unsigned long		biCompression;//is this image compressed.0 indicates no compression.
	unsigned long		biSizeImage;//the size of image data
	long				biXPelsPerMeter;
	long				biYPelsPerMeter;
	unsigned long		biClrUsed;
	unsigned long		biClrImportant;

} WL_BITMAPINFOHEADER, *LPWL_BITMAPINFOHEADER;
//调色板即使将图像数据中使用的一种颜色对应到RGB颜色中，指针之类的东西
typedef struct _RGBQUAD_
{

	unsigned char    rgbBlue;
	unsigned char    rgbGreen;
	unsigned char    rgbRed;
	unsigned char    rgbReserved;

} WL_RGBQUAD, *LPWL_RGBQUAD;


#pragma   pack()  // 还原





#ifdef __cplusplus
extern "C" {
#endif


int GFPLoadImage(const char filename[], unsigned char *ucImage, int *h, int *w);
void GFP_Write_Bmp_1(const char filename[], unsigned char *ucImg, int h, int w);
void GFP_Write_Bmp_8(const char filename[], unsigned char *ucImg, int h, int w);
void GFP_Write_Bmp_24(const char filename[], WL_RGBQUAD *ucImg, int h, int w);
int GFPLoadBin(const char filename[], unsigned char *ucImage, int h, int w);
void GFP_Bmp_Change_8(unsigned char *imageChange, unsigned char *image, int h, int w, int c_h, int c_w);

void SaveWSQ(char *name, unsigned char *data, int len);
int WritePGM(char *PGMFileName, unsigned char *idata, int height, int width);
double ImageComparePSNR(unsigned char *idata1, int height1, int width1, 
						unsigned char *idata2, int height2, int width2);





#ifdef __cplusplus
}
#endif

#endif //结束第一个#ifndef



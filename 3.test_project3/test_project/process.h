
#define  MAX_WIDTH 200
#define  MAX_HEIGHT 200

struct image_parameter
{
	int blockwidth;//像素块宽度
	int blockheight;//像素块高度
	int blackmin;//黑块的灰度极小均值
	int blackmax;//黑块的灰度极大均值
	int whitemin;//白块的灰度极小均值
	int whitemax;//白块的灰度极大均值
	int dvalue;//黑白块的灰度差值
};

int GetDvalueimage(unsigned char * image, int width, int height, image_parameter &imagepar, int * percent);




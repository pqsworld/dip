
#define  MAX_WIDTH 200
#define  MAX_HEIGHT 200

struct image_parameter
{
	int blockwidth;//���ؿ���
	int blockheight;//���ؿ�߶�
	int blackmin;//�ڿ�ĻҶȼ�С��ֵ
	int blackmax;//�ڿ�ĻҶȼ����ֵ
	int whitemin;//�׿�ĻҶȼ�С��ֵ
	int whitemax;//�׿�ĻҶȼ����ֵ
	int dvalue;//�ڰ׿�ĻҶȲ�ֵ
};

int GetDvalueimage(unsigned char * image, int width, int height, image_parameter &imagepar, int * percent);




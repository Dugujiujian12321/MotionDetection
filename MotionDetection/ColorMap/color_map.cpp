/*
Thu Oct 26 17:57:33 CST 2017
ʹ����ɫ�ֲ�������ͼ�����ƶ�

���ڱȽϻҶ�ֱ��ͼ�ķ�����������ͼ�����ƶ����н϶࣬
һ��ԭ����ֱ��ͼ��������ԣ�
����ӳͼ�����ظ��Ҷ�ֵ��������
���ܷ�ӳͼ������ṹ��
��һ��ԭ�������ʹ�ø÷���ʱ��
���ڲ�ɫͼ��
һ�㶼�ǽ���תΪ�Ҷ�ͼ��
Ȼ���ڼ�����Ҷ�ֱ��ͼ��
����ٲ�������Ƚϣ�
�������ڲ�ɫת�Ҷȵ�ת�������н���ʧͼ����ɫ��Ϣ��
�����ڼ���ʱ���ڴ������С�
�ɵ�һ��ԭ����������к����ҵ����������
���ǲ������������
���ڶ�ԭ��
���ڲ�ɫͼ��
���ǿ��Ի�һ�ַ�ʽ����ֱ��ͼ��
��һ��Ĳ��͡�����ͼƬ������ԭ���������ᵽ�����ֲ�ɫͼ��ֱ��ͼ���㷽����
���ﲻ��׸����
ͨ��ʵ�鷢�֣����ַ�ʽ�ܺõĿ˷��˵ڶ���ԭ����������У�
���ƶȼ�����׼ȷ��������ߡ�
�������㷨ʵ�֣�
�����н���ɫ�ֳ�8��������ӳ�䣬
����ֳ�16���������ߵ�������ӳ�䣬
����׼ȷ�Ȼ���ߡ�
��Ȼ�����㷨��ͼ������������ر����У�
��̫�����������������������Ƶ���仯��ͼ��
*/
#include <iostream>                                                      
#include <opencv2/opencv.hpp>
#include <math.h>
using namespace std;
using namespace cv;

// bgr��ɫ������ӳ��
uchar ColorValMapping(uchar &val)
{
	uchar mapVal = 0;
	if (val > 223)
	{
		// [224 ~ 255]
		mapVal = 7;
	}
	else if (val > 191)
	{
		// [192 ~ 223]
		mapVal = 6;
	}
	else if (val > 159)
	{
		// [160 ~ 191]
		mapVal = 5;
	}
	else if (val > 127)
	{
		// [128 ~ 159]
		mapVal = 4;
	}
	else if (val > 95)
	{
		// [96 ~ 127]
		mapVal = 3;
	}
	else if (val > 63)
	{
		// [64 ~ 95]
		mapVal = 2;
	}
	else if (val > 31)
	{
		// [32 ~ 63]
		mapVal = 1;
	}
	else
	{
		// [0 ~ 31]
		mapVal = 0;
	}

	return mapVal;
}

// ������ɫֱ��ͼ����
void CompImageFeature(Mat &inputImg, int *pImgFeature)
{
	int index = 0;
	int row = inputImg.rows;
	int col = inputImg.cols;
	uchar map_b = 0, map_g = 0, map_r = 0;
	uchar* pImg = inputImg.data;

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			// ��ɫӳ��
			map_b = ColorValMapping(pImg[3 * j]);
			map_g = ColorValMapping(pImg[3 * j + 1]);
			map_r = ColorValMapping(pImg[3 * j + 2]);
			index = map_b * 64 + map_g * 8 + map_r;
			pImgFeature[index]++;
		}

		pImg += inputImg.step;
	}
}

// ��������ͼ�����ƶ�
double CompImageSimilarity(Mat &img0, Mat &img1)
{
	// ������ɫֱ��ͼ����>>��8������,��8*8*8 = 512�����
	int imgFeature0[512] = { 0 };
	int imgFeature1[512] = { 0 };
	CompImageFeature(img0, imgFeature0);
	CompImageFeature(img1, imgFeature1);

	// �����������ƶ�>>����ֵԽ�ӽ�1,�����н�Խ�ӽ�0��,��������Խ����
	double sum_square0 = 0.0, sum_square1 = 0.0, sum_multiply = 0.0;
	for (int i = 0; i < 512; i++)
	{
		sum_square0 += imgFeature0[i] * imgFeature0[i];
		sum_square1 += imgFeature1[i] * imgFeature1[i];
		sum_multiply += imgFeature0[i] * imgFeature1[i];
	}

	return sum_multiply / (sqrt(sum_square0) * sqrt(sum_square1));
}

int main(int argc, char **argv)
{
	double similarity = 0.0;

	// ��������ͼ�����ƶ�
	//Mat img0 = imread(argv[1]);
	//Mat img1 = imread(argv[2]);

	Mat img0 = imread("1.bmp");
	Mat img1 = imread("2.bmp");
	Mat img2 = imread("9.bmp");

	//720P
	//Mat img0 = imread("12.jpg");
	//Mat img1 = imread("13.jpg");
	//Mat img2 = imread("14.jpg");

	//1080P
	//Mat img0 = imread("1.jpeg");
	//Mat img1 = imread("2.jpeg");
	//Mat img2 = imread("3.jpeg");
	
	cout << " �����Է���������ɫ�ֲ���(ֱ��ͼ�Ľ���)�� " << endl ;
	double start = static_cast<double>(getTickCount());

	similarity = CompImageSimilarity(img0, img1);
	printf("pic1 pic2-------���ƶ�-------> %f \n", similarity);

	double start2 = static_cast<double>(getTickCount());
	cout << "pic1 pic2-------��ʱ -------> " << (start2 - start) / getTickFrequency() * 1000 << " ms" << endl;
	cout << endl;
	similarity = CompImageSimilarity(img0, img2);
	printf("pic1 pic3-------���ƶ�-------> %f \n", similarity);

	double end = ((double)getTickCount() - start2) / getTickFrequency() * 1000;
	cout << "pic1 pic3-------��ʱ-------> " << end << " ms" << endl;

	getchar();
	return 0;
}

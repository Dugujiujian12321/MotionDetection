/*
Thu Sep 28 14:35:08 CST 2017
���ø�֪��ϣ�㷨,
����������ͼƬ�����ƶȡ�
*/
#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

#define IMG01	argv[1]	
#define IMG02	argv[2]

int PerHash(string ImageName1, string ImageName2) {

	cv::Mat matSrc1, matSrc2;

	matSrc1 = cv::imread(ImageName1, CV_LOAD_IMAGE_COLOR);
	matSrc2 = cv::imread(ImageName2, CV_LOAD_IMAGE_COLOR);

	//1.��С�ߴ�
	//��ͼƬ��С��8x8�ĳߴ磬�ܹ�64�����ء�
	//��һ����������ȥ��ͼƬ��ϸ�ڣ�ֻ�����ṹ�������Ȼ�����Ϣ��������ͬ�ߴ硢����������ͼƬ���졣
	cv::Mat matDst1, matDst2;

	cv::resize(matSrc1, matDst1, cv::Size(8, 8), 0, 0, cv::INTER_CUBIC);
	cv::resize(matSrc2, matDst2, cv::Size(8, 8), 0, 0, cv::INTER_CUBIC);

	//2.��ɫ��
	//����С���ͼƬ��תΪ64���Ҷȡ�Ҳ����˵���������ص��ܹ�ֻ��64����ɫ��
	cv::cvtColor(matDst1, matDst1, CV_BGR2GRAY);
	cv::cvtColor(matDst2, matDst2, CV_BGR2GRAY);

	//3.����ƽ��ֵ
	//��������64�����صĻҶ�ƽ��ֵ��
	int iAvg1 = 0, iAvg2 = 0;
	int arr1[64], arr2[64];

	for (int i = 0; i < 8; i++) {
		uchar* data1 = matDst1.ptr<uchar>(i);
		uchar* data2 = matDst2.ptr<uchar>(i);

		int tmp = i * 8;

		for (int j = 0; j < 8; j++) {
			int tmp1 = tmp + j;

			arr1[tmp1] = data1[j] / 4 * 4;
			arr2[tmp1] = data2[j] / 4 * 4;

			iAvg1 += arr1[tmp1];
			iAvg2 += arr2[tmp1];
		}
	}

	iAvg1 /= 64;
	iAvg2 /= 64;

	//4.�Ƚ����صĻҶ�
	//��ÿ�����صĻҶȣ���ƽ��ֵ���бȽϡ����ڻ����ƽ��ֵ����Ϊ1��С��ƽ��ֵ����Ϊ0��
	for (int i = 0; i < 64; i++) {
		arr1[i] = (arr1[i] >= iAvg1) ? 1 : 0;
		arr2[i] = (arr2[i] >= iAvg2) ? 1 : 0;
	}

	//5.�����ϣֵ
	//����һ���ıȽϽ���������һ�𣬾͹�����һ��64λ�����������������ͼƬ��ָ�ơ�
	//��ϵĴ��򲢲���Ҫ��ֻҪ��֤����ͼƬ������ͬ����������ˡ�
	int iDiffNum = 0;

	for (int i = 0; i < 64; i++)
	if (arr1[i] != arr2[i])
		++iDiffNum;

	return iDiffNum;
}

void distance(int iDiffNum)
{
	//�õ�ָ���Ժ󣬾Ϳ��ԶԱȲ�ͬ��ͼƬ������64λ���ж���λ�ǲ�һ���ġ�
	//�������ϣ����ͬ�ڼ���"��������"��Hamming distance����
	//�������ͬ������λ������5����˵������ͼƬ�����ƣ��������10����˵���������Ų�ͬ��ͼƬ��
	if (iDiffNum <= 5)
		cout << "two images are very similar!" << endl;
	else if (iDiffNum > 10)
		cout << "they are two different images!" << endl;
	else
		cout << "two image are somewhat similar!" << endl;
}

int main(int argc, char **argv)
{
	cout << " �����Է���������֪��ϣ�㷨�� " << endl;

	//string ImageName1 = IMG01;
	//string ImageName2 = IMG02;

	//string ImageName1 = "1.bmp";
	//string ImageName2 = "2.bmp";
	//string ImageName3 = "9.bmp";

	//720
	//string ImageName1 = "12.jpg";
	//string ImageName2 = "13.jpg";
	//string ImageName3 = "14.jpg";

	//1080
	string ImageName1 = "1.jpeg";
	string ImageName2 = "2.jpeg";
	string ImageName3 = "3.jpeg";

	double start = static_cast<double>(getTickCount());
	int iDiffNum1 = PerHash(ImageName1, ImageName2);
	double start2 = static_cast<double>(getTickCount());

	cout << "iDiffNum = " << iDiffNum1 << endl;
	distance(iDiffNum1);
	cout << "pic1 pic2-------��ʱ -------> " << (start2 - start) / getTickFrequency() * 1000 << " ms" << endl;

	cout << endl;
	
	int iDiffNum2 = PerHash(ImageName1, ImageName3);
	double end = ((double)getTickCount() - start2) / getTickFrequency() * 1000;

	cout << "iDiffNum = " << iDiffNum2 << endl;
	distance(iDiffNum2);
	cout << "pic1 pic3-------��ʱ-------> " << end << " ms" << endl;

	getchar();
	return 0;
}

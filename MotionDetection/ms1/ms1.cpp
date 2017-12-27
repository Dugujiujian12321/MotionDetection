#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include <omp.h>
//#include <opencv2/video/tracking_c.h>

//#include<sys/time.h>
#include "opencv2/optflow/motempl.hpp"
#include "iostream"
using namespace cv::motempl;

const double MHI_DURATION = 1;//�����ȵ���(ֵԽ������ʱ��ԽС)
const double MAX_TIME_DELTA = 0.5;
const double MIN_TIME_DELTA = 0.05;
const int N = 2;
const int CONTOUR_MAX_AERA = 1500;//�������
IplImage **buf = 0;
IplImage *mhi = 0;
int last = 0;
int flag;
using namespace std;

void update_mhi(IplImage* img, IplImage* dst, int diff_threshold)
{
	double timestamp = (double)clock() / CLOCKS_PER_SEC;
	//printf("timestamp = %f\n",timestamp);
	CvSize size = cvSize(img->width, img->height); //�õ���ά����ĳߴ�
	int i, idx1, idx2;
	IplImage* silh;
	IplImage* pyr = cvCreateImage(cvSize((size.width & -2) / 2, (size.height & -2) / 2), 8, 1); //����ͼ���׵�ַ��������洢�ռ�
	CvMemStorage *stor;
	CvSeq *cont;

	if (!mhi || mhi->width != size.width || mhi->height != size.height)
	{
		if (buf == 0)
		{
			buf = (IplImage**)malloc(N*sizeof(buf[0]));
			memset(buf, 0, N*sizeof(buf[0]));
		}
		//#pragma omp parallel for
		for (i = 0; i < N; i++)
		{
			cvReleaseImage(&buf[i]);//�ͷ�ͼ���ļ���������ڴ�
			buf[i] = cvCreateImage(size, IPL_DEPTH_8U, 1);//����ͼ���׵�ַ��������洢�ռ�
			cvZero(buf[i]);//��ʼ��
		}
		cvReleaseImage(&mhi);
		mhi = cvCreateImage(size, IPL_DEPTH_32F, 1);
		cvZero(mhi);

		cvCvtColor(img, buf[((last + 1) % N)], CV_BGR2GRAY);//�������ͨ����һ����ɫ�ռ�ת������һ����ɫ�ռ�
	}
	cvCvtColor(img, buf[last], CV_BGR2GRAY);
	idx1 = last;
	idx2 = (last + 1) % N;
	last = idx2;

	silh = buf[idx2];
	cvAbsDiff(buf[idx1], buf[idx2], silh);//֡�� 
	cvThreshold(silh, silh, 30, 1, CV_THRESH_BINARY);//ͼ����ֵ�� 

	cv::Mat m_silh = cv::cvarrToMat(silh);
	cv::Mat m_mhi = cv::cvarrToMat(mhi);
	cv::motempl::updateMotionHistory(m_silh, m_mhi, timestamp, MHI_DURATION);   //�������ص���˶���ʷ 
	cvCvtScale(mhi, dst, 255. / MHI_DURATION,
		(MHI_DURATION - timestamp)*255. / MHI_DURATION);//timestamp��ʱ���;MHI_DURATION����õ��ǵ�ǰʱ��
	cvCvtScale(mhi, dst, 255. / MHI_DURATION, 0);
	cvSmooth(dst, dst, CV_MEDIAN, 3, 0, 0, 0);//// ��ֵ�˲�������С������  

	cvPyrDown(dst, pyr, 7);//// ���²�����ȥ������
	cvDilate(pyr, pyr, 0, 1);// �����Ͳ���������Ŀ��Ĳ������ն�
	cvPyrUp(pyr, dst, 7);//ͼ��������������е�ͼ����ÿ��ά���϶��Ŵ�����
	stor = cvCreateMemStorage(0); //����һ���ڴ�洢��
	cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), stor);//��������
	cvFindContours(dst, stor, &cont, sizeof(CvContour),
		CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));//�Ӷ�ֵͼ����Ѱ������
	//#pragma omp parallel for

	for (; cont; cont = cont->h_next)
	{
		flag = 0;
		CvRect r = ((CvContour*)cont)->rect;

		if (r.height * r.width > CONTOUR_MAX_AERA)
		{

			cvRectangle(img, cvPoint(r.x, r.y),
				cvPoint(r.x + r.width, r.y + r.height),
				CV_RGB(255, 0, 0), 1, CV_AA, 0); //������

			flag = 1;
		}
		else{
			flag = 0;
		}
	}

	cvReleaseMemStorage(&stor);
	cvReleaseImage(&pyr);
	cvZero(mhi);
}

void warning(IplImage *image, int j)
{

	char image_name[50];
	sprintf(image_name, "%s%d%s", "image_test", j, ".jpg");
	//      cvSaveImage(image_name, image);
	flag = 0;
}

int main(int argc, char** argv)
{
	cout << " �����Է���һ��֡�(PDG�ṩ)�� " << endl;

	IplImage* motion = 0;
	char cname[100];
	int numqq = 1;
	//struct timeval tv_begin, tv_end;
	//int ij = 0;
	int j = 0;
	//#pragma omp parallel sections         
	//#pragma omp parallel for              
	//for (int i = 0; i < 3; i++)
	for (int i = 0; i < 20; i++)//720P 1080P
	{
		printf("------num = %d\n", numqq);
		memset(cname, 0, 100);
		//sprintf(cname, "%d.bmp", numqq++);
		sprintf(cname, "img_1080/%d.jpeg", numqq++);
		//printf("------num = %d\n",num);               
		IplImage *image = cvLoadImage(cname);//��ͼ���ļ��������ڴ�
		if (!image)
		{
			printf("image %d no cvLoadImage\n", (numqq - 1));
			getchar();
			return -1;
		}
		//#pragma omp section
		if (image)
		{
			//#pragma omp section                   
			if (!motion)
			{
				motion = cvCreateImage(cvSize(image->width, image->height), 8, 1); //����һ��ͼ��
				cvZero(motion);//�൱�ڳ�ʼ��ͼƬ��ֵ��Ϊ0�������СΪimage�Ŀ�͸�
				motion->origin = image->origin;
			}
		}

		//clock_t t1 = clock();
		//gettimeofday(&tv_begin, NULL);
		double start2 = static_cast<double>(cv::getTickCount());
		update_mhi(image, motion, 4);
		double end_time = ((double)cv::getTickCount() - start2) / cv::getTickFrequency() * 1000;

		std::cout << "pic " << i + 1 << "-------��ʱ-------> " << end_time << " ms" << std::endl;
		//gettimeofday(&tv_end, NULL);
		//clock_t t2 = clock();

		j++;
		//#pragma omp section           
		if (flag){
			//printf("tv_usec :%d  vs\n", (int)(((tv_end.tv_sec) * 1000000 + tv_end.tv_usec) - ((tv_begin.tv_sec) * 1000000 + tv_begin.tv_usec)));
			//warning(image, j);
			//std::cout << "pic " << i + 1 << "-------��ʱ-------> " << end_time << " ms" << std::endl;

			flag = 0;
		}

		cvReleaseImage(&image);
		cvReleaseImage(&motion);
	}
	getchar();
	//return 0;
}


#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/opencv.hpp>  
#include <iostream>
#include <sstream>


using namespace cv;
using namespace std;
Mat frame;
Mat fgMaskMOG2;
Ptr<BackgroundSubtractor> pMOG2;
int keyboard;

int GMM(string videoFilename)
{
	//std::string videoFile = "../test.avi";

	cv::VideoCapture capture(videoFilename);
	//capture.open(videoFile);

	if (!capture.isOpened())
	{
		std::cout << "read video failure" << std::endl;
		return -1;
	}


	//cv::BackgroundSubtractorMOG2 mog;

	cv::Mat foreground;
	cv::Mat background;

	cv::Mat frame;

	double time = 0;
	double start;


	long frameNo = 0;
	while (capture.read(frame))
	{
		++frameNo;

		std::cout << frameNo << std::endl;

		start = static_cast<double>(getTickCount());

		// ͼ��ߴ���С
		cv::resize(frame, frame, cv::Size(), 0.5, 0.5);

		// �˶�ǰ����⣬�����±���  
		pMOG2->apply(frame, foreground/*, 0.001*/);

		// ��ʴ  
		cv::erode(foreground, foreground, cv::Mat());

		// ����  
		cv::dilate(foreground, foreground, cv::Mat());

		pMOG2->getBackgroundImage(background);   // ���ص�ǰ����ͼ��  

		time += ((double)getTickCount() - start) / getTickFrequency() * 1000;
		cout << "Time of Update GMM Background: " << time / frameNo << "ms" << endl;


		cv::imshow("video", foreground);
		cv::imshow("background", frame);


		if (cv::waitKey(25) > 0)
		{
			break;
		}
	}

}

int GMM_pic(string Filename)
{

	//cv::BackgroundSubtractorMOG2 mog;

	cv::Mat foreground;
	cv::Mat background;

	cv::Mat frame = imread(Filename);

	double time = 0;
	double start;

	//long frameNo = 0;
	//while (capture.read(frame))
	//{
		//++frameNo;

		//std::cout << frameNo << std::endl;

		start = static_cast<double>(getTickCount());

		// ͼ��ߴ���С
		cv::resize(frame, frame, cv::Size(), 0.5, 0.5);

		// �˶�ǰ����⣬�����±���  
		pMOG2->apply(frame, foreground/*, 0.001*/);

		// ��ʴ  
		cv::erode(foreground, foreground, cv::Mat());

		// ����  
		cv::dilate(foreground, foreground, cv::Mat());

		pMOG2->getBackgroundImage(background);   // ���ص�ǰ����ͼ��  

		time += ((double)getTickCount() - start) / getTickFrequency() * 1000;
		//cout << "Time of Update GMM Background: " << time / frameNo << "ms" << endl;
		cout << "Time of Update GMM Background: " << time  << "ms" << endl;


		cv::imshow("frame", foreground);
		cv::imshow("background", frame);


		if (cv::waitKey(25) > 0)
		{
			//break;
			return -1;
		}
	//}

}

//��ϸ�˹����ģ��ʵ��
void processVideo(string videoFilename)
{
	// ��Ƶ��ȡ
	VideoCapture capture(videoFilename);
	if (!capture.isOpened())
	{
		// �����Ƶ�ļ��򿪴�����Ϣ
		cerr << "Unable to open video file: " << videoFilename << endl;
		exit(EXIT_FAILURE);
	}
	double time = 0;
	double start;

	// ����q����esc�˳�
	while ((char)keyboard != 'q' && (char)keyboard != 27)
	{
		// ��ȡ��ǰ֡
		if (!capture.read(frame))
		{
			cerr << "Unable to read next frame." << endl;
			cerr << "Exiting..." << endl;
			exit(EXIT_FAILURE);
		}

		start = static_cast<double>(getTickCount());

		// ͼ��ߴ���С
		cv::resize(frame, frame, cv::Size(), 0.5, 0.5);
		//  ����ģ������
		pMOG2->apply(frame, fgMaskMOG2);
		
		time = ((double)getTickCount() - start) / getTickFrequency() * 1000;
		cout << "Time of Update BGDiff Background: " << time << "ms" << endl;

		// �����ǰ֡��
		stringstream ss;
		rectangle(frame, cv::Point(10, 2), cv::Point(100, 20),
			cv::Scalar(255, 255, 255), -1);
		ss << capture.get(CAP_PROP_POS_FRAMES);
		string frameNumberString = ss.str();
		// ���Ͻ���ʾ֡��
		putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
			FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
		// ������
		imshow("Frame", frame);
		imshow("FG Mask MOG 2", fgMaskMOG2);
		keyboard = waitKey(30);
	}
	capture.release();
}

void processVideo_pic(string Filename)
{
	//// ��Ƶ��ȡ
	//VideoCapture capture(videoFilename);
	//if (!capture.isOpened())
	//{
	//	// �����Ƶ�ļ��򿪴�����Ϣ
	//	cerr << "Unable to open video file: " << videoFilename << endl;
	//	exit(EXIT_FAILURE);
	//}
	double time = 0;
	double start;

	// ����q����esc�˳�
	//while ((char)keyboard != 'q' && (char)keyboard != 27)
	//{
	//	// ��ȡ��ǰ֡
	//	if (!capture.read(frame))
	//	{
	//		cerr << "Unable to read next frame." << endl;
	//		cerr << "Exiting..." << endl;
	//		exit(EXIT_FAILURE);
	//	}

		frame = imread(Filename);
		start = static_cast<double>(getTickCount());

		// ͼ��ߴ���С
		cv::resize(frame, frame, cv::Size(), 0.5, 0.5);
		//  ����ģ������
		pMOG2->apply(frame, fgMaskMOG2);

		time = ((double)getTickCount() - start) / getTickFrequency() * 1000;
		cout << "Time of Update BGDiff Background: " << time << "ms" << endl;

		//// �����ǰ֡��
		//stringstream ss;
		//rectangle(frame, cv::Point(10, 2), cv::Point(100, 20),
		//	cv::Scalar(255, 255, 255), -1);
		//ss << capture.get(CAP_PROP_POS_FRAMES);
		//string frameNumberString = ss.str();
		//// ���Ͻ���ʾ֡��
		//putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
		//	FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));

		// ������
		imshow("Frame", frame);
		imshow("FG Mask MOG 2", fgMaskMOG2);
		keyboard = waitKey(30);
	//}
	//capture.release();
}

int main(int argc, char* argv[])
{
	// ����������ģ��
	pMOG2 = createBackgroundSubtractorMOG2();
	// for video
	string inputPath = "D:\\videos\\Record\\20170808_A2.avi";
	//processVideo(inputPath);
	//GMM(inputPath);

	//for pictures
	cout << "img_576 : " << endl;
	cout << "GMM_pic time : " <<  endl;
	GMM_pic("../3rdParty/pics/img_576/1.png");
	GMM_pic("../3rdParty/pics/img_576/2.png");
	GMM_pic("../3rdParty/pics/img_576/3.png");
	cout << "process_pic time : " << endl;
	processVideo_pic("../3rdParty/pics/img_576/1.png");
	processVideo_pic("../3rdParty/pics/img_576/2.png");
	processVideo_pic("../3rdParty/pics/img_576/3.png");
	cout << endl;

	cout << "img_720 : " << endl;
	cout << "GMM_pic time : " << endl;
	GMM_pic("../3rdParty/pics/img_720/1.jpg");
	GMM_pic("../3rdParty/pics/img_720/2.jpg");
	GMM_pic("../3rdParty/pics/img_720/3.jpg");
	cout << "process_pic time : " << endl;
	processVideo_pic("../3rdParty/pics/img_720/1.jpg");
	processVideo_pic("../3rdParty/pics/img_720/2.jpg");
	processVideo_pic("../3rdParty/pics/img_720/3.jpg");
	cout << endl;


	cout << "img_1080 : " << endl;
	cout << "GMM_pic time : " << endl;
	GMM_pic("../3rdParty/pics/img_1080/1.jpeg");
	GMM_pic("../3rdParty/pics/img_1080/2.jpeg");
	GMM_pic("../3rdParty/pics/img_1080/3.jpeg");
	cout << "process_pic time : " << endl;
	processVideo_pic("../3rdParty/pics/img_1080/1.jpeg");
	processVideo_pic("../3rdParty/pics/img_1080/2.jpeg");
	processVideo_pic("../3rdParty/pics/img_1080/3.jpeg");
	cout << endl;

	getchar();
	return 0;
}
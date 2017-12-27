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
int main(int argc, char* argv[])
{
	// ����������ģ��
	pMOG2 = createBackgroundSubtractorMOG2();
	string inputPath = "D:\\videos\\Record\\20170808_A2.avi";
	//processVideo(inputPath);
	GMM(inputPath);
	return 0;
}
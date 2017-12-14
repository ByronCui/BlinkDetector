#pragma once
#include <iostream>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/objdetect/objdetect.hpp>  
#include <time.h>
//#include <sstream>
#include <deque>
#include <numeric>
#include <fstream>

#define DEBUG

class BlinkDetector
{
public:
	explicit BlinkDetector(const std::string faceCascadeName,
		const std::string eyesCascadeName,
		const int itdf,
		const std::string recordfile);
	~BlinkDetector();
	void start();

private:
	void cameraInit();
	void setCascade(const std::string faceCascadeName, const std::string eyesCascadeName);
	cv::Rect detectFace(cv::Mat originImg);
	int detectEyes(cv::Mat face);
	double pupilGrayscale(cv::Mat face, cv::Rect roi);
	cv::CascadeClassifier faceCascade;
	cv::CascadeClassifier eyesCascade;
	cv::VideoCapture capture;
	cv::Mat frame;
	cv::Rect preFaceRect;
	cv::Rect preOneEye;
	std::vector<cv::Rect> faces;
	std::vector<cv::Rect> eyes;
	//std::vector<std::pair<cv::Point, cv::Point>> faceRoi;
	//std::vector<std::pair<cv::Point, cv::Point>> eyesRoi;
	const int intervalToDetectFace = 0;
	std::deque<double> grayDeque;
	std::ofstream record;
	std::string recordFile;
};
#include "BlinkDetector.h"

BlinkDetector::BlinkDetector(const std::string faceCascadeName, const std::string eyesCascadeName, int itdf, const std::string recordfile) :intervalToDetectFace(itdf), recordFile(recordfile)
{
	cameraInit();
	setCascade(faceCascadeName, eyesCascadeName);
	record.open(recordFile, std::ofstream::app);
}

BlinkDetector::~BlinkDetector()
{
	std::cout << "析构开始" << std::endl;
	record.close();
	std::cout << "析构结束" << std::endl;
}


void BlinkDetector::start()
{
	cv::Mat frameGray;
	cv::Mat face;
	cv::Rect faceRect;
	cv::Mat mask;
	cv::Point reCheckRoiCenter;
	cv::Mat reCheckEye;
	int frameCount = 0;
	clock_t pretime, curtime;
	clock_t pretime_blink = 0, curtime_blink = 0;
	pretime = curtime = clock();
	std::string fcString = "";
	int timeToDetectFace = intervalToDetectFace;

	int x = 0;

	while (true)
	{
		//计算当前时间的帧率fcString，并写入当前帧
		pretime = curtime;
		curtime = clock();
		if ((curtime - pretime) != 0)
			frameCount = 1000 / (curtime - pretime);
		fcString.clear();
		while (frameCount / 10 != 0)
		{
			fcString = (char)((frameCount % 10) + 48) + fcString;
			frameCount /= 10;
		}
		fcString = (char)((frameCount % 10) + 48) + fcString;
		//std::cout << "帧率：" << fcString << "       ";
		capture >> frame;
		
		//cv::resize(frame, frame, cv::Size(frame.cols / 2, frame.rows / 2));

#ifdef DEBUG
		cv::putText(frame, fcString, cv::Point(50, 50), CV_FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0), 2, 2);
#endif // DEBUG

		cvtColor(frame, frameGray, CV_BGR2GRAY);
		equalizeHist(frameGray, frameGray);

		if (timeToDetectFace == intervalToDetectFace)
		{
			faceRect = detectFace(frameGray);
			record.close();
			record.open(recordFile, std::ofstream::app);
			if (faceRect.height == 0 && preFaceRect.height == 0)
			{
#ifdef DEBUG
				cv::imshow("detector", frame);
				cv::waitKey(10);
#endif // DEBUG
				continue;
			}
			else if (faceRect.height == 0 && preFaceRect.height != 0)
			{
				face = cv::Mat(frameGray, preFaceRect);
				faceRect = preFaceRect;
			}
			else if (faceRect.height != 0 && preFaceRect.height != 0)
			{
				if (abs(faceRect.area() - preFaceRect.area()) / preFaceRect.area() > 0.5)
				{
					face = cv::Mat(frameGray, preFaceRect);
					faceRect = preFaceRect;
				}
				else
				{
					face = cv::Mat(frameGray, faceRect);
					preFaceRect = faceRect;
				}
			}
			else
			{
				face = cv::Mat(frameGray, faceRect);
				preFaceRect = faceRect;
			}
			timeToDetectFace = 0;
		}
		++timeToDetectFace;
#ifdef DEBUG
		cv::rectangle(frame, faceRect, cv::Scalar(0, 255, 0), 3, 8, 0);
#endif // DEBUG

		int eyesCount = detectEyes(face);
		//std::cout << eyesCount << std::endl;
		if (eyesCount == 0)
		{
#ifdef DEBUG
			cv::imshow("detector", frame);
			cv::waitKey(10);
#endif // DEBUG
			//std::cout << "眨眼" << std::endl;
			if (preOneEye.height != 0)
			{
				//mask = cv::Mat::zeros(face.size(), CV_8UC1);
				//reCheckRoiCenter.x = preOneEye.x + preOneEye.width / 2;
				//reCheckRoiCenter.y = preOneEye.y + preOneEye.height / 2;
				//cv::circle(mask, reCheckRoiCenter, 0.2*preOneEye.width, cv::Scalar::all(255), -1);
				////cv::circle(frame, reCheckRoiCenter, 0.2*preFaceRect.width, cv::Scalar::all(255));
				//cv::floodFill(mask, reCheckRoiCenter, 255, NULL, cv::Scalar::all(0), CV_FLOODFILL_FIXED_RANGE);
				//face.copyTo(reCheckEye, mask);
				//cv::imshow("eye", reCheckEye);
				//cv::waitKey(10);
				//std::cout << "像素和：" << cv::sum(reCheckEye).val[0] / (0.2*preOneEye.width) << std::endl;
				//cv::imwrite(x + ".jpg", cv::Mat(face, preOneEye));
				//x += "1";
				double average = std::accumulate(grayDeque.begin(), grayDeque.end(), 0) / grayDeque.size();
				if (abs(pupilGrayscale(face, preOneEye) - average) / average > 0.5)
				{
					/*std::cout << "眨眼" << std::endl;
					std::cout << "差值：" << abs(pupilGrayscale(face, preOneEye) - average) << "average:" << average << std::endl;*/
					curtime_blink = clock();
					if (pretime_blink != 0 && abs(curtime_blink - pretime_blink) > 300)
					{
						std::cout << "眨眼" << x << std::endl;
						record << "眨眼";
						record << clock() << " ";
						++x;
						pretime_blink = curtime_blink;
					}
					else if(pretime_blink == 0)
					{
						std::cout << "眨眼" << x << std::endl;
						record << "眨眼";
						record << clock() << " ";
						++x;
						pretime_blink = curtime_blink;
					}
				}
			}
			continue;
		}
#ifdef DEBUG
		for (int i = 0; i < eyes.size(); ++i)
		{
			//cv::rectangle(frame, eyesRoi[i].first, eyesRoi[i].second, cv::Scalar(255, 255, 255), 3, 8, 0);
			cv::rectangle(frame, cv::Point(faceRect.x + eyes[i].x, faceRect.y + eyes[i].y),
				cv::Point(faceRect.x + eyes[i].x + eyes[i].width, faceRect.y + eyes[i].y + eyes[i].height),
				cv::Scalar(255, 255, 255), 3, 8, 0);
			grayDeque.push_back(pupilGrayscale(face, eyes[i]));
			if (grayDeque.size() > 50)
			{
				grayDeque.pop_front();
			}
		}
#endif // DEBUG

#ifdef DEBUG
		cv::imshow("detector", frame);
		cv::waitKey(10);
#endif // DEBUG
		//TODO
	}
	
}

double BlinkDetector::pupilGrayscale(cv::Mat face,cv::Rect roi)
{ 
	cv::Mat eyeRoi = cv::Mat(face, roi);
	cv::Mat binary = cv::Mat(eyeRoi.size(),CV_8UC1);
	//cv::Rect pupilArea = cv::Rect(cv::Point(roi.width*0.4, roi.height*0.4), cv::Point(roi.width*0.6, roi.height*0.6));
	cv::Rect pupilArea = cv::Rect(cv::Point(0, roi.height*0.3), cv::Point(roi.width, roi.height*0.7));
	cv::threshold(eyeRoi, binary, cv::sum(eyeRoi)[0] / roi.area(), 255, CV_THRESH_BINARY);
	binary = cv::Mat(binary, pupilArea);
#ifdef DEBUG
	cv::imshow("binary", binary);
	cv::waitKey(10);
#endif // DEBUG
	cv::Rect roiArea = cv::Rect(cv::Point(pupilArea.width*0.3, pupilArea.height*0.5 - 1), cv::Point(pupilArea.width*0.7, pupilArea.height*0.5 + 1));
	//return cv::sum(cv::Mat(binary, pupilArea))[0] / pupilArea.area();
	return cv::sum(cv::Mat(binary,roiArea))[0] / roiArea.area();
}

void BlinkDetector::setCascade(const std::string faceCascadeName, const std::string eyesCascadeName)
{
	if (!faceCascade.load(faceCascadeName))
		std::cout << "Load faceCascade failed!" << std::endl;
	if (!eyesCascade.load(eyesCascadeName))
		std::cout << "Load eyesCascade failed!" << std::endl;
}

cv::Rect BlinkDetector::detectFace(cv::Mat originImg)
{
	//cv::Mat faceOnly;
	//faceRoi.clear();
	faces.clear();
	faceCascade.detectMultiScale(originImg, faces, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
	if (faces.empty())
		return cv::Rect();
	//return faces[0];
	return cv::Rect(cv::Point(faces[0].x, faces[0].y + faces[0].height / 10), cv::Point(faces[0].x + faces[0].width, faces[0].y + faces[0].height * 3 / 5));

	//originImg(faces[0]).copyTo(faceOnly);
	/*preFaceRect = faces[0];
	faceRoi.clear();
	faceRoi.push_back(std::make_pair(cv::Point(faces[0].x, faces[0].y),
		cv::Point(faces[0].x + faces[0].width,faces[0].y + faces[0].height)));
	return faceOnly;*/
}

int BlinkDetector::detectEyes(cv::Mat face)
{
	//eyesRoi.clear();
	eyes.clear();
	//eyesCascade.detectMultiScale(face, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
	eyesCascade.detectMultiScale(face, eyes, 1.3, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
	if (!eyes.empty())
	{
		preOneEye = eyes[0];
	}

	//draw eyes

	/*if (eyes.empty())
		return 0;
	for (int i = 0; i < eyes.size(); ++i)
	{
		eyesRoi.push_back(std::make_pair(cv::Point(faceRoi[0].first.x + eyes[i].x, faceRoi[0].first.y + eyes[i].y)
			, cv::Point(faceRoi[0].first.x + eyes[i].x + eyes[i].width, faceRoi[0].first.y + eyes[i].y + eyes[i].height)));
	}*/
	/*cv::imshow("eye", cv::Mat(face, eyes[0]));
	cv::waitKey(30);*/
	return eyes.size();
}


void BlinkDetector::cameraInit()
{
	capture = cv::VideoCapture(0);
	if (!capture.isOpened())
	{
		std::cout << "Camera init failed !" << std::endl;
		exit(0);
	}
}




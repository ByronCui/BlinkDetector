#include <opencv2/objdetect/objdetect.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>  

#include <iostream>  
#include "BlinkDetector.h"

using namespace std;
using namespace cv;


int main()
{
	////Mat img = imread("womanWithGlasses.jpg");
	//Mat img = imread("open.jpg");
	//string faceCascadeName = "haarcascades\\haarcascade_frontalface_alt.xml";
	//string eyesCascadeName = "haarcascades\\haarcascade_eye_tree_eyeglasses.xml";
	//CascadeClassifier faceCascade;
	//CascadeClassifier eyesCascade;
	//if (!faceCascade.load(faceCascadeName))
	//	cout << "fail!" << endl;
	//cvtColor(img, img, CV_BGR2GRAY);
	//equalizeHist(img, img);
	//vector<Rect> faces;
	//Rect face;
	//faceCascade.detectMultiScale(img, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	//if (!faces.empty())
	//{
	//	cout << faces.size() << endl;
	//	face = faces[0];
	//	Mat roi(img, face);
	//	imshow("face", roi);
	//	waitKey(30);
	//}
	//else
	//	cout << "no face!" << endl;
	///*imshow("img", img);
	//waitKey(0);*/

	//if (!eyesCascade.load(eyesCascadeName))
	//	cout << "fail!" << endl;
	///*cvtColor(img, img, CV_BGR2GRAY);
	//equalizeHist(img, img);*/
	//vector<Rect> eyes;
	//Rect eye;
	//eyesCascade.detectMultiScale(img, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	//if (!eyes.empty())
	//{
	//	cout << eyes.size() << endl;
	//	eye = eyes[0];
	//	Mat roi(img, eye);
	//	imshow("eye", roi);
	//	waitKey(30);
	//}
	//else
	//	cout << "no face!" << endl;
	//imshow("img", img);
	//waitKey(0);




	//Mat open = imread("open.jpg",CV_8UC1);
	//Mat close = imread("close.jpg",CV_8UC1);
	//Mat openX = Mat(open.size(), CV_8UC1);
	//Mat closeX = Mat(close.size(), CV_8UC1);

	///*Rect upper = Rect(Point(open.cols*0.4, 0), Point(open.cols*0.6, open.rows*0.2));
	//Rect center = Rect(Point(open.cols*0.4, open.rows*0.4), Point(open.cols*0.6, open.rows*0.6));
	//int upperSum = cv::sum(Mat(open,upper))[0];
	//int centerSum = cv::sum(Mat(open, center))[0];
	//double threshold = (upperSum + centerSum) / upper.area();
	//cout << upperSum << "," << centerSum << endl;*/
	//cout << cv::sum(open)[0] / open.cols / open.rows << endl;
	//cout << cv::sum(close)[0] / close.cols / close.rows << endl;
	//
	////cv::adaptiveThreshold(open, openX, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 3, 5);
	//Rect center_open = Rect(Point(open.cols*0.4, open.rows*0.4), Point(open.cols*0.6, open.rows*0.6));
	//Rect center_close = Rect(Point(close.cols*0.4, close.rows*0.4), Point(close.cols*0.6, close.rows*0.6));
	//cv::threshold(open, openX, cv::sum(open)[0] / open.cols / open.rows, 255, CV_THRESH_BINARY);
	//cv::threshold(close, closeX, cv::sum(close)[0] / close.cols / close.rows, 255, CV_THRESH_BINARY);

	//cout << cv::sum(Mat(openX, center_open))[0] / center_open.area() << "," <<
	//	cv::sum(Mat(closeX, center_close))[0] / center_close.area();
	//cout << center_open.area() << "," << center_close.area();
	////cv::rectangle(openX, center_open, Scalar(0, 255, 0), 3, 8, 0);
	////cv::rectangle(closeX, center_close, Scalar(0, 255, 0), 3, 8, 0);

	//CascadeClassifier eyesCascade;
	//eyesCascade.load("haarcascades\\haarcascade_eye.xml");
	//vector<Rect> eyes;
	//eyesCascade.detectMultiScale(open, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	//if (!eyes.empty())
	//{
	//	cv::rectangle(open, eyes[0], Scalar(0, 255, 0), 3, 8, 0);
	//	cout << "open" << endl;
	//}
	//eyes.clear();
	//eyesCascade.detectMultiScale(close, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	//if (!eyes.empty())
	//{
	//	cv::rectangle(close, eyes[0], Scalar(0, 255, 0), 3, 8, 0);
	//	cout << "close" << endl;
	//}

	//imshow("closeX", closeX);
	//imshow("openX", openX);
	//imshow("open", open);
	//imshow("close", close);
	//waitKey(0);



	const string faceCascadeName = "haarcascades\\haarcascade_frontalface_alt.xml";
	const string eyesCascadeName = "haarcascades\\haarcascade_eye_tree_eyeglasses.xml";
	//const string eyesCascadeName = "haarcascades\\haarcascade_lefteye_2splits.xml";
	BlinkDetector *BD = new BlinkDetector(faceCascadeName, eyesCascadeName, 100);
	BD->start();
}
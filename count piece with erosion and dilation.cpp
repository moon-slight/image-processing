#include <opencv2/opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <vector>
using namespace std;
using namespace cv;

void main()
{
	IplImage *Im = cvLoadImage("IMG_5702.jpg", 0);
	cvSmooth(Im, Im, CV_MEDIAN, 5, 5, 0, 0); //Make sure noise won't let counting has error
	Mat img(Im, 0);
	resize(img, img, Size(img.cols/5, img.rows/5)); //Because image size too big
	Mat img2;
	img.copyTo(img2);
	imshow("Original", img);
	
	for (int x = 0; x < img.rows; x++) //Make image only has "black and white" two color
	{
		for (int y = 0; y < img.cols; y++)
		{
			if (img.at<uchar>(x, y) > 50)
				img2.at<uchar>(x, y) = 0;
			else
				img2.at<uchar>(x, y) = 255;
		}
	}
	imshow("Black and white", img2);
	
	//Do dilation
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	Mat element2 = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
	Mat dilateImg;
	dilate(img2, dilateImg, element2, cv::Point(-1, -1), 1);
	imshow("Dilation", dilateImg);
	
	//Do erosion several times
	Mat erodeImg;
	dilateImg.copyTo(erodeImg);
	erode(erodeImg, erodeImg, element, cv::Point(-1, -1), 16);
	imshow("Erosion", erodeImg);
	
	//Do canny
	Mat edge;
	Mat contoursImg = imread("IMG_5702.jpg", 1);
	resize(contoursImg, contoursImg, Size(contoursImg.cols/5, contoursImg.rows/5));
	blur(erodeImg, erodeImg, Size(3, 3));
	Canny(erodeImg, edge, 50, 150, 3);
	imshow("Canny", edge);
	
	//Plus blue circle on original image
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(edge, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
	int count = 0;
	for (int i = 0; i<contours.size(); i++) {
		if (hierarchy[i][2]<0)
		{
			Rect r = boundingRect(contours[i]);
			circle(contoursImg, Point(r.x, r.y), 10, Scalar(255, 0, 0), 3, 8, 0);
			count = count + 1;
		}
	}
	cout <<"Piece number is: "<< count << endl;
	imshow("Result", contoursImg);
	waitKey(0);
}
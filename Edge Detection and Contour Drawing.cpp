#include <opencv2/opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include<iostream>
#include<cmath>
using namespace std;
using namespace cv;

int main()
{
	Mat Image1 = imread("image.jpg", 0); //1 color
	Mat Image2,Image3;
	Image1.copyTo(Image2);
	Image1.copyTo(Image3);
	int threshold = 0;

	cout << "Please input threshold : ";
	cin >> threshold;
	
	for (int i = 1; i < Image1.cols - 1; i++)
	{
		for (int j = 1; j < Image1.rows - 1; j++)
		{
			//�� Sobel Operators ��@
			uchar result= abs(Image1.at<uchar>(i + 1, j - 1) + 2 * Image1.at<uchar>(i + 1, j) + Image1.at<uchar>(i + 1, j + 1) - Image1.at<uchar>(i - 1, j - 1) - 2 * Image1.at<uchar>(i - 1, j) - Image1.at<uchar>(i - 1, j + 1))
				+ abs(Image1.at<uchar>(i + 1, j + 1) + 2 * Image1.at<uchar>(i, j + 1) + Image1.at<uchar>(i - 1, j + 1) - Image1.at<uchar>(i - 1, j - 1) - 2 * Image1.at<uchar>(i, j - 1) - Image1.at<uchar>(i + 1, j - 1));
	
			//��t����
			Image2.at<uchar>(i, j) = (result >= threshold ? 255 : 0);
			Image3.at<uchar>(i, j) = result;
		}
	}

	imshow("output1", Image1);
	imshow("output2", Image2);
	imshow("output3", Image3);
	waitKey(0); //�Ȱ��{��������L��J
	return 0;
}
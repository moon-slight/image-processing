#include <opencv2/opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include<iostream>
using namespace std;
using namespace cv;

int main()
{
	Mat image = imread("image.jpg", 0); //1 color
	Mat new_image;
	image.copyTo(new_image);
	
	int matrix[256] = {0};
	float size = image.cols * image.rows;
	float value[256] = {0};
	float S[256] = {0};
	
	for (int x = 0; x < image.cols; x++) //store all pixel value of image
	{
		for (int y = 0; y < image.rows; y++)
			value[image.at<uchar>(x, y)]++;
	}

	S[0] = value[0] / size;
	for (int i = 1; i < 256; i++) //do histogram equalization
	{
		S[i] = S[i - 1] + value[i] / size;
		matrix[i] = (uchar)255. * S[i];
	}
	
	for (int x = 0; x < image.cols; x++) //put new value into new image
	{
		for (int y = 0; y < image.rows; y++)
			new_image.at<uchar>(x, y) = matrix[image.at<uchar>(x, y)];
	}

	imshow("output", image);
	imshow("output2", new_image);
	waitKey(0);
	return 0;
}
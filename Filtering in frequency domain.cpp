#include <opencv2/opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include<iostream>
#include <math.h> 
#include <vector>
using namespace std;
using namespace cv;

const double pi = 3.141592653;
double ** Calculate(int, double);

int main()
{
	Mat image = imread("image.jpg", 0);
	Mat output;
	image.copyTo(output);
	
	int length = 5; //kernel size
	double weight = 10; //stantard deviation
	
	cout << "Please input stantard deviation: ";
	cin >> weight;
	double(**Mask) = Calculate(length, weight);
	
	int begin = (length - 1) / 2;

	for (int i = 0; i < output.rows; i++)
		for (int i2 = 0; i2 < output.cols; i2++)
			output.at<uchar>(i, i2) = 0;
	
	for (int x = begin; x < image.cols-begin; x++)
	{
		for (int y = begin; y < image.rows-begin; y++)
		{
			for (int i = -1 * begin; i <= begin; i++)
			{
				for (int i2 = -1 * begin; i2 <= begin; i2++)
					output.at<uchar>(x, y) += image.at<uchar>(x + i, y + i2) * Mask[i + begin][i2 + begin]; //put mask onto original image
			}
		}
	}

	imshow("Image", image);
	imshow("Output", output);
	waitKey(0);
	return 0;
}

double ** Calculate(int length, double weight) //build mask
{

	double(**Kernel) = new double*[length];
	for (int i = 0; i < length; i++)
		Kernel[i] = new double[length];
	
	double sumTotal = 0;
	int kernelRadius = length / 2;
	double distance = 0;

	double calculatedEuler = 1.0 / (2.0 * pi * pow(weight, 2));

	for (int filterY = -kernelRadius; filterY <= kernelRadius; filterY++) 
	{
		for (int filterX = -kernelRadius; filterX <= kernelRadius; filterX++) 
		{
			distance = ((filterX * filterX) + (filterY * filterY)) / (2 * (weight * weight));
			Kernel[filterY + kernelRadius][filterX + kernelRadius] = calculatedEuler * exp(-distance);
			sumTotal += Kernel[filterY + kernelRadius][filterX + kernelRadius];
		}
	}

	for (int y = 0; y < length; y++) 
	{
		for (int x = 0; x < length; x++)
			Kernel[y][x] = Kernel[y][x] * (1.0 / sumTotal);
	}

	return Kernel;
}
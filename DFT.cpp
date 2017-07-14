#include "opencv2/core/core.hpp"
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>
using namespace cv;
using namespace std;

const float pi = 3.141592653;

void Dft(Mat input, Mat output) //do dft without library function
{
	Mat tmp;
	input.copyTo(tmp);

	for (float x = 0; x < output.cols; x++)
	{
		for (float y = 0; y < output.rows; y++)
		{
			float sum_x = 0, sum_y = 0;
			for (float input_x = 0; input_x < output.cols; input_x++)
			{
				for (float input_y = 0; input_y < output.rows; input_y++)
				{
					sum_x += (float)tmp.at<Vec2f>(input_x, input_y)[0] * cos(2 * pi*((float)(x* input_x / output.cols) + (float)(y* input_y / output.rows)));
					sum_y += (float)tmp.at<Vec2f>(input_x, input_y)[0] * sin(2 * pi*((float)(x* input_x / output.cols) + (float)(y* input_y / output.rows)));
				}
			}
			output.at<Vec2f>(x, y)[0] = sum_x;
			output.at<Vec2f>(x, y)[1] = sum_y;
		}
	}
}

int main(int argc, char **argv)
{
	Mat image = imread("right arrow.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (image.empty())
		return -1;

	Mat expand; //expand input image to optimal size
	int m = getOptimalDFTSize(image.rows);
	int n = getOptimalDFTSize(image.cols); //on the border add zero values
	copyMakeBorder(image, expand, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[] = { Mat_<float>(expand), Mat::zeros(expand.size(), CV_32F) };
	Mat complexI;
	merge(planes, 2, complexI); //Add to the expanded another plane with zeros

	dft(complexI, complexI); //this way the result may fit in the source matrix

	//compute the magnitude and switch to logarithmic scale
	//log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))

	split(complexI, planes); //planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
	magnitude(planes[0], planes[1], planes[0]);//planes[0] = magnitude
	Mat magI = planes[0];

	magI += Scalar::all(1); //switch to logarithmic scale
	log(magI, magI);

	// crop the spectrum, if it has an odd number of rows or columns
	magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

	//rearrange the quadrants of Fourier image  so that the origin is at the image center
	int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	Mat q0(magI, Rect(0, 0, cx, cy)); //Top-Left - Create a ROI per quadrant
	Mat q1(magI, Rect(cx, 0, cx, cy)); //Top-Right
	Mat q2(magI, Rect(0, cy, cx, cy)); //Bottom-Left
	Mat q3(magI, Rect(cx, cy, cx, cy));//Bottom-Right

	Mat tmp; //swap quadrants (Top-Left with Bottom-Right)

	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp); //swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);

	normalize(magI, magI, 0, 1, CV_MINMAX); //Transform the matrix with float values into a
											//viewable image form (float between values 0 and 1).

	imshow("Input Image", image); //Show the result
	imshow("dft", magI);


	Mat image2 = imread("right arrow.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (image2.empty())
		return -1;

	Mat expand2;
	int m2 = getOptimalDFTSize(image2.rows);
	int n2 = getOptimalDFTSize(image2.cols);
	copyMakeBorder(image2, expand2, 0, m2 - image2.rows, 0, n2 - image2.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes2[] = { Mat_<float>(expand2), Mat::zeros(expand2.size(), CV_32F) };
	Mat complexI2;
	merge(planes2, 2, complexI2);

	Dft(complexI2, complexI2);

	split(complexI2, planes2);
	magnitude(planes2[0], planes2[1], planes2[0]);
	Mat magI2 = planes2[0];

	magI2 += Scalar::all(1);
	log(magI2, magI2);

	magI2 = magI2(Rect(0, 0, magI2.cols & -2, magI2.rows & -2));

	int cx2 = magI2.cols / 2;
	int cy2 = magI2.rows / 2;

	Mat q02(magI2, Rect(0, 0, cx2, cy2));
	Mat q12(magI2, Rect(cx2, 0, cx2, cy2));
	Mat q22(magI2, Rect(0, cy2, cx2, cy2));
	Mat q32(magI2, Rect(cx2, cy2, cx2, cy2));

	Mat tmp2;
	q02.copyTo(tmp2);
	q32.copyTo(q02);
	tmp2.copyTo(q32);

	q12.copyTo(tmp2);
	q22.copyTo(q12);
	tmp2.copyTo(q22);

	normalize(magI2, magI2, 0, 1, CV_MINMAX);

	imshow("Dft", complexI2);
	waitKey();

	return 0;
} 
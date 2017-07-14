#include <opencv2/opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include<iostream>
#include<math.h>
using namespace std;
using namespace cv;

Mat Image_temp;
Mat Image;

String algorithm[3] = {"(Nearest Neighbor Interpolation)", "(Bilinear Interpolation)", "(Bicubic Interpolation)"};

Mat Rotation(Mat &, int);
Mat Algorithm1(double);
Mat Algorithm2(double);
Mat Algorithm3(double);
Mat select(int, double);

void menu();

int main()
{
	Image = imread("image.jpg", 1); //1 color
	
	menu();

	cout << "Thanks for your using.\n";
	waitKey(0); //等待鍵盤輸入，用來暫停程式。
	system("pause");
	return 0;
}

void menu()
{
	int mode = 0;
	cout << "Please input a number to select\n " << algorithm[0] << ": 1  " <<  algorithm[1] << ": 2  " << algorithm[2] << ": 3\n or input other number to exit" << endl;
	cin >> mode;
	
	if (mode == 1 || mode == 2 || mode == 3)
	{
		double rate = 1;
		int degree = 0;
		cout << "Please input scaling rate (not less than 0)： ";
		cin >> rate;
		
		if (rate >= 0)
		{
			cout << "Please input rotation degree (counterclockwise)： ";
			cin >> degree;
			
			if (degree >= 1 && degree <= 360)
			{
				cout << "computing";
				Mat temp;
				temp = Rotation(select(mode, rate), degree);
				imshow(algorithm[mode - 1], temp);
				imwrite("Output.jpg", temp);
				waitKey(1000);
				menu();
			}
		}
	}
}

Mat select(int mode, double rate)
{
	if (mode == 1)
		return Algorithm1(rate);
	else if (mode == 2)
		return Algorithm2(rate);
	else 
		return Algorithm3(rate);
}

Mat Rotation(Mat &input, int degree)
{
	double result = (degree * 3.1415926 / 180);
	double result_back = (-1 * degree * 3.1415926 / 180);
	Mat temp = Mat::zeros(input.cols*fabs(sin(result)) + input.rows*fabs(cos(result)), input.rows*fabs(sin(result)) + input.cols*fabs(cos(result)), CV_8UC3);
	int x_min = 0, y_min = 0;
	
	for (int x = 0; x < input.cols; x++)
	{
		for (int y = 0; y < input.rows; y++)
		{
			int x_pos = (int)((x)*cos(result) - (y)*sin(result));
			int y_pos = (int)((x)*sin(result) + (y)*cos(result));
			if (x_pos < x_min)
				x_min = x_pos;
			if (y_pos < y_min)
				y_min = y_pos;
		}
	}
	
	x_min = (x_min > 0 ? 0 : abs(x_min));
	y_min = (y_min > 0 ? 0 : abs(y_min));
	
	for (int x = 0; x < temp.cols; x++)
	{
		for (int y = 0; y < temp.rows; y++)
		{
			int x_pos = (int)((x - x_min)*cos(result_back) - (y - y_min)*sin(result_back));
			int y_pos = (int)((x - x_min)*sin(result_back) + (y - y_min)*cos(result_back));
			bool check_x = true, check_y = true;
			
			if (x_pos > input.cols || x_pos < -1)
				check_x = false;
			else if (x_pos == input.cols)
				x_pos--;
			else if (x_pos == -1)
				x_pos++;
			
			if (y_pos >= input.rows || y_pos < -1)
				check_y = false;
			else if (y_pos == input.rows)
				y_pos--;
			else if (y_pos == -1)
				y_pos++;
			
			if (check_x && check_y)
				temp.at<Vec3b>(x, y) = input.at<Vec3b>(x_pos, y_pos);
		}
	}
	cout << "\nComplete！\n\n\n";
	return temp;
}

Mat Algorithm1(double rate) //Nearest Neighbor Interpolation
{
	if (rate > 0)
	{
		Image_temp.create((int)(Image.cols * rate + 0.5), (int)(Image.rows * rate + 0.5), CV_8UC3);
		
		for (int y = 0; y < Image_temp.rows; y++)
		{
			for (int x = 0; x < Image_temp.cols; x++)
			{
				int resource_x, resource_y;
				resource_x = ((int)(x / rate + 0.5) < Image.cols ? (int)(x / rate + 0.5) : (int)(x / rate + 0.5) - 1); //避免四捨五入後out of range
				resource_y = ((int)(y / rate + 0.5) < Image.rows ? (int)(y / rate + 0.5) : (int)(y / rate + 0.5) - 1); //避免四捨五入後out of range
				
				Image_temp.at<Vec3b>(x, y)[0] = Image.at<Vec3b>(resource_x, resource_y)[0];
				Image_temp.at<Vec3b>(x, y)[1] = Image.at<Vec3b>(resource_x, resource_y)[1];
				Image_temp.at<Vec3b>(x, y)[2] = Image.at<Vec3b>(resource_x, resource_y)[2];
			}
		}
	}
	return Image_temp;
}

Mat Algorithm2(double rate) //Bilinear Interpolation
{
	if (rate > 0)
	{
		Image_temp.create((int)(Image.cols * rate + 0.5), (int)(Image.rows * rate + 0.5), CV_8UC3);
		for (int y = 0; y < Image_temp.rows; y++)
		{
			int resource_y = (int)(y / rate);
			float y_pos;
			
			if (resource_y == Image.rows - 1)
			{
				resource_y--;
				y_pos = (float)(y / rate) - 1 - resource_y;
			}
			else
				y_pos = (float)(y / rate) - resource_y;
			
			for (int x = 0; x < Image_temp.cols; x++)
			{
				int resource_x = (int)(x / rate);
				float x_pos;
				
				if (resource_x == Image.cols - 1)
				{
					resource_x--;
					x_pos = (float)(x / rate) - 1 - resource_x;
				}
				else
					x_pos = (float)(x / rate) - resource_x;
				
				for (int n = 0; n < 3; n++)
				{
					Image_temp.at<Vec3b>(x, y)[n] = Image.at<Vec3b>(resource_x, resource_y)[n] * (1 - x_pos)*(1 - y_pos)
						+ Image.at<Vec3b>(resource_x + 1, resource_y + 1)[n] * (x_pos)*(y_pos)
						+Image.at<Vec3b>(resource_x, resource_y + 1)[n] * (1 - x_pos)*(y_pos)
						+Image.at<Vec3b>(resource_x + 1, resource_y)[n] * (x_pos)*(1 - y_pos);
				}
			}
		}
	}
	return Image_temp;
}

float BiCubicPoly(float x)
{
	float abs_x = abs(x);
	float a = -0.5;
	
	if (abs_x <= 1.0)
		return (a + 2)*pow(abs_x, 3) - (a + 3)*pow(abs_x, 2) + 1;
	else if (abs_x < 2.0)
		return a*pow(abs_x, 3) - 5 * a*pow(abs_x, 2) + 8 * a*abs_x - 4 * a;
	else
		return 0.0;
}

Mat Algorithm3(double rate) //Bicubic Interpolation
{
	if (rate > 0)
	{
		Image_temp.create((int)(Image.cols * rate + 0.5), (int)(Image.rows * rate + 0.5), CV_8UC3);
		
		for (int y = 0; y < Image_temp.rows; y++)
		{
			int resource_y = (int)(y / rate) - 1;
			
			if (resource_y >= Image.rows - 3)
				resource_y = Image.rows - 4;
			
			if (resource_y < 0)
				resource_y = 0;
			
			for (int x = 0; x < Image_temp.cols; x++)
			{
				int resource_x = (int)(x / rate) - 1;
				
				if (resource_x >= Image.cols - 3)
					resource_x = Image.cols - 4;

				if (resource_x < 0)
					resource_x = 0;

				for (int k = 0; k < 3; k++)
				{
					Image_temp.at<Vec3b>(x, y)[k] = 0;
					
					for (int n = 0; n < 4; n++)
					{
						for (int n2 = 0; n2 < 4; n2++)
						{
							float x_ = BiCubicPoly(n), y_ = BiCubicPoly(n2);
							Image_temp.at<Vec3b>(x, y)[k] += Image.at<Vec3b>(resource_x + n, resource_y + n2)[k] * x_*y_;
						}
					}
				}
			}
		}
	}
	return Image_temp;
}
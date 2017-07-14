#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat image ,temp;
	int pixel_value = 0;

	image = imread("image.jpg", CV_LOAD_IMAGE_COLOR);
	temp = imread("image.jpg", CV_LOAD_IMAGE_COLOR);

	if(!image.data) // Check for invalid input
	{
		cout << "Could not open or find the image" << endl ;
		system("pause");
		return -1;
	}

	// let pixel_value between 0 ~ 255
	if(pixel_value > 255)
		pixel_value = 255;

	else if (pixel_value < 0)
		pixel_value = 0;

	image.convertTo(temp, -1, 1, pixel_value); // change pixel value
	
	namedWindow("OpenCV", CV_WINDOW_AUTOSIZE); // Create a window for display
	
	imshow("OpenCV", image); // Show our image inside it
	
	waitKey(0); // Wait for a keystroke in the window
	
	return 0;
}
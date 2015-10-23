#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

using namespace std;
using namespace cv;

Mat image;
Mat image_gray;
Mat image_gray2;
Mat threshold_output;
Mat frame;
int thresh = 220, max_thresh = 255;


int main(int argc, char** argv)
{
ofstream myfile;
myfile.open ("data.csv");
myfile << "10" ;
//Capture Video
VideoCapture capCam(1);
if (!capCam.isOpened())
{
cout << "ERROR: Failed to Initialize Camera" << endl;
return 1;
}
else
{
cout << "Camera Initialized" << endl;
}

//Create Window

char* OutputFootage = "Processed";
namedWindow(OutputFootage, CV_WINDOW_AUTOSIZE);

while (1)
{
capCam >> frame;
//imshow(ImputFootage, frame);

if (capCam.read(frame))
{

//Convert Image to gray
cvtColor(frame, image_gray, CV_BGR2GRAY);

//Threshold Gray Image
threshold(image_gray, threshold_output, thresh, max_thresh, THRESH_BINARY);

//2D Container
vector<vector<Point> > contours;

//Fnd Countours Points, (Imput Image, Storage, Mode1, Mode2, Offset??)
findContours(threshold_output, contours, // a vector of contours
CV_RETR_EXTERNAL, // retrieve the external contours
CV_CHAIN_APPROX_NONE, Point(0, 0)); // all pixels of each contours

// Draw black contours on a white image
Mat result(threshold_output.size(), CV_8U, Scalar(255));
drawContours(result, contours, -1, // draw all contours
Scalar(0), // in black
2); // with a thickness of 2
for (unsigned int i = 0; i < contours.size(); i++)
		{
			double area = cv::contourArea(contours[i]);
			cv::Rect rect = cv::boundingRect(contours[i]);	
			int radius = rect.width/2;
			cv::Size s= frame.size();
			int h = s.height;
			int w = s.width;
			if (area >= h*w*0.005 && //Size of the contour
				std::abs(1 - ((double)rect.width / (double)rect.height)) <= 0.5 &&//If the countour is circular
					std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.5)	
					//If the contour is big and circular then it is pupil
			{
				std::ostringstream strs;
				strs << (radius*2);
				std::string str = strs.str();
				myfile <<","<< str ;
				cv::putText(frame, str, cv::Point(h/10, w/10), CV_FONT_HERSHEY_SIMPLEX, 1, CV_RGB(100,255,0), 4);
				cv::circle(frame, cv::Point(rect.x + radius, rect.y + radius), radius, CV_RGB(100,255,0), 2);//overlay a circle on pupil
				//if (area1 > 5){
				//	cv::putText(result, "Pupil is dilated", cv::Point(h/10, w/10+40), CV_FONT_HERSHEY_SIMPLEX, 1, CV_RGB(100,255,0), 1);
				//}
				
			}
		}
imshow(OutputFootage, result);
char CheckForEscKey = waitKey(10);

//If the key pressed by user is Esc(ASCII is 27) then break out of the loop
if (CheckForEscKey == 27)
{
break;
myfile.close();
}

}
}

return 0;
}

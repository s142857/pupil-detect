#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cmath>
#include <opencv/cv.h>
#include <string>
#include <opencv2/core/core.hpp>
#include <iostream>

int main(int argc, char** argv)
{
	// Take image from file
	cv::Mat src = cv::imread(argv[1]);
	if (src.empty())
		return -1;

	cv::Mat gray; //Declare image matrix
	cv::cvtColor(~src, gray, CV_BGR2GRAY);  //grayscale and invert image

	cv::threshold(gray, gray, 220, 255, cv::THRESH_BINARY); //thresholding
	
	std::vector<std::vector<cv::Point> > contours;
	cv::findContours(gray.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //find Contours
	cv::imshow("image", src);
	
	cv::drawContours(gray, contours, -1, CV_RGB(255,255,255), -1); //If there are any gaps in the contours they are fllled

	for (unsigned int i = 0; i < contours.size(); i++)
	{
		double area = cv::contourArea(contours[i]);
		cv::Rect rect = cv::boundingRect(contours[i]);
		int radius = rect.width/2;

		cv::Size s= src.size();
		int h = s.height;
		int w = s.width;
		if (area >= h*w*0.005 && //Size of the contour
		    std::abs(1 - ((double)rect.width / (double)rect.height)) <= 0.2 &&//If the countour is circular
				std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2)	
				//If the contour is big and circular then it is pupil
		{
			double area1 = (CV_PI * std::pow(radius, 2) * 100)/(h*w); //calculate area of pupil (for future release
			printf("\npupil area %lfpercent of total area \n",area1); //print area of pupil
			std::ostringstream strs;
			strs << (radius*2);
			std::string str = strs.str();
			cv::putText(src, str, cv::Point(50, 50), CV_FONT_HERSHEY_SIMPLEX, 1, CV_RGB(100,255,0), 4);
			cv::circle(src, cv::Point(rect.x + radius, rect.y + radius), radius, CV_RGB(100,255,0), 2);//overlay a circle on pupil
			if (area1 > 5){
				printf("pupils are dilated \n \n");
			}
			
		}
	}

	cv::imshow("image", src); //show image
	cv::waitKey(0);
	
	return 0;
}

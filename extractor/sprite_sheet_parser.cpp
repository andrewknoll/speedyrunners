#include <stdio.h>
#include <string>
#include <list>
#include <opencv2/opencv.hpp>

using namespace cv;

void extractSprites(std::string filename) {

	Mat bufferedImage = imread(filename, 1);
	Mat image = imread(filename, 1);
	Mat gray;
	Mat thresh;
	Mat close;
	Mat dilate;
	Mat hierarchy;
	std::list<Mat> contours = mutableListOf<MatOfPoint>()

	cvtColor(image, gray, COLOR_BGR2GRAY)
	adaptiveThreshold(gray, thresh, 255.0,
		ADAPTIVE_THRESH_GAUSSIAN_C,
		THRESH_BINARY_INV, 5, 1.0)
	Mat kernel = getStructuringElement(MORPH_RECT,
		Size(1.0, 1.0))
	morphologyEx(thresh, close,MORPH_CLOSE,
		kernel, Point(-1.0, -1.0), 2)
	dilate(close, dilate, kernel,
		Point(-1.0, -1.0), 1)
	findContours(dilate, contours, hierarchy,
		RETR_EXTERNAL, CHAIN_APPROX_SIMPLE)

	int index = 0;
	for(auto it : contours){
		Rect rect = boundingRect(it)
		Mat sprite(bufferedImage, Rect);
		std::string output = filename + index;

		imwrite(output, sprite);
		index++;
	}
}
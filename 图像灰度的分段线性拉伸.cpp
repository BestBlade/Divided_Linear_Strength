#include <iostream>
#include<opencv2/core/core.hpp>  
#include<opencv2/highgui/highgui.hpp>  
#include"opencv2/imgproc/imgproc.hpp"
#include <stdio.h>

using namespace std;
using namespace cv;

int matchArray(float array[], float target) {
	float val = 500;
	for (int i = 0; i < 256; i++) {
		float temp = abs(array[i] - target);
		if (val >= temp) {
			val = temp;
			continue;
		}
		else {
			return i;
		}
	}
	return -1;
}

Mat dividedLinearStrength(Mat img) {
	int img_cnt[256] = { 0 };
	for (int x = 0; x < img.rows; x++) {
		for (int y = 0; y < img.cols; y++) {
			img_cnt[img.at<uchar>(x, y)]++;
		}
	}
	float img_cdf[256] = { 0 };
	for (int i = 0; i < 256; i++) {
		img_cdf[i] = img_cnt[i] / float(img.rows * img.cols);
		if (i == 0) {
			continue;
		}
		img_cdf[i] += img_cdf[i - 1];
	}

	int start = matchArray(img_cdf, 0.05);
	int end = matchArray(img_cdf, 0.95);

	Mat res(img.rows, img.cols, img.type());

	for (int x = 0; x < res.rows; x++) {
		for (int y = 0; y < res.cols; y++) {
			if (img.at<uchar>(x, y) < start) {
				res.at<uchar>(x, y) = 0;
			}
			else if (img.at<uchar>(x, y) >= end) {
				res.at<uchar>(x, y) = 255;
			}
			else {
				res.at<uchar>(x, y) = (img.at<uchar>(x, y) - start) * 243 / (end - start) + 1;
			}
		}
	}
	return res;
}
int main() {
	Mat img = imread("C://Users//Chrysanthemum//Desktop//1.png",0);

	Mat res = dividedLinearStrength(img);
	//

	//

	imshow("origin pic", img);
	imshow("result pic", res);
	waitKey();
}
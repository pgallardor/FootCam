#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(){
	VideoCapture cap(0);
	if (!cap.isOpened()) return -1;

	cout << "LMAOOO" << endl;

	namedWindow("video", 1);
	Mat frames;
	while(true){
		cap >> frames;
		imshow("video", frames);
		waitKey(1);
		//if (waitKey(30) >= 0) break;
	}
	return 0;
}

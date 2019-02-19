#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cstdio>

using namespace cv;
using namespace std;

int main(){
        bool opened = false;
	VideoCapture  cap;
	for (int i = 0; i < 20; i++){
	  cap = VideoCapture(i);
	  if (!cap.isOpened()){
	    printf("Error opening device %d\n", i);
	    continue;
	  }
	  opened = true;
	  break;
	}
	//VideoCapture cap(0);
	//if (!cap.isOpened()) return -1;
	if (!opened){
	  cout << "AYYYYY" << endl;
	  return -1;
	}
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

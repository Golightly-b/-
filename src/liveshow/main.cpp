#include "opencv2\highgui.hpp"
#include <string>
#include <iostream>
using namespace cv;
using namespace std;

int main(int argc, char*argv) {
	VideoCapture cam;
	string url = "";
	namedWindow("video");
	Mat frame;
	if (cam.open(0)) {
		cout << "open cam success" << endl;
	}
	else {
		cout << "open cam failed" << endl;
		waitKey(0);
		return -1;
	}
	for (;;) {
		cam.read(frame);
		imshow("video", frame);
		waitKey(1);
	}
	return 0;
}
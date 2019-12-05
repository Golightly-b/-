#include "opencv2\highgui.hpp"
#include <string>
#include <iostream>
using namespace cv;
using namespace std;

int main(int argc, char*argv) {
	VideoCapture cam;
	string url = "rtmp://192.1681.1.11/live";
	namedWindow("video");
	Mat frame;
	cam.open(0);
	try
	{
		if (!cam.isOpened()) {
			throw exception("cam open failed");
		}
		cout << "cam open success" << endl;
		for (;;) {
			//读取rtsp视频帧，解码视频帧
			if (!cam.grab()) {
				continue;
			}
			//yuv转换为rgb
			if (!cam.retrieve(frame)) {
				continue;
			}
			imshow("video", frame);
			waitKey(1);
		}
	}
	catch (exception &ex) {
		cerr << ex.what() << endl;
	}
	getchar();
	return 0;
}
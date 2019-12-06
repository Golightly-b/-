#include "opencv2\highgui.hpp"
#include <string>
#include <iostream>

extern "C"
{
#include "libswscale\swscale.h"
#include "libavcodec\avcodec.h"
#include "libavformat\avformat.h"
}
#pragma comment(lib, "swscale.lib")
//#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avutil.lib")
//#pragma comment(lib, "avformat.lib")
//#pragma comment(lib,"opencv_world320.lib")

using namespace cv;
using namespace std;

int main(int argc, char*argv) {
	VideoCapture cam;
	string url = "rtmp://192.1681.1.117/live";
	namedWindow("video");
	Mat frame;
	cam.open(0);
	SwsContext *vsc = NULL;//头文件libswscale\swscale.h 库文件swscale.lib
	AVFrame *yuv = NULL;
	try
	{
		if (!cam.isOpened()) {
			throw exception("cam open failed");
		}
		cout << "cam open success" << endl;
		int inWidth = cam.get(CAP_PROP_FRAME_WIDTH);
		int inHegitht = cam.get(CAP_PROP_FRAME_HEIGHT);

		//初始化格式转换上下文
		vsc = sws_getCachedContext(vsc,
			inWidth, inHegitht, AV_PIX_FMT_BGR24,
			inWidth, inHegitht, AV_PIX_FMT_YUV420P,
			SWS_BICUBIC,
			0, 0, 0);
		if (!vsc) {
			throw exception("sws_getCachedContext failed");
		}
		//输出的数据结构
		yuv = av_frame_alloc();//头文件avcodec.lib 
		yuv->format = AV_PIX_FMT_YUV420P;
		yuv->width = inWidth;
		yuv->height = inHegitht;
		yuv->pts = 0;
		int ret = av_frame_get_buffer(yuv, 32);
		if (!ret) {
			char buf[1024] = { 0 };
			av_strerror(ret, buf, sizeof(buf) - 1);//TODO 无法解析的外部符号
			throw exception(buf);
		}
		
		 
		

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
			//rgb转换为yuv
			//输入的数据格式
			uint8_t *indata[AV_NUM_DATA_POINTERS] = { 0 };//bgrbgr   plane indata[0] bbbbb indata[1]  gggg
			indata[0] = frame.data;
			int insize[AV_NUM_DATA_POINTERS] = { 0 };//一行数据的字节数
			insize[0] = frame.cols * frame.elemSize();
			int h = sws_scale(vsc, indata, insize, 0, frame.rows,
				yuv->data, yuv->linesize);
			if (h <= 0) {
				continue;
			}
			cout << h << "" << flush;
		}
	}
	catch (exception &ex) {
		if (cam.isOpened())
			cam.release();
		if (vsc) {
			sws_freeContext(vsc);
			vsc = NULL;
		}
		cerr << ex.what() << endl;
	}
	getchar();
	return 0;
}
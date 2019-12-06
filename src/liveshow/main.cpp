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
	SwsContext *vsc = NULL;//ͷ�ļ�libswscale\swscale.h ���ļ�swscale.lib
	AVFrame *yuv = NULL;
	try
	{
		if (!cam.isOpened()) {
			throw exception("cam open failed");
		}
		cout << "cam open success" << endl;
		int inWidth = cam.get(CAP_PROP_FRAME_WIDTH);
		int inHegitht = cam.get(CAP_PROP_FRAME_HEIGHT);

		//��ʼ����ʽת��������
		vsc = sws_getCachedContext(vsc,
			inWidth, inHegitht, AV_PIX_FMT_BGR24,
			inWidth, inHegitht, AV_PIX_FMT_YUV420P,
			SWS_BICUBIC,
			0, 0, 0);
		if (!vsc) {
			throw exception("sws_getCachedContext failed");
		}
		//��������ݽṹ
		yuv = av_frame_alloc();//ͷ�ļ�avcodec.lib 
		yuv->format = AV_PIX_FMT_YUV420P;
		yuv->width = inWidth;
		yuv->height = inHegitht;
		yuv->pts = 0;
		int ret = av_frame_get_buffer(yuv, 32);
		if (!ret) {
			char buf[1024] = { 0 };
			av_strerror(ret, buf, sizeof(buf) - 1);//TODO �޷��������ⲿ����
			throw exception(buf);
		}
		
		 
		

		for (;;) {
			//��ȡrtsp��Ƶ֡��������Ƶ֡
			if (!cam.grab()) {
				continue;
			}
			//yuvת��Ϊrgb
			if (!cam.retrieve(frame)) {
				continue;
			}
			imshow("video", frame);
			waitKey(1);
			//rgbת��Ϊyuv
			//��������ݸ�ʽ
			uint8_t *indata[AV_NUM_DATA_POINTERS] = { 0 };//bgrbgr   plane indata[0] bbbbb indata[1]  gggg
			indata[0] = frame.data;
			int insize[AV_NUM_DATA_POINTERS] = { 0 };//һ�����ݵ��ֽ���
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
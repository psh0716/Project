#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

string haar_xml = "haarcascade_fullbody.xml";
string hog_xml = "hogcascade_pedestrians.xml";
CascadeClassifier haar;
CascadeClassifier hog;

int main()
{
	VideoCapture cam("test.avi");
	Mat frame, gray, roi;
	vector<Rect> found, detect, detect2;

	int diff = 8;
	int x0, x1, y0, y1;

	// ���� ũ�⿡ ���� width, height ����
	int width = cam.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = cam.get(CV_CAP_PROP_FRAME_HEIGHT);;

	// ���� �ӵ� ���� ����
	__int64 freq, start, finish;
	double fps;
	::QueryPerformanceFrequency((_LARGE_INTEGER*)&freq);

	haar.load(haar_xml);
	hog.load(hog_xml);

	// ���� ��ü�� ũ�� ����
	Size min_obj_sz(36, 72);
	Size max_obj_sz(100, 200);

	/* �ǽð� ķ ������ ������ ����
	cam.set(CAP_PROP_FRAME_WIDTH, 640);
	cam.set(CAP_PROP_FRAME_HEIGHT, 480);
	cam.isOpened();
	*/

	while (cam.read(frame))	// ���� �� ������ �б�
	{
		// ���� �ӵ� ������ ���� ī��Ʈ ����
		::QueryPerformanceCounter((_LARGE_INTEGER*)&start);

		// gray scale ��ȯ �� ������׷� ��Ȱȭ
		cvtColor(frame, gray, CV_BGR2GRAY);
		equalizeHist(gray, gray);

		// 1������ haar ���� ����
		// �Է¿��� / ����ȿ��� / ������ / �ĺ��� / 0 / �ּҹ�üũ��/ �ִ빰üũ��
		haar.detectMultiScale(gray, found, 1.1, 1, 0, min_obj_sz, max_obj_sz);

		for (int i = 0; i < found.size(); i++)
		{
			// roi ���� ����
			found[i].x - diff > 0 ? x0 = found[i].x - diff : x0 = 0;
			found[i].x + found[i].width + diff < width ? x1 = found[i].x + found[i].width + diff : x1 = width;
			found[i].y - diff > 0 ? y0 = found[i].y - diff : y0 = 0;
			found[i].y + found[i].height + diff < height ? y1 = found[i].y + found[i].height + diff : y1 = height;

			roi = frame(Range(y0, y1), Range(x0, x1));
			
			// roi ������ ���� ��Ȯ���� ���� Haar ���� ����
			haar.detectMultiScale(roi, detect2, 1.05, 4, 0, min_obj_sz, max_obj_sz);

			if (detect2.size())
			{
				rectangle(roi, detect2[0], Scalar(0, 255, 0), 2);	// ���� ���� ǥ��
			}
			else
			{
				// roi ������ ���� HOG ���� ����
				hog.detectMultiScale(roi, detect, 1.05, 3, 0, min_obj_sz, max_obj_sz);

				if (detect.size())	// roi ���� HOG ����
				{
					rectangle(roi, detect[0], Scalar(0, 255, 0), 2); // ���� ���� ǥ��
				}
			}

			::QueryPerformanceCounter((_LARGE_INTEGER*)&finish); // ������ ��ġ�� �ð� ����
		}

		// ���� �ӵ� ��� �� ǥ��
		fps = freq / double(finish - start);
		char fps_str[20];
		sprintf_s(fps_str, 20, "FPS: %.1lf", fps);
		putText(frame, fps_str, Point(5, 35), FONT_HERSHEY_SIMPLEX, 1., Scalar(0, 255, 0), 2);

		// ��� ���� ������ ���
		imshow("Result", frame);
		if (waitKey(5) == 27) break; // ESCŰ ������ ����
	}

	return 0;
}

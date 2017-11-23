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

	// 영상 크기에 따라 width, height 설정
	int width = cam.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = cam.get(CV_CAP_PROP_FRAME_HEIGHT);;

	// 검출 속도 측정 변수
	__int64 freq, start, finish;
	double fps;
	::QueryPerformanceFrequency((_LARGE_INTEGER*)&freq);

	haar.load(haar_xml);
	hog.load(hog_xml);

	// 검출 물체의 크기 설정
	Size min_obj_sz(36, 72);
	Size max_obj_sz(100, 200);

	/* 실시간 캠 설정시 사이즈 설정
	cam.set(CAP_PROP_FRAME_WIDTH, 640);
	cam.set(CAP_PROP_FRAME_HEIGHT, 480);
	cam.isOpened();
	*/

	while (cam.read(frame))	// 영상 한 프레임 읽기
	{
		// 검출 속도 측정을 위한 카운트 시작
		::QueryPerformanceCounter((_LARGE_INTEGER*)&start);

		// gray scale 변환 및 히스토그램 평활화
		cvtColor(frame, gray, CV_BGR2GRAY);
		equalizeHist(gray, gray);

		// 1차적인 haar 검출 진행
		// 입력영상 / 검출된영역 / 스케일 / 후보수 / 0 / 최소물체크기/ 최대물체크기
		haar.detectMultiScale(gray, found, 1.1, 1, 0, min_obj_sz, max_obj_sz);

		for (int i = 0; i < found.size(); i++)
		{
			// roi 영역 설정
			found[i].x - diff > 0 ? x0 = found[i].x - diff : x0 = 0;
			found[i].x + found[i].width + diff < width ? x1 = found[i].x + found[i].width + diff : x1 = width;
			found[i].y - diff > 0 ? y0 = found[i].y - diff : y0 = 0;
			found[i].y + found[i].height + diff < height ? y1 = found[i].y + found[i].height + diff : y1 = height;

			roi = frame(Range(y0, y1), Range(x0, x1));
			
			// roi 영역에 대해 정확도를 높인 Haar 검출 진행
			haar.detectMultiScale(roi, detect2, 1.05, 4, 0, min_obj_sz, max_obj_sz);

			if (detect2.size())
			{
				rectangle(roi, detect2[0], Scalar(0, 255, 0), 2);	// 검출 영역 표시
			}
			else
			{
				// roi 영역에 대해 HOG 검출 진행
				hog.detectMultiScale(roi, detect, 1.05, 3, 0, min_obj_sz, max_obj_sz);

				if (detect.size())	// roi 영역 HOG 검출
				{
					rectangle(roi, detect[0], Scalar(0, 255, 0), 2); // 검출 영역 표시
				}
			}

			::QueryPerformanceCounter((_LARGE_INTEGER*)&finish); // 검출을 마치는 시간 측정
		}

		// 검출 속도 계산 및 표시
		fps = freq / double(finish - start);
		char fps_str[20];
		sprintf_s(fps_str, 20, "FPS: %.1lf", fps);
		putText(frame, fps_str, Point(5, 35), FONT_HERSHEY_SIMPLEX, 1., Scalar(0, 255, 0), 2);

		// 결과 영상 프레임 출력
		imshow("Result", frame);
		if (waitKey(5) == 27) break; // ESC키 누르면 종료
	}

	return 0;
}

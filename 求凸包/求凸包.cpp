#include "stdafx.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int thresh = 80;
int max_thresh = 255;
RNG rng(12345);


struct D2POINT
{
	int x; int y;
};

char path[32] = "F:\\data\\010";
/// Function header
void thresh_callback(int, void*, char fullpath[32], char childpath[10]);
void potoName(char fullpath[32], char childpath[10], int FrameNO1);
void SaveImg(char fullpath[32], char childpath[10]);
/** @function main */
int main(int argc, char** argv)
{
	/// Load source image and convert it to gray
	//src = imread("06.jpg", 1);

	///// Convert image to gray and blur it
	//cvtColor(src, src_gray, CV_BGR2GRAY);
	//blur(src_gray, src_gray, Size(3, 3));

	///// Create Window
	//char* source_window = "Source";
	//namedWindow(source_window, CV_WINDOW_AUTOSIZE);
	//imshow(source_window, src);

	///*createTrackbar(" Threshold:", "Source", &thresh, max_thresh, thresh_callback);*/
	//thresh_callback(0, 0);
	char fullpath[32] = "";
	char childpath[10] = "\\0.jpg";
	char copychildpath[10] = "\\0.jpg";

	for (int i = 1; i < 400; i++){

		strcpy(fullpath, path);
		potoName(fullpath,childpath,i);
		strcpy(copychildpath, childpath);

		src = imread(fullpath, 1);
		if (src.data == 0)
			continue;
		/// Convert image to gray and blur it
		cvtColor(src, src_gray, CV_BGR2GRAY);
		blur(src_gray, src_gray, Size(3, 3));

		/*createTrackbar(" Threshold:", "Source", &thresh, max_thresh, thresh_callback);*/
		thresh_callback(0, 0, fullpath, copychildpath);
	}
	

	waitKey(0);
	return(0);
}
//int function(D2POINT p,int &key){
//	//
//	static D2POINT PArr;
//	static D2POINT Offset[8];
//	int val = 0; 
//	int max = 0; int temp = 0;
//	if (PArr.x == 0&&PArr.y==0){//第一次保存
//		key = 0;
//		PArr.x = p.x;
//		PArr.y = p.y;
//		return val;
//	}
//	else{
//		//求两帧图像的偏移量
//		Offset[key].x = p.x - PArr.x;
//		Offset[key].y = p.y - PArr.y;
//		//数据更新
//		PArr.x = p.x;
//		PArr.y = p.y;
//		key++;
//	}
//	
//	//jisuan
//	for (int i= 0,j = i + 1; j < key; j++)
//	{
//		if (Offset[i].x*Offset[j].y < 0){
//
//			temp = abs(Offset[i].x) + abs(Offset[j].y);
//		}
//		else{
//			temp = Offset[i].x>Offset[j].x ? Offset[i].x - Offset[j].x : Offset[j].x - Offset[i].x;
//		}
//		if (max < temp)
//			max = temp;
//	}
//	//如果多帧位移差
//	if (max > 40){
//			
//		for (int i = 0; i < key; i++){
//				
//			Offset[i].x = 0;
//			Offset[i].y = 0;
//		}
//		key = 0;
//	}
//	
//	
//	return key;
//
//}
void potoName(char fullpath[32], char childpath[10], int FrameNO1){

	if (FrameNO1<10)
	{
		childpath[0] = '\\';
		childpath[1] = 'c';//数字转字符
		childpath[2] = FrameNO1 + '0';
		childpath[3] = '.';
		childpath[4] = 'j';
		childpath[5] = 'p';
		childpath[6] = 'g';
		childpath[7] = '\0';
	}
	else{

		if (FrameNO1<100){

			if (FrameNO1 == 10){
				childpath[0] = '\\';
				childpath[1] = 'c';
				childpath[2] = '1';
				childpath[3] = '0';
				childpath[4] = '.';
				childpath[5] = 'j';
				childpath[6] = 'p';
				childpath[7] = 'g';
				childpath[8] = '\0';
			}
			else{
				childpath[3] = FrameNO1 % 10 + '0';
				childpath[2] = FrameNO1 / 10 + '0';
			}

		}
		else{

			if (FrameNO1 == 100){
				childpath[0] = '\\';
				childpath[1] = 'c';
				childpath[2] = '1';
				childpath[3] = '0';
				childpath[4] = '0';
				childpath[5] = '.';
				childpath[6] = 'j';
				childpath[7] = 'p';
				childpath[8] = 'g';
				childpath[9] = '\0';
			}
			else{
				childpath[4] = (FrameNO1 % 100) % 10 + '0';
				childpath[3] = (FrameNO1 % 100) / 10 + '0';
				childpath[2] = FrameNO1 / 100 + '0';
			}

		}
	}
	strcat(fullpath, childpath);//获得路径名称

}


/** @function thresh_callback */
void thresh_callback(int, void*,char fullpath[32],char childpath[10])
{
	Mat src_copy = src.clone();
	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using Threshold
	threshold(src_gray, threshold_output, thresh, 255, THRESH_BINARY);

	/// Find contours
	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	//double  HS = cvContourArea(cvArr*( contours), CV_WHOLE_SEQ);
	//double  HL = cvArcLength(contours, CV_WHOLE_SEQ, -1);
	/// Find the convex hull object for each contour
	vector<vector<Point> >hull(contours.size());
	// Int type hull
	vector<vector<int>> hullsI(contours.size());
	// Convexity defects
	vector<vector<Vec4i>> defects(contours.size());

	for (size_t i = 0; i < contours.size(); i++)
	{
		convexHull(Mat(contours[i]), hull[i], false);//false:逆时针
		//void convexHull(InputArray points, OutputArray hull, bool clockwise=false, bool returnPoints=true);
		// find int type hull
		convexHull(Mat(contours[i]), hullsI[i], false);
		// get convexity defects
		convexityDefects(Mat(contours[i]), hullsI[i], defects[i]);

	}

	/// Draw contours + hull results
	Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
	//cvContourArea((cvArr)drawing, CV_WHOLE_SEQ);
	for (size_t i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());//轮廓
		drawContours(drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point());//凸包


		// draw defects
		size_t count = contours[i].size();
		std::cout << "Count : " << count << std::endl;
		if (count < 200)
			continue;

		vector<Vec4i>::iterator d = defects[i].begin();

		while (d != defects[i].end()) {
			Vec4i& v = (*d);
			//if(IndexOfBiggestContour == i)
			{

				int startidx = v[0];
				Point ptStart(contours[i][startidx]); // point of the contour where the defect begins
				int endidx = v[1];
				Point ptEnd(contours[i][endidx]); // point of the contour where the defect ends
				int faridx = v[2];
				Point ptFar(contours[i][faridx]);// the farthest from the convex hull point within the defect
				int depth = v[3] / 256; // distance between the farthest point and the convex hull

				if (depth > 18 && depth < 90)
				{
					line(drawing, ptStart, ptFar, CV_RGB(0, 255, 0), 2);
					line(drawing, ptEnd, ptFar, CV_RGB(255, 0, 0), 2);
					circle(drawing, ptStart, 4, Scalar(255, 0, 0), 2);
					circle(drawing, ptEnd, 4, Scalar(255, 255, 255), 2);
					circle(drawing, ptFar, 4, Scalar(0, 0, 255), 2);
					printf("start(%d,%d) end(%d,%d), far(%d,%d)\n",	ptStart.x, ptStart.y, ptEnd.x, ptEnd.y, ptFar.x, ptFar.y);
				}

				
			}
			d++;
		}


	}

	/// Show in a window
	namedWindow("Hull demo", CV_WINDOW_AUTOSIZE);
	imshow("Hull demo", drawing);
	SaveImg(fullpath, childpath);
	imwrite(fullpath, drawing);
}
void SaveImg(char fullpath[32], char childpath[10]){

	strcpy(fullpath, path);
	bool val = false;
	for (int i = 0; i != 8; i++){

		if ('c'==childpath[i]){
			childpath[i] = 'd';
			break;
		}
	
	}
	
	strcat(fullpath, childpath);//获得路径名称

}
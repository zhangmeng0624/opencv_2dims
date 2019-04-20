
#include<opencv2\imgcodecs.hpp>
#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\imgproc.hpp>
#include<vector>
#include<time.h>
#include"IPClocation.h"
#include<iostream>
using namespace cv;
using namespace std;




/*
	*@brief：画表示物体方向的箭头
	@param  img:图片
	@param	pLocation:物体位置点
	@param	pDirection:物体方向向量
	@param	oPoint:坐标中心点的像素坐标
	@param	len:箭头长度
	@param	len:箭头角度
	@returns：void
	*/
void drawArrow(cv::Mat& img, cv::Point2d pLocation, cv::Point2d pDirection, Point2d oPoint, int len, int alpha,
	cv::Scalar color, int thickness = 2, int lineType = 8)
{
	const double PI = 3.1415926;
	Point2d arrow;

	//计算 θ 角
	double angle = atan2((double)(oPoint.y - pDirection.y), (double)(oPoint.x - pDirection.x));

	pLocation.x = pLocation.x - sqrt(2) * 4 * cos(angle);
	pLocation.y = pLocation.y - sqrt(2) * 4 * sin(angle);
	//计算箭角边的另一端的端点位置
	arrow.x = pLocation.x + len * cos(angle + PI * alpha / 180);
	arrow.y = pLocation.y + len * sin(angle + PI * alpha / 180);
	line(img, pLocation, arrow, color, thickness, lineType);
	arrow.x = pLocation.x + len * cos(angle - PI * alpha / 180);
	arrow.y = pLocation.y + len * sin(angle - PI * alpha / 180);
	line(img, pLocation, arrow, color, thickness, lineType);
}

/*
	*@brief：画坐标轴的箭头
	@param  img:图片
	@param	pLocation:位置点
	@param	pDirection:箭头方向向量
	@param	oPoint:坐标中心点的像素坐标
	@param	len:箭头长度
	@param	len:箭头角度
	@returns：void
	*/
void drawCoorArrow(cv::Mat& img, cv::Point2d pLocation, cv::Point2d pDirection, Point2d oPoint, int len, int alpha,
	cv::Scalar color, int thickness = 2, int lineType = 8)
{
	const double PI = 3.1415926;
	Point2d arrow;

	//计算 θ 角
	double angle = atan2((double)(oPoint.y - pDirection.y), (double)(oPoint.x - pDirection.x));

	//计算箭角边的另一端的端点位置
	arrow.x = pLocation.x + len * cos(angle + PI * alpha / 180);
	arrow.y = pLocation.y + len * sin(angle + PI * alpha / 180);
	line(img, pLocation, arrow, color, thickness, lineType);
	arrow.x = pLocation.x + len * cos(angle - PI * alpha / 180);
	arrow.y = pLocation.y + len * sin(angle - PI * alpha / 180);
	line(img, pLocation, arrow, color, thickness, lineType);
}


/*
	*@brief：在二维坐标系中显示机器人的坐标和方向以及物体的坐标
	@param  vector<IPCobj>:物体类型类向量
	@param	center:坐标原点
	@param	scale:刻度,即1m对应图片的像素值
	@returns：Mat:显示的图片
	*/
Mat paintObject(vector<IPCobj> input, Point2d center = Point2d(400, 300), float scale = 50)
{
	vector<Point2d> points ;
	vector<Point2d>	directions;
	
	for (int i = 0; i < input.size(); i++)
	{
		points.push_back(Point2d(input[i].coordinate3D[0], input[i].coordinate3D[1]));
		directions.push_back(Point2d(input[i].direction3D[0], input[i].direction3D[1]));
	}
	double rows = center.y * 2; //图片的行
	double cols = center.x * 2;

	if (scale<10)
	{
		scale = 10;
	}
	if (scale > 500)
	{
		scale = 500;
	}
	Mat back(rows, cols, CV_8UC3, Scalar(255, 255, 255));
	Point2d oPoint = center; //中心点坐标
	vector<Point2d> xPoint; //x轴坐标
	vector<Point2d> yPoint;
	for (int i = -5; i <= 5; i++)
	{
		xPoint.push_back(Point2d(oPoint.x + i * scale, oPoint.y));
	}

	for (int i = -5; i <= 5; i++)
	{
		yPoint.push_back(Point2d(oPoint.x, oPoint.y + i * scale));
	}
	//sort(yPoint.begin(), yPoint.end(), cmp); //降序排列
	reverse(yPoint.begin(),yPoint.end());

	Point2d xEnd = Point2d(xPoint[10].x + 20, xPoint[10].y); //x轴终点
	Point2d yEnd = Point2d(yPoint[10].x, yPoint[10].y - 20); //y轴终点
	line(back, xPoint[0], xEnd, 0); //画出x轴
	drawCoorArrow(back, xEnd, Point2d(scale * 1 + cols / 2, -scale * 0 + rows / 2), oPoint, 5, 45, Scalar(0, 0, 0), 1);//画x轴的箭头

	line(back, yPoint[0], yEnd, 0);//画出y轴
	drawCoorArrow(back, yEnd, Point2d(scale * 0 + cols / 2, -scale * 1 + rows / 2), oPoint, 5, 45, Scalar(0, 0, 0), 1);//画y轴的箭头

		//显示x，y轴的坐标值
	char text[11];
	for (int i = -5; i <= 5; i++)
	{
		if (i == 0)
		{
			continue;
		}
		if (scale <= 20)
		{
			if (i % 2 == 1 or i % 2 == -1)
			{
				sprintf_s(text, "%d", i); //格式化输出
				putText(back, text, xPoint[i + 5], CV_FONT_HERSHEY_COMPLEX, 0.3, Scalar(0, 0, 255));
			}

		}
		else
		{
			sprintf_s(text, "%d", i); //格式化输出
			putText(back, text, xPoint[i + 5], CV_FONT_HERSHEY_COMPLEX, 0.4, Scalar(0, 0, 255));
		}

	}
	for (int i = -5; i <= 5; i++)
	{
		if (scale <= 20)
		{
			if (i % 2 == 1 or i % 2 == -1)
			{
				sprintf_s(text, "%d", i); //格式化输出
				putText(back, text, yPoint[i + 5], CV_FONT_HERSHEY_COMPLEX, 0.3, Scalar(0, 0, 255));
			}
		}
		else
		{
			sprintf_s(text, "%d", i);
			putText(back, text, yPoint[i + 5], CV_FONT_HERSHEY_COMPLEX, 0.4, Scalar(0, 0, 255));
		}

	}

	//画坐标网格
	for (int i = 0; i < xPoint.size(); i++)
	{
		if (scale >= 180)
		{
			if (i < xPoint.size() - 1)
			{
				line(back, Point2d(xPoint[i].x + 0.25*scale, xPoint[i].y - 5 * scale), Point2d(xPoint[i].x + 0.25*scale, xPoint[i].y + 5 * scale), Scalar(125, 125, 125), 1);
				line(back, Point2d(xPoint[i].x + 0.5*scale, xPoint[i].y - 5 * scale), Point2d(xPoint[i].x + 0.5*scale, xPoint[i].y + 5 * scale), Scalar(125, 125, 125), 1);
				line(back, Point2d(xPoint[i].x + 0.75*scale, xPoint[i].y - 5 * scale), Point2d(xPoint[i].x + 0.75*scale, xPoint[i].y + 5 * scale), Scalar(125, 125, 125), 1);
			}
			line(back, Point2d(xPoint[i].x, xPoint[i].y - 5 * scale), Point2d(xPoint[i].x, xPoint[i].y + 5 * scale), Scalar(125, 125, 125), 1);
		}
		if (scale >= 60 and scale<180)
		{
			if (i < xPoint.size() - 1)
			{
				line(back, Point2d(xPoint[i].x + 0.5*scale, xPoint[i].y - 5 * scale), Point2d(xPoint[i].x + 0.5*scale , xPoint[i].y + 5 * scale), Scalar(125, 125, 125), 1);
			}
			line(back, Point2d(xPoint[i].x, xPoint[i].y - 5 * scale), Point2d(xPoint[i].x, xPoint[i].y + 5 * scale), Scalar(125, 125, 125), 1);
		}
		if (scale > 20 and scale <60)
		{
			line(back, Point2d(xPoint[i].x, xPoint[i].y - 5 * scale), Point2d(xPoint[i].x, xPoint[i].y + 5 * scale), Scalar(125, 125, 125), 1);
		}
		if (scale <= 20)
		{
			if (i % 2 == 0 )
			{
				line(back, Point2d(xPoint[i].x, xPoint[i].y - 5 * scale), Point2d(xPoint[i].x, xPoint[i].y + 5 * scale), Scalar(125, 125, 125), 1);
			}
		}
	}
	for (int i = 0; i < yPoint.size(); i++)
	{
		if (scale >= 180)
		{
			if (i < yPoint.size() - 1)
			{
				line(back, Point2d(yPoint[i].x - 5 * scale, yPoint[i].y - 0.25*scale), Point2d(yPoint[i].x + 5 * scale, yPoint[i].y - 0.25*scale), Scalar(125, 125, 125), 1);
				line(back, Point2d(yPoint[i].x - 5 * scale, yPoint[i].y - 0.5*scale), Point2d(yPoint[i].x + 5 * scale, yPoint[i].y - 0.5*scale), Scalar(125, 125, 125), 1);
				line(back, Point2d(yPoint[i].x - 5 * scale, yPoint[i].y - 0.75*scale), Point2d(yPoint[i].x + 5 * scale, yPoint[i].y - 0.75*scale), Scalar(125, 125, 125), 1);
			}
			line(back, Point2d(yPoint[i].x - 5 * scale, yPoint[i].y), Point2d(yPoint[i].x + 5 * scale, yPoint[i].y), Scalar(125, 125, 125), 1);
		}
		if (scale >= 60 and scale< 180)
		{
			if (i < yPoint.size() - 1)
			{
				line(back, Point2d(yPoint[i].x - 5 * scale, yPoint[i].y - 0.5*scale), Point2d(yPoint[i].x + 5 * scale, yPoint[i].y - 0.5*scale), Scalar(125, 125, 125), 1);
			}
			line(back, Point2d(yPoint[i].x - 5 * scale, yPoint[i].y), Point2d(yPoint[i].x + 5 * scale, yPoint[i].y), Scalar(125, 125, 125), 1);
		}
		if (scale > 20 and scale < 60)
		{
			line(back, Point2d(yPoint[i].x - 5 * scale, yPoint[i].y), Point2d(yPoint[i].x + 5 * scale, yPoint[i].y), Scalar(125, 125, 125), 1);
		}
		if (scale <= 20)
		{
			if (i % 2 == 0)
			{
				line(back, Point2d(yPoint[i].x - 5 * scale, yPoint[i].y), Point2d(yPoint[i].x + 5 * scale, yPoint[i].y), Scalar(125, 125, 125), 1);
			}
		}
	}
	   


	//显示物体
	for (int i = 0; i < points.size(); i++)
	{
		int id = input[i].ID;
		//显示机器人的信息
		if (input[i].cls == 1)
		{
			circle(back, Point2d(scale * points[i].x + cols / 2, -scale * points[i].y + rows / 2), 4, Scalar(0, 0, 255), -1); //显示位置
			drawArrow(back, Point2d(scale * points[i].x + cols / 2, -scale * points[i].y + rows / 2), Point2d(scale * directions[i].x + cols / 2, -scale * directions[i].y + rows / 2),
				oPoint, 10, 45, Scalar(255, 0, 0)); //显示方向，用箭头表示
			//显示id
			string str;
			stringstream ss;
			ss << id;
			ss >> str;
			putText(back, str, Point2d(scale * points[i].x + cols / 2 - 8, -scale * points[i].y + rows / 2 - 10), CV_FONT_HERSHEY_COMPLEX, 0.4, Scalar(0, 0, 255));
		}
		//显示其他物体的位置
		else
		{
			rectangle(back, Point2d(scale * points[i].x + cols / 2 - 5, -scale * points[i].y + rows / 2 - 5),
				Point2d(scale * points[i].x + cols / 2 + 5, -scale * points[i].y + rows / 2 + 5), Scalar(0, 255, 0), -1); //显示位置
		}
	}
	
	//imshow("back", back);
	//imwrite("7.jpg", back);
	//waitKey(500);
	return back;
}

int main()
{
	vector<IPCobj> input;
	IPCobj c;
	c.cls = IPCobj::Robot;
	c.coordinate3D = Vec3d(3, 4, 0);
	c.direction3D = Vec3d(1, 1, 0);
	c.ID = 11;

	IPCobj d;
	d.cls = IPCobj::object;
	d.coordinate3D = Vec3d(-3, -3, 0);
	d.ID = 0;
	d.direction3D = Vec3d(-1, 1, 0);

	IPCobj e;
	e.cls = IPCobj::Robot;
	e.coordinate3D = Vec3d(-3, 4, 0);
	e.direction3D = Vec3d(-3, -4, 0);
	e.ID = 22;

	IPCobj f;
	f.cls = IPCobj::object;
	f.coordinate3D = Vec3d(1, -4.9, 0);
	f.ID = 0;
	f.direction3D = Vec3d(0, 0, 0);
	input.push_back(c);
	input.push_back(d);
	input.push_back(e);
	input.push_back(f);
	Point2d center = Point2d(400, 400);
	int scale = 80;
	/*while (1)
	{
		paintObject(input, center, scale);
		scale = scale + 5;
	}*/
	/*time_t start, stop;
	time(&start);
	paintObject(input, center, scale);
	time(&stop);
	double runtime =(double)( stop - start);
	cout << "运行时间:" << runtime << endl;*/

	double timeStart = (double)getTickCount();
	paintObject(input, center, scale);
	double nTime = ((double)getTickCount() - timeStart) / getTickFrequency();
	cout << "运行时间:" << nTime << endl;
	while (1);
	return 0;
}
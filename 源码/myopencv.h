#pragma once
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include<math.h>
#include<vector>
//�Ҷ�ͼ�任
#define PI 3.1415926
struct MYLINE {
	float k;
	float b;
	int count;
};

class myopencv
{
private:
	int Count[250][1000] = { 0 };	//�ۼ�����
	int Laplace1[3][3] = { 0, -1, 0, -1, 4, -1, 0, -1, 0 };//laplaceģ��,4����  
	int Laplace3[3][3] = { -1, 0, -1, 0, 4, 0, -1, 0, -1 };//laplaceģ��,4����  
	int Laplace2[3][3] = { -1, -1, -1, -1, 8, -1, -1, -1, -1 };//laplaceģ��,8���� 
	int erode_kernel[3][3] = { 0,1,0,1,1,1,0,1,0 };//��ʴ��
public:
	//ֱ��ͼ���⻯-->ͼ����ǿ
	void HistEqual(cv::Mat inputImg, cv::Mat outputImg);
	//��ɫͼ���Ҷ�ͼ
	void ColorToGray(cv::Mat inputImg, cv::Mat outputImg);
	//��ָ�˲�-->ȥ��
	void Median(cv::Mat inputImg, cv::Mat outputImg);
	//�Ҷ�ͼ����ֵͼ
	void GrayToBinary(cv::Mat inputImg, cv::Mat outputImg);
	void GrayToBinary_OSTU(cv::Mat inputImg, cv::Mat outputImg);
	//��ȡ��Ե
	void GetEdge(cv::Mat inputImg, cv::Mat outputImg);
	//��ȡROI��ͼƬ�°벿�֣�
	void mask(cv::Mat inputImg, cv::Mat outputImg);
	//��ȡ��Ҫֱ�ߣ������ߣ� ���ؽṹ���������洢ֱ����Ϣ
	std::vector<MYLINE> GetLines(cv::Mat& inputImg, cv::Mat& outputImg);
	//���ӻ�������
	void ShowLines(cv::Mat& inputImg, cv::Mat& outputImg);
	//��ʴ����
	void erode(cv::Mat inputImg, cv::Mat outputImg);
};


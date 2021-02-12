
#include <fstream>
#include <json\value.h>
#include<json/reader.h>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc.hpp>
#include"myopencv.h"
using namespace std;
bool FlagToShow;
/*
Lane�������ڳ����߼�⣬����Ϊ�����ͼƬ·�������Ϊ�ṹ������
���裺
1.ת���Ҷ�ͼ
2.��ֵ�˲�
3.��ֵ��
4.��ʴ
5.��ȡ��Ե
6.����任
7.���ӻ�
*/
std::vector<MYLINE> Lane(string  path) {
	
	myopencv myop;
	std::vector<MYLINE>lines;
	cv::Mat frame = cv::imread(path);
	if (frame.empty()) {
		return lines;
	}
	else {
		cv::Mat img_gray(frame.size(), CV_8UC1);
		cv::Mat img_gray2(frame.size(), CV_8UC1);
		cv::Mat img_denoise(frame.size(), CV_8UC1);
		cv::Mat img_edges(frame.size(), CV_8UC1);
		cv::Mat img_mask(frame.size(), CV_8UC1);
		cv::Mat img_lines(frame.size(), CV_8UC1);
		
		myop.ColorToGray(frame, img_gray);

		//myop.HistEqual(img_gray, img_gray2);
	
		myop.Median(img_gray, img_gray2);
	
		myop.GrayToBinary_OSTU(img_gray2, img_denoise);

		myop.erode(img_denoise, img_gray);
		img_denoise = img_gray;
	
		myop.GetEdge(img_denoise, img_edges);
	
		myop.mask(img_edges, img_mask);
		
		lines=myop.GetLines(img_mask, img_lines);
		
		if (FlagToShow) {
			myop.ShowLines(frame, img_lines);
			cv::imshow(path, frame);
			cv::waitKey(1000);
		}
		
		
		
	}
	return lines;
}
//��С�����ʱ��Ϊ10
bool isInLine(MYLINE line, cv::Point p) {
	if (fabs(p.x - line.k * p.y - line.b) < 10) return true;
	else return false;
}
int main()
{
	//1.��Json�ļ�
	ifstream in("E:\\groundtruth.json");
	if (!in.is_open())
	{
		std::cout << "Error opening file\n";
		return 0;
	}

	//2.ѡ���Ƿ���ӻ�
	std::cout << "�Ƿ���ӻ����?y/n" << endl;
	if (getchar() == 'y')FlagToShow = true;
	else FlagToShow = false;
	int test_line = 100;//���ͼƬ��
	string readline;
	float rate[100] = {0};//��¼100��ͼƬ��ÿ��ͼ��������
	if(FlagToShow)cv::namedWindow("Windows", CV_WINDOW_AUTOSIZE);
	//���ж�ȡ�ļ�
	while (getline(in,readline)&&test_line)
	{
		//��һ���ж�ȡ����  
		Json::Reader reader;
		Json::Value root;
		if (reader.parse(readline.c_str(), root))
		{
			string name = root["raw_file"].asString();
			//ͼƬ�ļ�ֻ��0531��0601
			if (name.find("/0531/") != string::npos|| name.find("/0601/") != string::npos) {
				std::vector<MYLINE>lines= Lane(name);
				if (!lines.empty()) {
					std::cout << name << endl;
					int total_count = 0;//�ܵļ���
					int count = 0;//��Ч����
					for (int i = 0; i < 4; i++) {
						for (int j = 0; j < root["lanes"][(Json::Value::UInt)i].size(); j++) {
								cv::Point point;
								point.x = root["lanes"][(Json::Value::UInt)i][(Json::Value::UInt)j].asInt();
								point.y = root["h_samples"][(Json::Value::UInt)j].asInt();
								if (point.x != -2) {
									total_count++;
									for (int x = 0; x < lines.size(); x++) {
										//��ֵ���ڼ���������ʱ
										if (isInLine(lines[x], point)) {
										count++;
										break;
										}
									}
								}
						}
						;
					}
					//3.���ÿ��ͼ�������Ϣ
					std::cout << "count:" << count << " " << "total count:" << total_count << endl;
					rate[100-test_line] =  (float)count / (float)total_count;
					std::cout <<"NO."<< 100-test_line<<"  "<<"acc:" << rate[100 - test_line] << endl;
					test_line--;
					std::cout << endl;
				}
				
			}
		
		}
		
	}
	//4.����ƽ��������
	float total_rate = 0;
	for (int i = 0; i < 100; i++) {
		total_rate += rate[i];
	}
	total_rate /= 100-test_line;
	cout << "total rate:" << total_rate << endl;

    return 0;
}

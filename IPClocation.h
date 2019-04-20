/*
IPClocation
˵  ����һ����RTSP��ַIP����ͷ�����ж�λ�Ŀ⣬�ṩ�˶�λ�ӿ�
�����ṩ��һ����Ϣ��IPCobj��������Ϊ��Ϣ������洢���ڳ������ж���
���岢�Ҹ���������λ�á�
�����ˣ�����ǿ
��  ����beta 0.1
*/

#pragma once
#include <opencv2\aruco.hpp>
#include "opencv2/opencv.hpp"
#include <time.h>
#include <vector>
#include <stdio.h>
#include <string>
#include <io.h>
//#include "zfun.h"
#include <windows.h>


using namespace cv;



//����һ���������ͣ���¼�洢������Ϣ
class IPCobj
{
public:
	IPCobj()
	{};

	enum objclass
	{
		Robot = 1,     //������
		object = 2	 //��Ҫץȡ��Ŀ��
	};

	//����
	objclass cls;//������ʲô��

	Vec3d coordinate3D; //�����3Dλ��
	Vec3d direction3D;  //�����3D����
	
	uint8_t ID; //������ʱrobot������ID�ǡ�

	uint8_t dimension; //����ά�� 3������3D��������ϵ�£�2���������2D����ϵ�£�4����3D��������ϵ����ʱ�����

	Point2i coordinate2D; //�����2Dλ��

	double time;//4Dά���²���

private:

};

//����һ��IPC���ͣ����ڴ洢IPC��Ϣ
class IPCmsg
{
public:
	IPCmsg();
	IPCmsg(String addr);
	//����
	String rtsp;  //���IPC��rtsp��ַ
	Mat cameraMatrix; //����ڲ�
	Mat distCoeffs;  //����������
	Mat RwMatrix, TwVec, RwMatrixI; //�����ת�����λ�ƾ����Լ���ת����������
	VideoCapture cap;//��� Open()�ɹ� ���Ϳ��Է����������
	//IPC״̬����
	enum StatusEnum
	{
		CANNOT_OPEN = -1, //���ܴ�����ͷ
		NORMAL = 1,
		NOSTAR=0,
	};
	
private:
	
	StatusEnum Status;//�洢���Ŀǰ��״̬

public: //��������ӿ�
	/*
	@brief:���������������Ϳ�����VideoCapture��
	@returns:������ȷ
	*/
	bool Open();

	/*
	@brief:������״̬
	@returns:����Status
	*/
	StatusEnum checkStatus();
	/*
	@brief:������ת����������
	*/
	void updateRwMatrixI();

};


/*--------------------------------------------------------------
@brief������һ��IPC��λ�࣬���Ľӿ���,ͨ��ͼƬ�е�ARtag�������ж�λ
---------------------------------------------------------------*/
class IPClocation
{
public:
	IPClocation();
	//����
	std::vector<IPCmsg> IPC; //����IPC
private:
	
	//���ڽ�����������ϵ�ı궨AR��ID�����Ĵ�С
	uint8_t worldAR_ID=11; //11����Ϊ��������ϵ�궨ARtag
	float worldAR_size= 0.1784;	   // ��Ϊ��λ 0.5

	//AR������
	aruco::PREDEFINED_DICTIONARY_NAME ARtag_dict = aruco::DICT_6X6_250;
	//AR����������Լ�size
	//��AR_ID��Ϊ2�����䣬ÿ��������3��ֵ��IDȡֵ��2�� ��AR��Ĵ�Сsizeһ��(��λmm)
	uint16_t AR_ID_distribList[2][3] = {
		{20,60,178},	//��Ż����˵ĳߴ�
		{80,150,100}	//С�Ż����˵ĳߴ�
	};

	//�洢�������
	String xmlfileName;

	void UpdateXMLfile();//��IPC�����¸��µ�xmlFILE�У�˽�к�����ֻ��bind�˲��� Update
	void UpdateIPC();	 //��xmlFILE�����¸��µ�IPC�У�˽�к�����ֻ��bind�˲��� Update

	//�Զ��庯��
	/*
	@brief:ͨ��AR���ĸ��ǵ㣬��������ĵ�
	@param  corner������4���ǵ�
	*/
	Point2i calculateCentre(std::vector<Point2f> corner);

public://��������ӿ�

	/*
	@brief:��һ������IPC��ͼƬ����λͼ�е�AR�룬��������3D����
	@param  img������ͼƬ���ڼ��
	@param  IPCindex�����ĸ���ŵ�IPC����Ϊ׼��
	@returns���������У�Ӧ��ȫ����robot�����Ϊ3D��������
	@time��i7-6700k ���� 13.6ms
	*/
	std::vector<IPCobj> location(Mat img, int IPCindex);

	/*
	@brief:��һ������IPC��ͼƬ����λͼ�е�AR�룬��������3D����,ͬʱ����һ������ͼƬ
	@param  img������ͼƬ���ڼ��
	@param  IPCindex�����ĸ���ŵ�IPC����Ϊ׼��
	@param  outimg���������һ������ͼƬ
	@returns���������У�Ӧ��ȫ����robot�����Ϊ3D��������
	@time��i7-6700k ���� 14.7ms
	*/
	std::vector<IPCobj> location(Mat img, int IPCindex,Mat &outimg);

	/*
	@brief:��IPCindex��IP������㵽�Ļ�������λAR�룬��������3D����
	@param  IPCindex�����ĸ���ŵ�IPC����Ϊ׼��
	@returns���������У�Ӧ��ȫ����robot�����Ϊ3D��������
	*/
	std::vector<IPCobj> location(int IPCindex);

	/*
	@brief:��һ������IPC��ͼƬ����λͼ�е�AR�룬�����������
	@time��i7-6700k ���� 12.8ms
	*/
	std::vector<IPCobj> locationMat(Mat img, int IPCindex);

	/*
	*@brief����ͼƬ�����������IP��������img�е�ͼƬ���б궨��errΪ����׼ֵ
	@param  img:ͼƬ����
	@param	board_size:���̵ĸ�����
	@param	square_size:����ÿһ���ӵĿ��
	@param	rtsp:��������RTSP��ַ
	@param	err_th:�궨��ӳ��������ֵ����������ֵ
	@returns����������ȷ
	*/
	bool AddIPC(std::vector<Mat> img, cv::Size board_size, cv::Size square_size, cv::String rtsp, double err_th = 1);

	/*
	*@brief����ͼƬ�ļ��У����������IP���������ļ����е�JPGͼƬ���б궨��errΪ����׼ֵ
	@param  filedir:ͼƬ�ļ���·��
	@param	board_size:���̵ĸ�����
	@param	square_size:����ÿһ���ӵĿ��
	@param	rtsp:��������RTSP��ַ
	@param	err_th:�궨��ӳ��������ֵ����������ֵ
	@returns����������ȷ
	*/
	bool AddIPC(std::string filedir, cv::Size board_size, cv::Size square_size, cv::String rtsp, double err_th=1 );

	/*
	*@brief��������������ϵ���������е�IPC������Ρ��������������IPC����ʱ�ų���
	���д˺���ʱ��Ӧ�ñ�֤����IPC�ֲ������ܣ��ҳ�������һ���궨ARtag��������IPC��
	�ܿ����˱궨ARtag��һ��Ҫ��֤�궨ARtag���ܶ���ֱ����ɱ궨��
	@returns����������ȷ
	*/
	bool setWorld();

	/*
	*@brief��������ɾ��IPC
	@param  index:Ӧ��ɾ����IPC�����
	@returns����������ȷ
	*/
	bool DeleteIPC(int index);

	/*
	*@brief����RTSPɾ��IPC
	@param  rtsp:Ӧ��ɾ����IPC��RTSP
	@returns����������ȷ
	*/
	bool DeleteIPC(String rtsp);

	/*
	*@brief���õ�����IPC��rtsp
	@returns������IPC��rtsp
	*/
	std::vector<std::string> getIPCrtsp();

	/*
	*@brief���õ�IPC����
	@returns��IPC����
	*/
	int getIPCNum();

	/*
	*@brief����һ��xml�ļ�ͬ����ʵ����flash����,,��xmlfile���ڣ����xml�ļ���
	���µ�IPC�У���xmlfile�����ڣ����IPC�и��µ�xml��
	@param xmlfile��xmlfile xml�ļ��� xmlfilename.xml
	*/
	void bindxml(std::string xmlfile);

	/*
	*@brief���õ�ָ��IPC��ŵ�ͼ��
	@param index��IPC���
	@returns��ͼ��
	*/
	Mat getIPCimage(int index);

	/*
	*@brief���õ�ָ��IPC��ŵľ���AR�����ͼƬ
	@param index��IPC���
	@returns��ͼ��
	*/
	Mat getIPCARPimage(int index);


};

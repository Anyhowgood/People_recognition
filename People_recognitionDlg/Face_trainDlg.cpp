// Face_trainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "People_recognitionDlg.h"
#include "Face_trainDlg.h"
#include "afxdialogex.h"

using namespace cv;
using namespace cv::face;
using namespace std;
using namespace face;

extern CascadeClassifier face_cascade;   //定义人脸分类器

//extern VideoCapture cap(0);    //打开默认摄像头
extern VideoCapture cap;    //打开默认摄像头
extern Mat frame;
extern Mat edges;
extern Mat gray;
//测试用图像与点

extern Point text_lb;

 CRect my_rect;
 CDC *my_pDC;
 HDC my_hDC;
 CWnd *my_pwnd;

 IplImage*  my_frame;
 /*
 创建CSV文件所需元素
 */
 vector<int> FT_number;
 int FT_num = 0;
 char* FT_filepath = "G:\\Microsft_Visual_Studio\\Visuak_Studio_2015_Projects\\People_face";
/*
加载人像类，检测基类作为全局变量
*/
extern CascadeClassifier cascade;
Ptr<FaceRecognizer> modelLBPH = Algorithm::load<LBPHFaceRecognizer>("G:\\Microsft_Visual_Studio\\Visuak_Studio_2015_Projects\\my_xml\\MyFaceLBPHModel.xml");

//建立用于存放人脸的向量容器
//extern vector<Rect> faces(0);
extern vector<Rect> faces;
extern int predictLBPH;

/*
获得所有注册的文件名信息
*/
vector<string> FT_Dir_name;
extern CStringArray user_name;
extern vector<string> Dir_name;

string user_name_init[];
// Face_trainDlg 对话框

void FT_getFiles(string path, vector<string>& files);

IMPLEMENT_DYNAMIC(Face_trainDlg, CDialogEx)

Face_trainDlg::Face_trainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FACE_TRAIN, pParent)
{
	
	
}

Face_trainDlg::~Face_trainDlg()
{
	
}

void Face_trainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Face_trainDlg, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &Face_trainDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL Face_trainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	AllocConsole(); //调试信息窗口的初始化函数

/*
	int user_size = user_name.GetSize();

	for (int j = 0; j < user_size; j++) {
		_cprintf("user_name = %s\n ", user_name.GetAt(j));
		user_name_init[j] = user_name.GetAt(j);
		//CString strTmp = user_name.GetAt(j);
		//cout << Dir_name[j].c_str() << endl;
	}

	putchar(10);
	for (int j = 0; j < user_size; j++) {
		//_cprintf("user_name_init = %s\n ", user_name_init[j].c_str());
		//user_name_init[j] = user_name.GetAt(j);
	}
*/
	vector<string> files;
	FT_getFiles(FT_filepath, files);

	int FT_Dir_size = FT_Dir_name.size();
	for (int j = 0; j < FT_Dir_size; j++) {
		_cprintf("FT_Dir_name = %s\n ", FT_Dir_name[j].c_str());
	//	user_name.Add(Dir_name[j].c_str());
		//cout << Dir_name[j].c_str() << endl;
	}

	SetTimer(3, 5, NULL);//定时器，定时时间和帧率一致??
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// Face_trainDlg 消息处理程序
/*
获取注册用户名以及它的序号
*/
void FT_getFiles(string path, vector<string>& files)
{
	//文件句柄  
	long long  hFile = 0;
	/*在win10中，使用文件遍历函数_findnext会报0xC0000005错误
	原因：
	_findnext()第一个参数”路径句柄”，返回的类型为intptr_t（long long），如果定义为long，在win7中是没有问题，但是在win10中就要改为long long或者intptr_t
	*/
	//文件信息结构体
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib &  _A_SUBDIR ))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0 && strcmp(fileinfo.name,"csv") != 0) {
					FT_Dir_name.push_back(fileinfo.name);
					FT_getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
					FT_num++;
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				FT_number.push_back(FT_num);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

/*
默认的定时服务函数
*/
void Face_trainDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	cap >> frame;

	cvtColor(frame, gray, CV_BGR2GRAY);
	//改变图像大小，使用双线性差值
	//resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);
	//变换后的图像进行直方图均值化处理
	equalizeHist(gray, gray);

	cascade.detectMultiScale(gray, faces,
		1.1, 2, 0
		//|CV_HAAR_FIND_BIGGEST_OBJECT
		//|CV_HAAR_DO_ROUGH_SEARCH
		| CV_HAAR_SCALE_IMAGE,
		Size(30, 30));

	Mat face;
	//Point text_lb;

	for (size_t i = 0; i < faces.size(); i++)
	{
		if (faces[i].height > 0 && faces[i].width > 0)
		{
			face = gray(faces[i]);
			text_lb = Point(faces[i].x, faces[i].y);

			rectangle(frame, faces[i], Scalar(255, 0, 0), 1, 8, 0);
		}
	}
	Mat face_test;

	predictLBPH = 0;
	if (face.rows >= 120)
	{
		resize(face, face_test, Size(92, 112));
	}

	if (!face_test.empty())
	{
		//测试图像应该是灰度图
		predictLBPH = modelLBPH->predict(face_test);
	}

//	cout << predictLBPH << endl;
	string name = "Not Find";

	_cprintf("FT_Dir_name.siz = %d\n ", FT_Dir_name.size());
	_cprintf("predictLBPH_num = %d\n ", predictLBPH);

	if (predictLBPH <= 19)
	{
		putText(frame, name, text_lb, FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255));
	}
	else {
		string name1 = FT_Dir_name.at(predictLBPH);
		//_cprintf("name1 = %s\n ", FT_Dir_name.at(predictLBPH).c_str());
		putText(frame, name1, text_lb, FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255));
	}

	my_frame = &IplImage(frame); //g_srcImage强转为Mat格式 单图片显示用

	my_pDC = GetDlgItem(AFX_IDC_PRINT_DOCNAME3)->GetDC(); //获取显示控件的DC()
	GetDlgItem(AFX_IDC_PRINT_DOCNAME3)->GetClientRect(&my_rect);
	my_hDC = my_pDC->GetSafeHdc(); //获取显示控件的句柄  

	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(my_frame, 1);//复制该帧图像     &IplImage
	m_CvvImage.DrawToHDC(my_hDC, &my_rect); //显示到设备的矩形框内  

	CDialogEx::OnTimer(nIDEvent);
}

void Face_trainDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(3);   //关闭定时器
	FT_Dir_name.clear();
	CDialogEx::OnCancel();
}

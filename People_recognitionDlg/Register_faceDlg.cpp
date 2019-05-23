// Register_faceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "People_recognitionDlg.h"
#include "Register_faceDlg.h"
#include "afxdialogex.h"

/*
#include "opencv2/opencv.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/face.hpp"
#include "opencv2/core.hpp"
#include "conio.h"

#include<stdio.h>
#include<fstream>
#include<sstream>
#include<iostream>
#include<vector>
#include<io.h>
#include<math.h>
*/
using namespace std;
using namespace cv;
using namespace face;

/*
读取摄像头和显示在控件上需要元素
*/
Mat g_srcImage, dest_Image;
//读取视频或摄像头
VideoCapture Capture(0);
IplImage* frame;
IplImage* m_grabframe;//存储单张照片用
CRect rect;
CDC *pDC;
HDC hDC;
CWnd *pwnd;
/*
人脸裁剪需要元素
*/
void detectAndDisplay(Mat frame, const char *ImagesName);
int ListFolder(CString sPath);
extern String face_cascade_name;
//extern String face_cascade_name = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_default.xml";
String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";

extern CascadeClassifier face_cascade;   //定义人脸分类器
CascadeClassifier eyes_cascade;   //定义人眼分类器
String window_name = "Capture - Face detection";
/*
将图像保存到文件夹中所需元素
*/
int ImgNum = 0;
char  People_Everyone_Path[150];
/*
创建CSV文件所需元素
*/
vector<int> number;
int num = 0;
char* filepath = "G:\\Microsft_Visual_Studio\\Visuak_Studio_2015_Projects\\People_face";
/*
获取注册信息所需元素
*/
const CString origiImgFilePath = "G:\\Microsft_Visual_Studio\\Visuak_Studio_2015_Projects\\People_face\\*";
void myDeleteDirectory(CString directory_path);
/*
获得所有注册的文件名信息
*/
vector<string> Dir_name;

CStringArray user_name;
// Register_faceDlg 对话框

IMPLEMENT_DYNAMIC(Register_faceDlg, CDialogEx)

Register_faceDlg::Register_faceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_REGISTER_FACEDLG, pParent)
{

}

Register_faceDlg::~Register_faceDlg()
{
}

void Register_faceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Register_faceDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Register_faceDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Register_faceDlg::OnBnClickedButton2)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON3, &Register_faceDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &Register_faceDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &Register_faceDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDCANCEL, &Register_faceDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// Register_faceDlg 消息处理程序


void Register_faceDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString Register_People_Name;
	CString People_Face_Dir = TEXT("G:\\Microsft_Visual_Studio\\Visuak_Studio_2015_Projects\\People_face\\");
	CString Dest_Path;

	GetDlgItem(IDC_EDIT1)->GetWindowText(Register_People_Name);	//读取

	GetDlgItem(IDC_EDIT2)->SetWindowText(Register_People_Name);  //写入 

	char  People_face_Path[150];
	sprintf_s(People_face_Path, "%s%s", People_Face_Dir, Register_People_Name);
	strncpy_s(People_Everyone_Path, People_face_Path, 150);
	Dest_Path = People_face_Path;  //char类型转换成CString类型

	if (!PathIsDirectory(Dest_Path))
	{
		CreateDirectory(Dest_Path, 0);//不存在则创建
		_cprintf("Dest_Path = %s\n", Dest_Path);
		MessageBox(_T("该用户文件夹已创建！！！"));
		//return;
	}
	else
	{
		_cprintf("Dest_Path = %s\n", Dest_Path);
		MessageBox(_T("该用户文件夹已存在！！！"));
	}
}


void Register_faceDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	Capture >> g_srcImage;    //摄像头读取到的照片重定向到g_srcImage
							  //imshow("显示摄像头",g_srcImage);
	frame = &IplImage(g_srcImage); //g_srcImage强转为Mat格式 单图片显示用
	pDC = GetDlgItem(AFX_IDC_PRINT_DOCNAME)->GetDC(); // GetDlgItem（IDC_PIC_STATIC）意思是获取显示控件的句柄（句柄就是指针），获取显示控件的DC??
	GetDlgItem(AFX_IDC_PRINT_DOCNAME)->GetClientRect(&rect);
	hDC = pDC->GetSafeHdc(); //获取显示控件的句柄??	

	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(frame, 1); //复制该帧图像? ? ?
	m_CvvImage.DrawToHDC(hDC, &rect); //显示到设备的矩形框内??
	ReleaseDC(pDC);

	SetTimer(1, 5, NULL);//定时器，定时时间和帧率一致??
}

/*
定时器默认服务函数
*/
void Register_faceDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Capture >> g_srcImage;
	//imshow("显示摄像头",g_srcImage);
	pDC = GetDlgItem(AFX_IDC_PRINT_DOCNAME)->GetDC(); //获取显示控件的DC()
	GetDlgItem(AFX_IDC_PRINT_DOCNAME)->GetClientRect(&rect);
	hDC = pDC->GetSafeHdc(); //获取显示控件的句柄  
	frame = &IplImage(g_srcImage);

	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(frame, 1); //复制该帧图像     
	m_CvvImage.DrawToHDC(hDC, &rect); //显示到设备的矩形框内  

	CDialogEx::OnTimer(nIDEvent);
}


void Register_faceDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	Capture >> g_srcImage;    //摄像头读取到的照片重定向到g_srcImage
	frame = &IplImage(g_srcImage); //g_srcImage强转为Mat格式 单图片显示用
	pDC = GetDlgItem(AFX_IDC_PRINT_PAGENUM)->GetDC(); // GetDlgItem（IDC_PIC_STATIC）意思是获取显示控件的句柄（句柄就是指针），获取显示控件的DC??
	GetDlgItem(AFX_IDC_PRINT_PAGENUM)->GetClientRect(&rect);
	hDC = pDC->GetSafeHdc(); //获取显示控件的句柄??	

	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(frame, 1); //复制该帧图像? ? ?
	m_CvvImage.DrawToHDC(hDC, &rect); //显示到设备的矩形框内??

	destroyWindow("显示摄像头");

	ReleaseDC(pDC);
	KillTimer(1);   //关闭定时器
}


/*
功能：遍历指定文件中的文件以及子文件，并对.jpg格式的文件计数
参数：指定的文件路径名
返回值： 当前文件下的.jpg文件个数值
*/
int ListFolder(CString sPath)
{
	CFileFind find;
	BOOL bFound;
	CString fdPath;   //设置路径变量
	CString strTmp;      //后缀名变量
	int i = 0;   //计数值

	bFound = find.FindFile(sPath + "\\*.* "); //确定文件夹下是否含有文件

	while (bFound)
	{
		bFound = find.FindNextFile();
		CString  fdPath = find.GetFilePath();

		if (find.IsDirectory())
		{
			if (!find.IsDots())//如果是文件  则递归
				i = ListFolder(fdPath);
		}
		else
		{
			strTmp = fdPath.Right(4);  //取后缀名
			strTmp.MakeLower();		//字符串小写化
			if (strTmp == ".jpg")
				i++;
		}
	}
	find.Close();

	return i;
}

/** @function detectAndDisplay */
/*切割图片并更改图片大小*/
void detectAndDisplay(Mat frame, const char *ImagesName)
{
	std::vector<Rect> faces;
	Mat frame_gray;
	Mat MyFace;
	char Images_Dest_Name[150];
	strncpy_s(Images_Dest_Name, ImagesName, 150);
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);
	//-- Detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 3, CV_HAAR_DO_ROUGH_SEARCH, Size(50, 50));
	for (size_t j = 0; j < faces.size(); j++)
	{
		Mat faceROI = frame_gray(faces[j]);
		if (faceROI.cols > 100)
		{
			resize(faceROI, MyFace, Size(92, 112));
			imwrite(Images_Dest_Name, MyFace);
		}
	}
	//-- Show what you got
}

void Register_faceDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	Capture >> g_srcImage;
	frame = &IplImage(g_srcImage); //g_srcImage强转为Mat格式 单图片显示用

	m_grabframe = frame;
	if (m_grabframe == 0)
	{
		MessageBox(_T("摄像头已关闭，无法捕捉图像！！！"));
		return;
	}

	if (!PathIsDirectory(People_Everyone_Path))
	{
		CreateDirectory(People_Everyone_Path, 0);//不存在则创建
		MessageBox(_T("标定图片文件夹已创建！！！"));
		return;
	}

	char ImagesName[150];
	ImgNum = ListFolder(People_Everyone_Path);//获取指定文件下的.jpg文件个数
	ImgNum = ImgNum + 1;

	_cprintf("ImgNum = %d\n", ImgNum);
	if (ImgNum >= 11) {
		MessageBox(_T("照片数量已经达到十张,请点击注册完成按钮进行下一步！"));
		return;
	}
	sprintf_s(ImagesName, "%s\\%.2d%s", People_Everyone_Path, ImgNum, ".jpg");
	//imwrite(ImagesName, dest_Image);//把灰度图片写入指定文件夹的文件中去
	detectAndDisplay(g_srcImage, ImagesName);//在函数中切割出人脸并修改大小保存到指定路径


	int Need_Imgnum = 0;
	Need_Imgnum = 10 - ImgNum;
	CString CNeed_Imgnum;
	CNeed_Imgnum.Format(_T("%d"), Need_Imgnum);
	GetDlgItem(IDC_EDIT3)->SetWindowText(CNeed_Imgnum);  //写入还需要拍照的数量

														 //以下代码是完成图像的显示过程
	pDC = GetDlgItem(AFX_IDC_PRINT_PAGENUM)->GetDC();//GetDlgItem(IDC_PIC_STATIC)意思为获取显示控件的句柄（句柄就是指针），获取显示控件的DC
	GetDlgItem(AFX_IDC_PRINT_PAGENUM)->GetClientRect(&rect);
	hDC = pDC->GetSafeHdc();//获取显示控件的句柄
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(frame, 1); //复制该帧图像   
	m_CvvImage.DrawToHDC(hDC, &rect); //显示到设备环境的矩形框内
}

void getFiles(string path, vector<string>& files)
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
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
					Dir_name.push_back(fileinfo.name);
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
					num++;
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				number.push_back(num);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
/*
读取CSV文件
*/
//使用CSV文件去读图像和标签，主要使用stringstream和getline方法
static void read_csv(const string &myfilename, vector<Mat>& images, vector<int>& labels, char separator = ';')
{
	std::ifstream file(myfilename.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, path, classlabel;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel);
		if (!path.empty() && !classlabel.empty()) {
			images.push_back(imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
}
/*
注册完成按钮的实现步骤
1.生产了一个csv.txt文件
size - 1 是为了不将csv.txt自己本身写进去
2.模型训练，生产一个.xml文件
*/
void Register_faceDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	DeleteFile("G:\\Microsft_Visual_Studio\\Visuak_Studio_2015_Projects\\People_face\\csv.txt");
	vector<string> files;
	getFiles(filepath, files);

	ofstream off("G:\\Microsft_Visual_Studio\\Visuak_Studio_2015_Projects\\People_face\\csv.txt", ios::out);
	int size = files.size();
	for (int i = 0; i < size; i++) {
		off << files[i].c_str();
		off << ";";
		off << number[i];
		off << "\n";
	}
	off.close();
	
	int Dir_size = Dir_name.size();
	for (int j = 0; j < Dir_size; j++) {
		_cprintf("message = %s\n ", Dir_name[j].c_str());
		user_name.Add(Dir_name[j].c_str());
		//cout << Dir_name[j].c_str() << endl;
	}

	putchar(10);
	for (int j = 0; j < Dir_size; j++) {
		_cprintf("user_name = %s\n ", user_name.GetAt(j));
		//cout << Dir_name[j].c_str() << endl;
	}

	MessageBox(_T("CSV文件创建成功！！！"));

	string fn_csv = "G:\\Microsft_Visual_Studio\\Visuak_Studio_2015_Projects\\People_face\\csv.txt";
	// 2个容器来存放图像数据和对应的标签
	vector<Mat> images;
	vector<int> labels;
	// 读取数据. 如果文件不合法就会出错
	// 输入的文件名已经有了.
	try
	{
		read_csv(fn_csv, images, labels);
	}
	catch (cv::Exception& e)
	{
		cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
		// 文件有问题，我们啥也做不了了，退出了
		exit(1);
	}

	// 如果没有读取到足够图片，也退出.
	if (images.size() <= 1) {
		string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
		CV_Error(CV_StsError, error_message);
		//_cprintf("error_message = %s\n ", error_message);
	}

	// 下面几行创建了一个特征脸模型用于人脸识别，
	// 通过CSV文件读取的图像和标签训练它。
	// T这里是一个完整的PCA变换
	//如果你只想保留10个主成分，使用如下代码
	//    cv::createEigenFaceRecognizer(10);
	// 如果你还希望使用置信度阈值来初始化，使用以下语句：
	//    cv::createEigenFaceRecognizer(10, 123.0);
	//
	// 如果你使用所有特征并且使用一个阈值，使用以下语句：
	//     cv::createEigenFaceRecognizer(0, 123.0);

	//	Ptr<EigenFaceRecognizer> model = EigenFaceRecognizer::create();
	//	Ptr<FisherFaceRecognizer> model = FisherFaceRecognizer::create();
	//	Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();
	

	//训练生成特征脸模型
	//Ptr<FaceRecognizer>model2 = LBPHFaceRecognizer::create(10, 123.0);//createLBPHFaceRecognizer();
	//model2->train(images, labels);
	//model2->save("G:\\Microsft_Visual_Studio\\Visuak_Studio_2015_Projects\\my_xml\\MyFaceLBPHModel.xml");

	Ptr<FaceRecognizer>model2 = LBPHFaceRecognizer::create(10);//createLBPHFaceRecognizer();
	model2->train(images, labels);
	model2->save("G:\\Microsft_Visual_Studio\\Visuak_Studio_2015_Projects\\my_xml\\MyFaceLBPHModel.xml");
	MessageBox(_T("特征脸模型生成成功！请进行下一步！"));

	CDialogEx::OnCancel();//退出当前界面，回到上一级界面
}

/*
删除一个非空文件夹
*/
void myDeleteDirectory(CString directory_path)
{
	CFileFind finder;
	CString path;
	path.Format(_T("%s/*.*"), directory_path);
	BOOL bWorking = finder.FindFile(path);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory() && !finder.IsDots())//处理文件夹
		{
			myDeleteDirectory(finder.GetFilePath()); //递归删除文件夹
			RemoveDirectory(finder.GetFilePath());
		}
		else//处理文件
		{
			DeleteFile(finder.GetFilePath());
		}
	}
	RemoveDirectory(directory_path);
}

void Register_faceDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();

	CString Remove_Path;

	GetDlgItem(IDC_EDIT2)->GetWindowText(Remove_Path);	//读取
	
	if (!Remove_Path.IsEmpty()) {
		Remove_Path = People_Everyone_Path;
		myDeleteDirectory(Remove_Path);
		MessageBox(_T("文件删除成功！！！"));
	}
}

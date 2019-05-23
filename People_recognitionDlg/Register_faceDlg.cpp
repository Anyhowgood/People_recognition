// Register_faceDlg.cpp : ʵ���ļ�
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
��ȡ����ͷ����ʾ�ڿؼ�����ҪԪ��
*/
Mat g_srcImage, dest_Image;
//��ȡ��Ƶ������ͷ
VideoCapture Capture(0);
IplImage* frame;
IplImage* m_grabframe;//�洢������Ƭ��
CRect rect;
CDC *pDC;
HDC hDC;
CWnd *pwnd;
/*
�����ü���ҪԪ��
*/
void detectAndDisplay(Mat frame, const char *ImagesName);
int ListFolder(CString sPath);
extern String face_cascade_name;
//extern String face_cascade_name = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_default.xml";
String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";

extern CascadeClassifier face_cascade;   //��������������
CascadeClassifier eyes_cascade;   //�������۷�����
String window_name = "Capture - Face detection";
/*
��ͼ�񱣴浽�ļ���������Ԫ��
*/
int ImgNum = 0;
char  People_Everyone_Path[150];
/*
����CSV�ļ�����Ԫ��
*/
vector<int> number;
int num = 0;
char* filepath = "G:\\Microsft_Visual_Studio\\Visuak_Studio_2015_Projects\\People_face";
/*
��ȡע����Ϣ����Ԫ��
*/
const CString origiImgFilePath = "G:\\Microsft_Visual_Studio\\Visuak_Studio_2015_Projects\\People_face\\*";
void myDeleteDirectory(CString directory_path);
/*
�������ע����ļ�����Ϣ
*/
vector<string> Dir_name;

CStringArray user_name;
// Register_faceDlg �Ի���

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


// Register_faceDlg ��Ϣ�������


void Register_faceDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString Register_People_Name;
	CString People_Face_Dir = TEXT("G:\\Microsft_Visual_Studio\\Visuak_Studio_2015_Projects\\People_face\\");
	CString Dest_Path;

	GetDlgItem(IDC_EDIT1)->GetWindowText(Register_People_Name);	//��ȡ

	GetDlgItem(IDC_EDIT2)->SetWindowText(Register_People_Name);  //д�� 

	char  People_face_Path[150];
	sprintf_s(People_face_Path, "%s%s", People_Face_Dir, Register_People_Name);
	strncpy_s(People_Everyone_Path, People_face_Path, 150);
	Dest_Path = People_face_Path;  //char����ת����CString����

	if (!PathIsDirectory(Dest_Path))
	{
		CreateDirectory(Dest_Path, 0);//�������򴴽�
		_cprintf("Dest_Path = %s\n", Dest_Path);
		MessageBox(_T("���û��ļ����Ѵ���������"));
		//return;
	}
	else
	{
		_cprintf("Dest_Path = %s\n", Dest_Path);
		MessageBox(_T("���û��ļ����Ѵ��ڣ�����"));
	}
}


void Register_faceDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Capture >> g_srcImage;    //����ͷ��ȡ������Ƭ�ض���g_srcImage
							  //imshow("��ʾ����ͷ",g_srcImage);
	frame = &IplImage(g_srcImage); //g_srcImageǿתΪMat��ʽ ��ͼƬ��ʾ��
	pDC = GetDlgItem(AFX_IDC_PRINT_DOCNAME)->GetDC(); // GetDlgItem��IDC_PIC_STATIC����˼�ǻ�ȡ��ʾ�ؼ��ľ�����������ָ�룩����ȡ��ʾ�ؼ���DC??
	GetDlgItem(AFX_IDC_PRINT_DOCNAME)->GetClientRect(&rect);
	hDC = pDC->GetSafeHdc(); //��ȡ��ʾ�ؼ��ľ��??	

	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(frame, 1); //���Ƹ�֡ͼ��? ? ?
	m_CvvImage.DrawToHDC(hDC, &rect); //��ʾ���豸�ľ��ο���??
	ReleaseDC(pDC);

	SetTimer(1, 5, NULL);//��ʱ������ʱʱ���֡��һ��??
}

/*
��ʱ��Ĭ�Ϸ�����
*/
void Register_faceDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	Capture >> g_srcImage;
	//imshow("��ʾ����ͷ",g_srcImage);
	pDC = GetDlgItem(AFX_IDC_PRINT_DOCNAME)->GetDC(); //��ȡ��ʾ�ؼ���DC()
	GetDlgItem(AFX_IDC_PRINT_DOCNAME)->GetClientRect(&rect);
	hDC = pDC->GetSafeHdc(); //��ȡ��ʾ�ؼ��ľ��  
	frame = &IplImage(g_srcImage);

	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(frame, 1); //���Ƹ�֡ͼ��     
	m_CvvImage.DrawToHDC(hDC, &rect); //��ʾ���豸�ľ��ο���  

	CDialogEx::OnTimer(nIDEvent);
}


void Register_faceDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Capture >> g_srcImage;    //����ͷ��ȡ������Ƭ�ض���g_srcImage
	frame = &IplImage(g_srcImage); //g_srcImageǿתΪMat��ʽ ��ͼƬ��ʾ��
	pDC = GetDlgItem(AFX_IDC_PRINT_PAGENUM)->GetDC(); // GetDlgItem��IDC_PIC_STATIC����˼�ǻ�ȡ��ʾ�ؼ��ľ�����������ָ�룩����ȡ��ʾ�ؼ���DC??
	GetDlgItem(AFX_IDC_PRINT_PAGENUM)->GetClientRect(&rect);
	hDC = pDC->GetSafeHdc(); //��ȡ��ʾ�ؼ��ľ��??	

	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(frame, 1); //���Ƹ�֡ͼ��? ? ?
	m_CvvImage.DrawToHDC(hDC, &rect); //��ʾ���豸�ľ��ο���??

	destroyWindow("��ʾ����ͷ");

	ReleaseDC(pDC);
	KillTimer(1);   //�رն�ʱ��
}


/*
���ܣ�����ָ���ļ��е��ļ��Լ����ļ�������.jpg��ʽ���ļ�����
������ָ�����ļ�·����
����ֵ�� ��ǰ�ļ��µ�.jpg�ļ�����ֵ
*/
int ListFolder(CString sPath)
{
	CFileFind find;
	BOOL bFound;
	CString fdPath;   //����·������
	CString strTmp;      //��׺������
	int i = 0;   //����ֵ

	bFound = find.FindFile(sPath + "\\*.* "); //ȷ���ļ������Ƿ����ļ�

	while (bFound)
	{
		bFound = find.FindNextFile();
		CString  fdPath = find.GetFilePath();

		if (find.IsDirectory())
		{
			if (!find.IsDots())//������ļ�  ��ݹ�
				i = ListFolder(fdPath);
		}
		else
		{
			strTmp = fdPath.Right(4);  //ȡ��׺��
			strTmp.MakeLower();		//�ַ���Сд��
			if (strTmp == ".jpg")
				i++;
		}
	}
	find.Close();

	return i;
}

/** @function detectAndDisplay */
/*�и�ͼƬ������ͼƬ��С*/
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Capture >> g_srcImage;
	frame = &IplImage(g_srcImage); //g_srcImageǿתΪMat��ʽ ��ͼƬ��ʾ��

	m_grabframe = frame;
	if (m_grabframe == 0)
	{
		MessageBox(_T("����ͷ�ѹرգ��޷���׽ͼ�񣡣���"));
		return;
	}

	if (!PathIsDirectory(People_Everyone_Path))
	{
		CreateDirectory(People_Everyone_Path, 0);//�������򴴽�
		MessageBox(_T("�궨ͼƬ�ļ����Ѵ���������"));
		return;
	}

	char ImagesName[150];
	ImgNum = ListFolder(People_Everyone_Path);//��ȡָ���ļ��µ�.jpg�ļ�����
	ImgNum = ImgNum + 1;

	_cprintf("ImgNum = %d\n", ImgNum);
	if (ImgNum >= 11) {
		MessageBox(_T("��Ƭ�����Ѿ��ﵽʮ��,����ע����ɰ�ť������һ����"));
		return;
	}
	sprintf_s(ImagesName, "%s\\%.2d%s", People_Everyone_Path, ImgNum, ".jpg");
	//imwrite(ImagesName, dest_Image);//�ѻҶ�ͼƬд��ָ���ļ��е��ļ���ȥ
	detectAndDisplay(g_srcImage, ImagesName);//�ں������и���������޸Ĵ�С���浽ָ��·��


	int Need_Imgnum = 0;
	Need_Imgnum = 10 - ImgNum;
	CString CNeed_Imgnum;
	CNeed_Imgnum.Format(_T("%d"), Need_Imgnum);
	GetDlgItem(IDC_EDIT3)->SetWindowText(CNeed_Imgnum);  //д�뻹��Ҫ���յ�����

														 //���´��������ͼ�����ʾ����
	pDC = GetDlgItem(AFX_IDC_PRINT_PAGENUM)->GetDC();//GetDlgItem(IDC_PIC_STATIC)��˼Ϊ��ȡ��ʾ�ؼ��ľ�����������ָ�룩����ȡ��ʾ�ؼ���DC
	GetDlgItem(AFX_IDC_PRINT_PAGENUM)->GetClientRect(&rect);
	hDC = pDC->GetSafeHdc();//��ȡ��ʾ�ؼ��ľ��
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(frame, 1); //���Ƹ�֡ͼ��   
	m_CvvImage.DrawToHDC(hDC, &rect); //��ʾ���豸�����ľ��ο���
}

void getFiles(string path, vector<string>& files)
{
	//�ļ����  
	long long  hFile = 0;
	/*��win10�У�ʹ���ļ���������_findnext�ᱨ0xC0000005����
	ԭ��
	_findnext()��һ��������·������������ص�����Ϊintptr_t��long long�����������Ϊlong����win7����û�����⣬������win10�о�Ҫ��Ϊlong long����intptr_t
	*/
	//�ļ���Ϣ�ṹ��
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//�����Ŀ¼,����֮  
			//�������,�����б�  
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
��ȡCSV�ļ�
*/
//ʹ��CSV�ļ�ȥ��ͼ��ͱ�ǩ����Ҫʹ��stringstream��getline����
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
ע����ɰ�ť��ʵ�ֲ���
1.������һ��csv.txt�ļ�
size - 1 ��Ϊ�˲���csv.txt�Լ�����д��ȥ
2.ģ��ѵ��������һ��.xml�ļ�
*/
void Register_faceDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	MessageBox(_T("CSV�ļ������ɹ�������"));

	string fn_csv = "G:\\Microsft_Visual_Studio\\Visuak_Studio_2015_Projects\\People_face\\csv.txt";
	// 2�����������ͼ�����ݺͶ�Ӧ�ı�ǩ
	vector<Mat> images;
	vector<int> labels;
	// ��ȡ����. ����ļ����Ϸ��ͻ����
	// ������ļ����Ѿ�����.
	try
	{
		read_csv(fn_csv, images, labels);
	}
	catch (cv::Exception& e)
	{
		cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
		// �ļ������⣬����ɶҲ�������ˣ��˳���
		exit(1);
	}

	// ���û�ж�ȡ���㹻ͼƬ��Ҳ�˳�.
	if (images.size() <= 1) {
		string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
		CV_Error(CV_StsError, error_message);
		//_cprintf("error_message = %s\n ", error_message);
	}

	// ���漸�д�����һ��������ģ����������ʶ��
	// ͨ��CSV�ļ���ȡ��ͼ��ͱ�ǩѵ������
	// T������һ��������PCA�任
	//�����ֻ�뱣��10�����ɷ֣�ʹ�����´���
	//    cv::createEigenFaceRecognizer(10);
	// ����㻹ϣ��ʹ�����Ŷ���ֵ����ʼ����ʹ��������䣺
	//    cv::createEigenFaceRecognizer(10, 123.0);
	//
	// �����ʹ��������������ʹ��һ����ֵ��ʹ��������䣺
	//     cv::createEigenFaceRecognizer(0, 123.0);

	//	Ptr<EigenFaceRecognizer> model = EigenFaceRecognizer::create();
	//	Ptr<FisherFaceRecognizer> model = FisherFaceRecognizer::create();
	//	Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();
	

	//ѵ������������ģ��
	//Ptr<FaceRecognizer>model2 = LBPHFaceRecognizer::create(10, 123.0);//createLBPHFaceRecognizer();
	//model2->train(images, labels);
	//model2->save("G:\\Microsft_Visual_Studio\\Visuak_Studio_2015_Projects\\my_xml\\MyFaceLBPHModel.xml");

	Ptr<FaceRecognizer>model2 = LBPHFaceRecognizer::create(10);//createLBPHFaceRecognizer();
	model2->train(images, labels);
	model2->save("G:\\Microsft_Visual_Studio\\Visuak_Studio_2015_Projects\\my_xml\\MyFaceLBPHModel.xml");
	MessageBox(_T("������ģ�����ɳɹ����������һ����"));

	CDialogEx::OnCancel();//�˳���ǰ���棬�ص���һ������
}

/*
ɾ��һ���ǿ��ļ���
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
		if (finder.IsDirectory() && !finder.IsDots())//�����ļ���
		{
			myDeleteDirectory(finder.GetFilePath()); //�ݹ�ɾ���ļ���
			RemoveDirectory(finder.GetFilePath());
		}
		else//�����ļ�
		{
			DeleteFile(finder.GetFilePath());
		}
	}
	RemoveDirectory(directory_path);
}

void Register_faceDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();

	CString Remove_Path;

	GetDlgItem(IDC_EDIT2)->GetWindowText(Remove_Path);	//��ȡ
	
	if (!Remove_Path.IsEmpty()) {
		Remove_Path = People_Everyone_Path;
		myDeleteDirectory(Remove_Path);
		MessageBox(_T("�ļ�ɾ���ɹ�������"));
	}
}

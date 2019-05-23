// Face_trainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "People_recognitionDlg.h"
#include "Face_trainDlg.h"
#include "afxdialogex.h"

using namespace cv;
using namespace cv::face;
using namespace std;
using namespace face;

extern CascadeClassifier face_cascade;   //��������������

//extern VideoCapture cap(0);    //��Ĭ������ͷ
extern VideoCapture cap;    //��Ĭ������ͷ
extern Mat frame;
extern Mat edges;
extern Mat gray;
//������ͼ�����

extern Point text_lb;

 CRect my_rect;
 CDC *my_pDC;
 HDC my_hDC;
 CWnd *my_pwnd;

 IplImage*  my_frame;
 /*
 ����CSV�ļ�����Ԫ��
 */
 vector<int> FT_number;
 int FT_num = 0;
 char* FT_filepath = "G:\\Microsft_Visual_Studio\\Visuak_Studio_2015_Projects\\People_face";
/*
���������࣬��������Ϊȫ�ֱ���
*/
extern CascadeClassifier cascade;
Ptr<FaceRecognizer> modelLBPH = Algorithm::load<LBPHFaceRecognizer>("G:\\Microsft_Visual_Studio\\Visuak_Studio_2015_Projects\\my_xml\\MyFaceLBPHModel.xml");

//�������ڴ����������������
//extern vector<Rect> faces(0);
extern vector<Rect> faces;
extern int predictLBPH;

/*
�������ע����ļ�����Ϣ
*/
vector<string> FT_Dir_name;
extern CStringArray user_name;
extern vector<string> Dir_name;

string user_name_init[];
// Face_trainDlg �Ի���

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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	AllocConsole(); //������Ϣ���ڵĳ�ʼ������

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

	SetTimer(3, 5, NULL);//��ʱ������ʱʱ���֡��һ��??
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

// Face_trainDlg ��Ϣ�������
/*
��ȡע���û����Լ��������
*/
void FT_getFiles(string path, vector<string>& files)
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
Ĭ�ϵĶ�ʱ������
*/
void Face_trainDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	cap >> frame;

	cvtColor(frame, gray, CV_BGR2GRAY);
	//�ı�ͼ���С��ʹ��˫���Բ�ֵ
	//resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);
	//�任���ͼ�����ֱ��ͼ��ֵ������
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
		//����ͼ��Ӧ���ǻҶ�ͼ
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

	my_frame = &IplImage(frame); //g_srcImageǿתΪMat��ʽ ��ͼƬ��ʾ��

	my_pDC = GetDlgItem(AFX_IDC_PRINT_DOCNAME3)->GetDC(); //��ȡ��ʾ�ؼ���DC()
	GetDlgItem(AFX_IDC_PRINT_DOCNAME3)->GetClientRect(&my_rect);
	my_hDC = my_pDC->GetSafeHdc(); //��ȡ��ʾ�ؼ��ľ��  

	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(my_frame, 1);//���Ƹ�֡ͼ��     &IplImage
	m_CvvImage.DrawToHDC(my_hDC, &my_rect); //��ʾ���豸�ľ��ο���  

	CDialogEx::OnTimer(nIDEvent);
}

void Face_trainDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	KillTimer(3);   //�رն�ʱ��
	FT_Dir_name.clear();
	CDialogEx::OnCancel();
}


// People_recognitionDlgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "People_recognitionDlg.h"
#include "People_recognitionDlgDlg.h"
#include "afxdialogex.h"
#include "Register_faceDlg.h"
#include "Face_trainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace cv;
using namespace cv::face;
using namespace std;
using namespace face;

String face_cascade_name = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_default.xml";
CascadeClassifier face_cascade;   //��������������

VideoCapture cap(0);    //��Ĭ������ͷ
Mat frame;
Mat edges;
Mat gray;
//������ͼ�����

Point text_lb;


/*
���������࣬��������Ϊȫ�ֱ���
*/
CascadeClassifier cascade;
//Ptr<FaceRecognizer> modelLBPH = Algorithm::load<LBPHFaceRecognizer>("G:\\Microsft_Visual_Studio\\Visuak_Studio_2015_Projects\\my_xml\\MyFaceLBPHModel.xml");;
//Ptr<FaceRecognizer> modelLBPH= Algorithm::load<LBPHFaceRecognizer>("G:\\Microsft_Visual_Studio\\Visuak_Studio_2015_Projects\\my_xml\\MyFaceLBPHModel.xml");;

//�������ڴ����������������
vector<Rect> faces(0);

int predictLBPH = 0;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPeople_recognitionDlgDlg �Ի���



CPeople_recognitionDlgDlg::CPeople_recognitionDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PEOPLE_RECOGNITIONDLG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPeople_recognitionDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPeople_recognitionDlgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPeople_recognitionDlgDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPeople_recognitionDlgDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CPeople_recognitionDlgDlg ��Ϣ�������

BOOL CPeople_recognitionDlgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	AllocConsole(); //������Ϣ���ڵĳ�ʼ������
	if (!face_cascade.load(face_cascade_name)) { printf("--(!)Error loading face cascade\n"); return -1; };

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPeople_recognitionDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPeople_recognitionDlgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPeople_recognitionDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPeople_recognitionDlgDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->ShowWindow(SW_HIDE);
	Register_faceDlg dlg;//����ӵĶԻ����ͷ�ļ�Ҫ�ӵ�ԭ�Ի����.cpp�ļ��� ������벻ͨ��
	dlg.DoModal();
	this->ShowWindow(SW_SHOW);
}

void CPeople_recognitionDlgDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
//	VideoCapture cap(0);    //��Ĭ������ͷ
	if (!cap.isOpened())
	{
		return;
	}
/*
	Mat frame;
	Mat edges;
	Mat gray;

	CascadeClassifier cascade;
*/
	//ѵ���õ��ļ����ƣ������ڿ�ִ���ļ�ͬĿ¼��
	cascade.load("haarcascade_frontalface_alt.xml");
	//if (!face_cascade.load(face_cascade_name)) { printf("--(!)Error loading face cascade\n"); return -1; };
	Ptr<FaceRecognizer> modelLBPH = Algorithm::load<LBPHFaceRecognizer>("G:\\Microsft_Visual_Studio\\Visuak_Studio_2015_Projects\\my_xml\\MyFaceLBPHModel.xml");

//	SetTimer(1, 15, NULL);//��ʱ��1����ʱʱ���֡��һ��??

	this->ShowWindow(SW_HIDE);
	Face_trainDlg dlg;//����ӵĶԻ����ͷ�ļ�Ҫ�ӵ�ԭ�Ի����.cpp�ļ��� ������벻ͨ��
	dlg.DoModal();
	this->ShowWindow(SW_SHOW);

}

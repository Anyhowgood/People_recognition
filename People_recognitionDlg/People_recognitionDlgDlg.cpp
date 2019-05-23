
// People_recognitionDlgDlg.cpp : 实现文件
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
CascadeClassifier face_cascade;   //定义人脸分类器

VideoCapture cap(0);    //打开默认摄像头
Mat frame;
Mat edges;
Mat gray;
//测试用图像与点

Point text_lb;


/*
加载人像类，检测基类作为全局变量
*/
CascadeClassifier cascade;
//Ptr<FaceRecognizer> modelLBPH = Algorithm::load<LBPHFaceRecognizer>("G:\\Microsft_Visual_Studio\\Visuak_Studio_2015_Projects\\my_xml\\MyFaceLBPHModel.xml");;
//Ptr<FaceRecognizer> modelLBPH= Algorithm::load<LBPHFaceRecognizer>("G:\\Microsft_Visual_Studio\\Visuak_Studio_2015_Projects\\my_xml\\MyFaceLBPHModel.xml");;

//建立用于存放人脸的向量容器
vector<Rect> faces(0);

int predictLBPH = 0;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CPeople_recognitionDlgDlg 对话框



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


// CPeople_recognitionDlgDlg 消息处理程序

BOOL CPeople_recognitionDlgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	AllocConsole(); //调试信息窗口的初始化函数
	if (!face_cascade.load(face_cascade_name)) { printf("--(!)Error loading face cascade\n"); return -1; };

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPeople_recognitionDlgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPeople_recognitionDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPeople_recognitionDlgDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ShowWindow(SW_HIDE);
	Register_faceDlg dlg;//你添加的对话框的头文件要加到原对话框的.cpp文件中 否则编译不通过
	dlg.DoModal();
	this->ShowWindow(SW_SHOW);
}

void CPeople_recognitionDlgDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	
//	VideoCapture cap(0);    //打开默认摄像头
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
	//训练好的文件名称，放置在可执行文件同目录下
	cascade.load("haarcascade_frontalface_alt.xml");
	//if (!face_cascade.load(face_cascade_name)) { printf("--(!)Error loading face cascade\n"); return -1; };
	Ptr<FaceRecognizer> modelLBPH = Algorithm::load<LBPHFaceRecognizer>("G:\\Microsft_Visual_Studio\\Visuak_Studio_2015_Projects\\my_xml\\MyFaceLBPHModel.xml");

//	SetTimer(1, 15, NULL);//定时器1，定时时间和帧率一致??

	this->ShowWindow(SW_HIDE);
	Face_trainDlg dlg;//你添加的对话框的头文件要加到原对话框的.cpp文件中 否则编译不通过
	dlg.DoModal();
	this->ShowWindow(SW_SHOW);

}

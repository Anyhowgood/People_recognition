
// People_recognitionDlgDlg.h : ͷ�ļ�
//
#include "CvvImage.h"
#include "Register_faceDlg.h"

#pragma once


// CPeople_recognitionDlgDlg �Ի���
class CPeople_recognitionDlgDlg : public CDialogEx
{
// ����
public:
	CPeople_recognitionDlgDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PEOPLE_RECOGNITIONDLG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#pragma once

#include "CvvImage.h"
#include "Register_faceDlg.h"
// Face_trainDlg 对话框

class Face_trainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(Face_trainDlg)

public:
	Face_trainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Face_trainDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FACE_TRAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
};

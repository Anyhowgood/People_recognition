#pragma once

#include "CvvImage.h"

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
// Register_faceDlg 对话框

class Register_faceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(Register_faceDlg)

public:  
	Register_faceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Register_faceDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REGISTER_FACEDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedCancel();
};

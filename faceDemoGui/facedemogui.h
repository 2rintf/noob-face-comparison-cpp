#pragma once

#include <QtWidgets/QMainWindow>
#include <qdebug.h>
#include <QtWidgets/qfiledialog.h>
#include <qmessagebox.h>

#include <opencv2/opencv.hpp>

#include "ui_facedemogui.h"
#include "DialogForAddNewFace.h"

#include "faceCompareWrap.h"

class faceDemoGui : public QMainWindow
{
	Q_OBJECT

public:
	faceDemoGui(QWidget *parent = Q_NULLPTR);

	std::vector<face_info> five_best_results;
	int result_flag = 0;
	bool isCompared = false;

	std::string lib_json_path = "simple_face_encoding_v2.json";

private:
	Ui::faceDemoGuiClass ui;
	QString upload_img_path;
	cv::Mat upload_img;

	void allClearOfInfoShow();
	void showResults(int flag);

private slots:

	void on_selectBtn_clicked();
	void on_matchBtn_clicked();
	void on_firstBtn_clicked();
	void on_secondBtn_clicked();
	void on_thirdBtn_clicked();
	void on_fourthBtn_clicked();
	void on_fifthBtn_clicked();
	//void on_addBtnTest_clicked();
	void on_addAction_clicked();
};

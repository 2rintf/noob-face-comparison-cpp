#include "facedemogui.h"

faceDemoGui::faceDemoGui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);



}

void faceDemoGui::on_selectBtn_clicked()
{
	allClearOfInfoShow();

	qDebug() << "select." << endl;
	upload_img_path = QFileDialog::getOpenFileName(this,
		"选取文件",
		".",
		"Image File(*.jpg *.png)");

	qDebug() << upload_img_path;
	if (upload_img_path.isNull())
	{
		QMessageBox msg;
		msg.setWindowTitle("Warning!");
		msg.setText("所选图片为空!请重新选择。");
		msg.exec();
	}
	else
	{
		cv::Mat img_cv = cv::imread(upload_img_path.toStdString());
		cv::cvtColor(img_cv, img_cv, cv::COLOR_BGR2RGB);
		cv::resize(img_cv, img_cv, cv::Size(ui.selectedPic->width(), ui.selectedPic->height()));
		QImage img_qt = QImage(img_cv.data, img_cv.cols, img_cv.rows, img_cv.depth(), QImage::Format_RGB888);
		ui.selectedPic->setPixmap(QPixmap::fromImage(img_qt));
		
	}
}

void faceDemoGui::allClearOfInfoShow()
{
	//todo: 清除获得的5个匹配结果

	ui.selectedPic->clear();
	ui.nameShow->clear();
	ui.sexShow->clear();
	ui.ageShow->clear();
	ui.phoneShow->clear();
	ui.emailShow->clear();
	ui.matchedPicShow->clear();
}

void faceDemoGui::on_matchBtn_clicked()
{
	qDebug() << "match." ;

}

void faceDemoGui::on_firstBtn_clicked()
{
	qDebug() << "first." << endl;
}

void faceDemoGui::on_secondBtn_clicked()
{
	qDebug() << "second." << endl;
}

void faceDemoGui::on_thirdBtn_clicked()
{
	qDebug() << "third." << endl;
}

void faceDemoGui::on_fourthBtn_clicked()
{
	qDebug() << "fourth." << endl;
}

void faceDemoGui::on_fifthBtn_clicked()
{
	qDebug() << "fifth." << endl;
}

void faceDemoGui::on_addBtnTest_clicked()
{
	DialogForAddNewFace dialog;
	int result = dialog.exec();
	qDebug() << result << endl;
	qDebug() << dialog.trans_test;
}
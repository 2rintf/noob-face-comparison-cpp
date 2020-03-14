#include "facedemogui.h"

faceDemoGui::faceDemoGui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void faceDemoGui::on_selectBtn_clicked()
{
	qDebug() << "select." << endl;
	upload_img_path = QFileDialog::getOpenFileName(this,
		"ѡȡ�ļ�",
		".",
		"Image File(*.jpg *.png)");

	qDebug() << upload_img_path;
	if (upload_img_path.isNull())
	{
		QMessageBox msg;
		msg.setWindowTitle("Warning!");
		msg.setText("��ѡͼƬΪ��!������ѡ��");
		msg.exec();
	}
}

void faceDemoGui::allClearOfInfoShow()
{
	ui.selectedPic->clear();
	ui.nameShow->clear();
	ui.sexShow->clear();
	ui.ageShow->clear();
	ui.phoneShow->clear();
	ui.emailShow->clear();
	ui.matchedPicShow->clear();

	//to-do: �����õ�5��ƥ����
}

void faceDemoGui::on_matchBtn_clicked()
{
	qDebug() << "match." << endl;
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
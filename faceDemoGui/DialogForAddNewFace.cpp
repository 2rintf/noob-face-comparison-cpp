#include "DialogForAddNewFace.h"



DialogForAddNewFace::DialogForAddNewFace(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

void DialogForAddNewFace::allInfoClear()
{

	ui.selectedPicShow->clear();
	ui.nameEdit->clear();
	ui.ageEdit->clear();
	ui.phoneEdit->clear();
	ui.emailEdit->clear();
}
void DialogForAddNewFace::on_OkBtn_clicked()
{
	try
	{
		//todo 业务
		add_face.name =  qstr2str(ui.nameEdit->text());
		add_face.age = ui.ageEdit->text().toInt();
		add_face.phone = ui.phoneEdit->text().toInt();
		add_face.name = qstr2str(ui.emailEdit->text());
		if (ui.femaleRadio->isChecked())
			add_face.sex = "female";
		else
			add_face.sex = "male";
		

	}
	catch (const std::exception&)
	{
		qDebug() << "shit";
	}
	this->accept();
}
void DialogForAddNewFace::on_CancelBtn_clicked()
{
	this->reject();
}

void DialogForAddNewFace::on_selectNewFaceBtn_clicked()
{
	allInfoClear();
	qDebug() << "select from dialog.";
	QString upload_img_path = QFileDialog::getOpenFileName(this,
		str2qstr("选取文件"),
		".",
		"Image File(*.jpg *.png)");

	qDebug() << upload_img_path;
	if (upload_img_path.isNull())
	{
		QMessageBox msg;
		msg.setWindowTitle("Warning!");
		msg.setText(str2qstr("所选图片为空!请重新选择。"));
		msg.exec();
	}
	else
	{
		cv::Mat img_cv = cv::imread(upload_img_path.toStdString());
		cv::cvtColor(img_cv, img_cv, cv::COLOR_BGR2RGB);
		cv::resize(img_cv, img_cv, cv::Size(ui.selectedPicShow->width(), ui.selectedPicShow->height()));
		QImage img_qt = QImage(img_cv.data, img_cv.cols, img_cv.rows, img_cv.step, QImage::Format_RGB888);
		ui.selectedPicShow->setPixmap(QPixmap::fromImage(img_qt));

		add_face.path = upload_img_path.toStdString();

	}

}

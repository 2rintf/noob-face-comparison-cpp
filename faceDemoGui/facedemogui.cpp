#include "facedemogui.h"


QString str2qstr(const std::string str)
{
	return QString::fromLocal8Bit(str.data());
}

std::string qstr2str(const QString qstr)
{
	QByteArray cdata = qstr.toLocal8Bit();
	return std::string(cdata);
}


faceDemoGui::faceDemoGui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.addAction, &QAction::triggered, this, &faceDemoGui::on_addAction_clicked);
	//! 初始化检测器和model.
	face_compare_alg::initialize();

}

void faceDemoGui::on_selectBtn_clicked()
{
	allClearOfInfoShow();

	qDebug() << "select." << endl;
	upload_img_path = QFileDialog::getOpenFileName(this,
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
		cv::resize(img_cv, img_cv, cv::Size(ui.selectedPic->width(), ui.selectedPic->height()));
		QImage img_qt = QImage(img_cv.data, img_cv.cols, img_cv.rows, img_cv.step, QImage::Format_RGB888);
		ui.selectedPic->setPixmap(QPixmap::fromImage(img_qt));
		
	}
}

void faceDemoGui::allClearOfInfoShow()
{
	//! 清除获得的5个匹配结果
	five_best_results.clear();
	result_flag = 0;
	isCompared = false;

	upload_img_path = "";

	ui.selectedPic->clear();
	ui.nameShow->clear();
	ui.sexShow->clear();
	ui.ageShow->clear();
	ui.phoneShow->clear();
	ui.emailShow->clear();
	ui.matchedPicShow->clear();
}

void faceDemoGui::showResults(int flag)
{
	ui.nameShow->setText(QString::fromStdString(five_best_results[flag].name));
	ui.sexShow->setText(QString::fromStdString(five_best_results[flag].sex));
	ui.ageShow->setText(QString::number(five_best_results[flag].age));
	ui.phoneShow->setText(QString::number(five_best_results[flag].phone));
	ui.emailShow->setText(QString::fromStdString(five_best_results[flag].email));

	try
	{
		cv::Mat result_pic_cv = cv::imread(five_best_results[flag].path);
		cv::cvtColor(result_pic_cv, result_pic_cv, cv::COLOR_BGR2RGB);
		cv::resize(result_pic_cv, result_pic_cv, cv::Size(ui.matchedPicShow->width(), ui.matchedPicShow->height()));
		QImage img_qt = QImage(result_pic_cv.data, result_pic_cv.cols, result_pic_cv.rows, result_pic_cv.step,QImage::Format_RGB888);
		ui.matchedPicShow->setPixmap(QPixmap::fromImage(img_qt));
	}
	catch (const std::exception&)
	{
		QMessageBox msg;
		msg.setWindowTitle("Warning!");
		msg.setText(str2qstr("图片路径为空,请检查图片库！只展示身份信息。"));
		msg.exec();
	}

}

void faceDemoGui::on_matchBtn_clicked()
{
	qDebug() << "match." ;

	std::string result_file_path = face_compare_alg::compare_face_on_json(qstr2str(upload_img_path), lib_json_path);
	cout << result_file_path << endl;
	five_best_results =  face_compare_alg::read_result_from_json(result_file_path, lib_json_path);

	isCompared = true;
	showResults(0);



}

void faceDemoGui::on_firstBtn_clicked()
{
	if(isCompared)
		showResults(0);
}

void faceDemoGui::on_secondBtn_clicked()
{
	if (isCompared)
		showResults(1);
}

void faceDemoGui::on_thirdBtn_clicked()
{
	if (isCompared)
		showResults(2);
}

void faceDemoGui::on_fourthBtn_clicked()
{
	if (isCompared)
		showResults(3);
}

void faceDemoGui::on_fifthBtn_clicked()
{
	if (isCompared)
		showResults(4);
}

void faceDemoGui::on_addBtnTest_clicked()
{
	DialogForAddNewFace dialog;
	int result = dialog.exec();
	qDebug() << result << endl;
	qDebug() << dialog.trans_test;
}

void faceDemoGui::on_addAction_clicked()
{
	qDebug() << "action.";
	DialogForAddNewFace dialog;
	int result = dialog.exec();
	qDebug() << result << endl;
	qDebug() << dialog.trans_test;
	qDebug() << str2qstr(dialog.add_face.name) << str2qstr(dialog.add_face.sex) << dialog.add_face.age << dialog.add_face.phone << endl;
}

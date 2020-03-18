#pragma once
#include <QtWidgets/QDialog>
#include <qdebug.h>
#include <qfile.h>
#include "ui_dialogforaddnewface.h"
#include <QtWidgets/qfiledialog.h>
#include <qmessagebox.h>

#include "faceCompareWrap.h"


class DialogForAddNewFace : public QDialog
{
	Q_OBJECT

public:
	DialogForAddNewFace(QWidget *parent = Q_NULLPTR);

	QString trans_test = "from dialog";

	face_info add_face;

private:
	Ui::DialogForAddNewFace ui;

	void getInput();
	void allInfoClear();

	QString str2qstr(const std::string str)
	{
		return QString::fromLocal8Bit(str.data());
	}

	std::string qstr2str(const QString qstr)
	{
		QByteArray cdata = qstr.toLocal8Bit();
		return std::string(cdata);
	}

private slots:
	void on_OkBtn_clicked();
	void on_CancelBtn_clicked();
	void on_selectNewFaceBtn_clicked();
};

#pragma once
#include <QtWidgets/QDialog>
#include <qdebug.h>
#include "ui_dialogforaddnewface.h"


class DialogForAddNewFace : public QDialog
{
	Q_OBJECT

public:
	DialogForAddNewFace(QWidget *parent = Q_NULLPTR);

	QString trans_test = "from dialog";

private:
	Ui::DialogForAddNewFace ui;

	void getInput();

private slots:
	void on_OkBtn_clicked();
	void on_CancelBtn_clicked();
	void on_selectNewFaceBtn_clicked();
};

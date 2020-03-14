#include "DialogForAddNewFace.h"

DialogForAddNewFace::DialogForAddNewFace(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}
void DialogForAddNewFace::on_OkBtn_clicked()
{
	this->accept();
}

void DialogForAddNewFace::on_CancelBtn_clicked()
{
	this->reject();
}

void DialogForAddNewFace::on_selectNewFaceBtn_clicked()
{
	qDebug() << "select from dialog.";

}

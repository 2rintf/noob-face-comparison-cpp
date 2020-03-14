#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_facedemogui.h"

class faceDemoGui : public QMainWindow
{
	Q_OBJECT

public:
	faceDemoGui(QWidget *parent = Q_NULLPTR);

private:
	Ui::faceDemoGuiClass ui;
};

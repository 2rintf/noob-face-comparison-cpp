#include "facedemogui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	faceDemoGui w;
	w.show();
	return a.exec();
}

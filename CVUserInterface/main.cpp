
/**
 *
 * Reza Adhitya Saputra (reza.adhitya.saputra@gmail.com)
 * Version: 2014
 *
 */

#include "stdafx.h"
#include "CVUserInterface.h"
#include <QtWidgets/QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QApplication::setStyle(QStyleFactory::create("fusion"));
	QPalette p;
	p = qApp->palette();
	p.setColor(QPalette::Button, QColor(200,200,200));
	qApp->setPalette(p);

	CVUserInterface w;
	w.show();
	return a.exec();
}




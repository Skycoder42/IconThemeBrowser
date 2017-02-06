#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QApplication::setApplicationName(TARGET);
	QApplication::setApplicationVersion(VERSION);
	QApplication::setOrganizationName(COMPANY);
	QApplication::setOrganizationDomain(QStringLiteral("de.skycoder42"));
	QApplication::setApplicationDisplayName(DISPLAY_NAME);
	QApplication::setWindowIcon(QIcon::fromTheme("view-list-icons"));

	MainWindow w;
	w.show();

	return a.exec();
}

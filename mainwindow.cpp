#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	loadThemeNames();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::loadThemeNames()
{
	foreach(auto searchPath, QIcon::themeSearchPaths()) {
		QDir searchDir(searchPath);
		searchDir.setFilter(QDir::AllDirs | QDir::Drives | QDir::NoDotAndDotDot | QDir::Readable);
		searchDir.setSorting(QDir::NoSort);
		foreach(auto entry, searchDir.entryInfoList()) {
			if(QFile::exists(QDir(entry.canonicalFilePath()).absoluteFilePath(QStringLiteral("index.theme"))))
				themes.insert(entry.fileName(), entry.filePath());
		}
	}

	ui->currentThemeComboBox->addItems(themes.keys());
	ui->currentThemeComboBox->setCurrentText(QIcon::themeName());
}

void MainWindow::on_addPathButton_clicked()
{

}

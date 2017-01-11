#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>
#include <QDir>
#include <QDebug>
#include <dialogmaster.h>
#include <QDirIterator>
#include <QProgressDialog>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->iconTreeView->sortByColumn(0, Qt::AscendingOrder);

	loadThemeNames();
	on_currentThemeComboBox_activated(QIcon::themeName());
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_addPathButton_clicked()//TODO list paths/remove paths
{
	auto path = DialogMaster::getText(this, tr("Enter an additional theme search path:"), tr("Add Theme path"));
	if(!path.isEmpty() && QDir(path).exists()) {
		QIcon::setThemeSearchPaths(QIcon::themeSearchPaths() << path);
		loadThemeNames();
	}
}

void MainWindow::on_currentThemeComboBox_activated(const QString &text)
{
	ui->iconTreeView->clear();
	QIcon::setThemeName(text);

	foreach (auto icon, iconNames)
		addIcon(icon);
	scanIcons(themes.value(text));

	ui->statusBar->showMessage(tr("Total Icons: %1").arg(ui->iconTreeView->topLevelItemCount()));
}

void MainWindow::on_iconSizeSpinBox_valueChanged(int iconSize)
{
	ui->iconTreeView->setIconSize({iconSize, iconSize});
}

void MainWindow::on_filterLineEdit_textChanged(const QString &text)
{
	auto matchItems = ui->iconTreeView->findItems(QLatin1Char('*') + text + QLatin1Char('*'), Qt::MatchWildcard);
	for(auto i = 0; i < ui->iconTreeView->topLevelItemCount(); i++) {
		auto item = ui->iconTreeView->topLevelItem(i);
		item->setHidden(!matchItems.contains(item));
	}
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

bool MainWindow::addIcon(const QString &name)
{
	auto icon = QIcon::fromTheme(name);
	if(!icon.isNull()) {
		auto item = new QTreeWidgetItem(ui->iconTreeView);
		item->setText(0, name);
		item->setIcon(0, icon);
		return true;
	} else
		return false;
}

void MainWindow::scanIcons(const QString &basePath)
{
	QDir iconDir(basePath);
	iconDir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);
	iconDir.setSorting(QDir::Name);
	QDirIterator iterator(iconDir, QDirIterator::FollowSymlinks | QDirIterator::Subdirectories);
	while(iterator.hasNext()) {
		iterator.next();
		auto iconName = iterator.fileInfo().baseName();
		if(iconNames.contains(iconName))
			continue;
		if(addIcon(iconName))
			iconNames.insert(iconName);
	}
}

void MainWindow::on_searchIconsButton_clicked()
{
	auto max = themes.size();
	auto values = themes.values();
	auto progress = DialogMaster::createProgress(this, tr("Scanning all themes for icon names"), max);
	progress->show();

	for(auto i = 0; i < max; i++) {
		progress->setValue(i);
		QApplication::processEvents();
		scanIcons(values[i]);
	}
	progress->setValue(max);

	progress->deleteLater();
	ui->statusBar->showMessage(tr("Total Icons: %1").arg(ui->iconTreeView->topLevelItemCount()));
}

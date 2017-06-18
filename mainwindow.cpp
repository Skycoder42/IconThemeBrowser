#include "editpathsdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>
#include <QDir>
#include <QDebug>
#include <dialogmaster.h>
#include <QDirIterator>
#include <QProgressDialog>
#include <QClipboard>
#include <QMimeData>
#include <QtConcurrent>
#include <QAction>
#include <QToolButton>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	countLabel(new QLabel(this)),
	modeGroup(new QActionGroup(this)),
	matchFlag(Qt::MatchWildcard),
	themes(),
	systemTheme(QIcon::themeName()),
	iconNames()
{
	ui->setupUi(this);
	addActions({
				   ui->actionZoomIn,
				   ui->actionZoomOut
			   });

	ui->iconTreeView->sortByColumn(0, Qt::AscendingOrder);
	auto seperator = new QAction(ui->iconTreeView);
	seperator->setSeparator(true);
	ui->iconTreeView->addActions({
									 ui->action_Copy_Icon_Name,
									 ui->actionCopy_Name,
									 ui->actionCopy_Icon,
									 seperator,
									 ui->action_Scan_for_more_icons
								 });
	ui->searchIconsButton->setDefaultAction(ui->action_Scan_for_more_icons);
	ui->addPathButton->addActions({
									  ui->actionAdd_theme_path,
									  ui->actionEdit_theme_paths
								  });
	ui->addPathButton->setDefaultAction(ui->actionAdd_theme_path);
	statusBar()->addWidget(countLabel);

	QMenu *selectMenu = new QMenu(tr("Filters"), this);
	selectMenu->setIcon(QIcon::fromTheme(QStringLiteral("dialog-filters")));
	selectMenu->addActions({
							   ui->actionMatch_Wildcard,
							   ui->actionMatch_Regular_Expression,
							   ui->actionMatch_Contains,
							   ui->actionMatch_Starts_with,
							   ui->actionMatch_Ends_with,
							   ui->actionMatch_Exactly
						   });
	ui->filterLineEdit->addAction(selectMenu->menuAction(), QLineEdit::TrailingPosition);

	modeGroup->setExclusive(true);
	modeGroup->addAction(ui->actionMatch_Wildcard);
	modeGroup->addAction(ui->actionMatch_Regular_Expression);
	modeGroup->addAction(ui->actionMatch_Contains);
	modeGroup->addAction(ui->actionMatch_Starts_with);
	modeGroup->addAction(ui->actionMatch_Ends_with);
	modeGroup->addAction(ui->actionMatch_Exactly);
	connect(modeGroup, &QActionGroup::triggered,
			this, &MainWindow::modeChanged);

	QSettings settings;
	auto paths = settings.value(QStringLiteral("themePaths")).toStringList();
	foreach (auto path, QIcon::themeSearchPaths()) {
		if(!paths.contains(path))
			paths.append(path);
	}
	QIcon::setThemeSearchPaths(paths);
	ui->iconSizeSpinBox->setValue(settings.value(QStringLiteral("iconSize"), 16).toInt());

	auto size = settings.beginReadArray(QStringLiteral("iconNames"));
	for(auto i = 0; i < size; i++) {
		settings.setArrayIndex(i);
		iconNames.insert(settings.value(QStringLiteral("n")).toString());
	}
	settings.endArray();

	loadThemeNames();
	ui->currentThemeComboBox->setCurrentText(settings.value(QStringLiteral("theme"), tr("System Theme")).toString());
	QMetaObject::invokeMethod(this, "on_currentThemeComboBox_activated", Qt::QueuedConnection,
							  Q_ARG(QString, ui->currentThemeComboBox->currentText()));
}

MainWindow::~MainWindow()
{
	QSettings settings;
	settings.setValue(QStringLiteral("themePaths"), QIcon::themeSearchPaths());
	settings.setValue(QStringLiteral("theme"), ui->currentThemeComboBox->currentText());
	settings.setValue(QStringLiteral("iconSize"), ui->iconSizeSpinBox->value());

	auto list = iconNames.toList();
	auto size = list.size();
	settings.beginWriteArray(QStringLiteral("iconNames"), size);
	for(auto i = 0; i < size; i++) {
		settings.setArrayIndex(i);
		settings.setValue(QStringLiteral("n"), list[i]);
	}
	settings.endArray();

	delete ui;
}

void MainWindow::updateCount()
{
	countLabel->setText(tr("Total Icons: %1").arg(ui->iconTreeView->topLevelItemCount()));
	modeChanged(modeGroup->checkedAction());
}

void MainWindow::createTreeItem(QString name, QIcon icon)
{
	auto item = new QTreeWidgetItem(ui->iconTreeView);
	item->setText(0, name);
	item->setIcon(0, icon);
}

void MainWindow::modeChanged(QAction *action)
{
	if(action == ui->actionMatch_Wildcard)
		matchFlag = Qt::MatchWildcard;
	else if(action == ui->actionMatch_Regular_Expression)
		matchFlag = Qt::MatchRegExp;
	else if(action == ui->actionMatch_Contains)
		matchFlag = Qt::MatchContains;
	else if(action == ui->actionMatch_Starts_with)
		matchFlag = Qt::MatchStartsWith;
	else if(action == ui->actionMatch_Ends_with)
		matchFlag = Qt::MatchEndsWith;
	else if(action == ui->actionMatch_Exactly)
		matchFlag = Qt::MatchExactly;
	on_filterLineEdit_textChanged(ui->filterLineEdit->text());
}

void MainWindow::on_currentThemeComboBox_activated(QString text)
{
	ui->iconTreeView->clear();
	if(text == tr("System Theme"))
		text = systemTheme;
	QIcon::setThemeName(text);
	auto progress = DialogMaster::createProgress(this, tr("Loading icons for theme"));
	progress->setAttribute(Qt::WA_DeleteOnClose, true);
	progress->show();

	QtConcurrent::run([=](){
		foreach (auto icon, iconNames) {
			if(progress->wasCanceled())
				break;
			addIcon(icon);
		}
		scanIcons(themes.value(text), progress);
		QMetaObject::invokeMethod(this, "updateCount");
		QMetaObject::invokeMethod(progress, "close");
	});
}

void MainWindow::on_iconSizeSpinBox_valueChanged(int iconSize)
{
	ui->iconTreeView->setIconSize({iconSize, iconSize});
}

void MainWindow::on_filterLineEdit_textChanged(QString text)
{
	if(!text.isEmpty()) {
		if(matchFlag == Qt::MatchWildcard)
			text = QLatin1Char('*') + text + QLatin1Char('*');
		auto matchItems = ui->iconTreeView->findItems(text, matchFlag);
		for(auto i = 0; i < ui->iconTreeView->topLevelItemCount(); i++) {
			auto item = ui->iconTreeView->topLevelItem(i);
			item->setHidden(!matchItems.contains(item));
		}
	} else {
		for(auto i = 0; i < ui->iconTreeView->topLevelItemCount(); i++) {
			auto item = ui->iconTreeView->topLevelItem(i);
			item->setHidden(false);
		}
	}
}

void MainWindow::on_iconTreeView_itemActivated(QTreeWidgetItem *item)
{
	doCopy(item);
	statusBar()->showMessage(tr("Copied name and icon of %1!").arg(item->text(0)), 5000);
}

void MainWindow::on_action_Copy_Icon_Name_triggered()
{
	auto item = ui->iconTreeView->currentItem();
	if(item) {
		doCopy(item);
		statusBar()->showMessage(tr("Copied name and icon of %1!").arg(item->text(0)), 5000);
	}
}

void MainWindow::on_actionCopy_Name_triggered()
{
	auto item = ui->iconTreeView->currentItem();
	if(item) {
		doCopy(item, true, false);
		statusBar()->showMessage(tr("Copied name of %1!").arg(item->text(0)), 5000);
	}
}

void MainWindow::on_actionCopy_Icon_triggered()
{
	auto item = ui->iconTreeView->currentItem();
	if(item) {
		doCopy(item, false, true);
		statusBar()->showMessage(tr("Copied icon of %1!").arg(item->text(0)), 5000);
	}
}

void MainWindow::on_action_Scan_for_more_icons_triggered()
{
	auto max = themes.size();
	auto values = themes.values();
	auto progress = DialogMaster::createProgress(this, tr("Scanning all themes for icon names"), max);
	progress->setAttribute(Qt::WA_DeleteOnClose, true);
	progress->setAutoClose(true);
	progress->setAutoReset(true);
	progress->show();

	QtConcurrent::run([=](){
		for(auto i = 0; i < max; i++) {
			if(progress->wasCanceled())
				break;
			scanIcons(values[i], progress);
			QMetaObject::invokeMethod(progress, "setValue", Q_ARG(int, i + 1));
		}
		QMetaObject::invokeMethod(this, "updateCount");
	});
}

void MainWindow::on_actionAdd_theme_path_triggered()
{
	auto path = DialogMaster::getText(this, tr("Enter an additional theme search path:"), tr("Add Theme path"));
	if(!path.isEmpty() && QDir(path).exists()) {
		QIcon::setThemeSearchPaths(QIcon::themeSearchPaths() << path);
		loadThemeNames();
	}
}

void MainWindow::on_actionEdit_theme_paths_triggered()
{
	if(EditPathsDialog::editIconPaths(this))
		loadThemeNames();
}

void MainWindow::on_actionZoomIn_triggered()
{
	ui->iconSizeSpinBox->stepUp();
}

void MainWindow::on_actionZoomOut_triggered()
{
	ui->iconSizeSpinBox->stepDown();
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

	auto names = themes.keys();
	names.prepend(tr("System Theme"));
	auto cText = ui->currentThemeComboBox->currentText();
	ui->currentThemeComboBox->clear();
	ui->currentThemeComboBox->addItems(names);
	ui->currentThemeComboBox->setCurrentText(cText);
}

bool MainWindow::addIcon(const QString &name)
{
	auto icon = QIcon::fromTheme(name);
	if(!icon.isNull()) {
		QMetaObject::invokeMethod(this, "createTreeItem", Q_ARG(QString, name), Q_ARG(QIcon, icon));
		return true;
	} else
		return false;
}

void MainWindow::scanIcons(const QString &basePath, QProgressDialog *progress)
{
	QDir iconDir(basePath);
	iconDir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);
	iconDir.setSorting(QDir::Name);
	QDirIterator iterator(iconDir, QDirIterator::FollowSymlinks | QDirIterator::Subdirectories);
	while(iterator.hasNext()) {
		iterator.next();
		if(progress->wasCanceled())
			break;

		auto iconName = iterator.fileInfo().baseName();
		if(iconNames.contains(iconName))
			continue;
		if(addIcon(iconName))
			iconNames.insert(iconName);
	}
}

void MainWindow::doCopy(QTreeWidgetItem *item, bool name, bool icon)
{
	auto clip = QApplication::clipboard();
	auto mime = new QMimeData();
	if(name)
		mime->setText(item->text(0));
	if(icon) {
		auto size = ui->iconSizeSpinBox->value();
		mime->setImageData(item->icon(0).pixmap({size, size}).toImage());
	}
	clip->setMimeData(mime);
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QMap>
#include <QProgressDialog>
#include <QSet>
#include <QTreeWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void updateCount();
	void createTreeItem(QString name, QIcon icon);

	void on_currentThemeComboBox_activated(const QString &text);
	void on_iconSizeSpinBox_valueChanged(int iconSize);
	void on_filterLineEdit_textChanged(const QString &text);
	void on_iconTreeView_itemActivated(QTreeWidgetItem *item);
	void on_action_Copy_Icon_Name_triggered();
	void on_actionCopy_Name_triggered();
	void on_actionCopy_Icon_triggered();
	void on_action_Scan_for_more_icons_triggered();
	void on_actionAdd_theme_path_triggered();
	void on_actionEdit_theme_paths_triggered();

private:
	Ui::MainWindow *ui;
	QLabel *countLabel;
	QMap<QString, QString> themes;
	QSet<QString> iconNames;

	void loadThemeNames();
	bool addIcon(const QString &name);
	void scanIcons(const QString &basePath, QProgressDialog *progress);
	void doCopy(QTreeWidgetItem *item, bool name = true, bool icon = true);
};

#endif // MAINWINDOW_H

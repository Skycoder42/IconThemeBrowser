#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QMap>
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
	void on_addPathButton_clicked();
	void on_currentThemeComboBox_activated(const QString &text);
	void on_iconSizeSpinBox_valueChanged(int iconSize);
	void on_filterLineEdit_textChanged(const QString &text);
	void on_searchIconsButton_clicked();
	void on_iconTreeView_itemActivated(QTreeWidgetItem *item);
	void on_action_Copy_Icon_Name_triggered();
	void on_actionCopy_Name_triggered();
	void on_actionCopy_Icon_triggered();

private:
	Ui::MainWindow *ui;
	QLabel *countLabel;
	QMap<QString, QString> themes;
	QSet<QString> iconNames;

	void loadThemeNames();
	bool addIcon(const QString &name);
	void scanIcons(const QString &basePath);
	void doCopy(QTreeWidgetItem *item, bool name = true, bool icon = true);
	void updateCount();
};

#endif // MAINWINDOW_H

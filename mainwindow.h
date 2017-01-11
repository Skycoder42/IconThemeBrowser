#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QSet>

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

private:
	Ui::MainWindow *ui;
	QMap<QString, QString> themes;
	QSet<QString> iconNames;

	void loadThemeNames();
	bool addIcon(const QString &name);
	void scanIcons(const QString &basePath);
};

#endif // MAINWINDOW_H

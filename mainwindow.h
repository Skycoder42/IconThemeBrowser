#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

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

private:
	Ui::MainWindow *ui;
	QMap<QString, QString> themes;

	void loadThemeNames();
};

#endif // MAINWINDOW_H

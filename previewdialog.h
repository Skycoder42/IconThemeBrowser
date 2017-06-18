#ifndef PREVIEWDIALOG_H
#define PREVIEWDIALOG_H

#include <QDialog>

namespace Ui {
class PreviewDialog;
}

class PreviewDialog : public QDialog
{
	Q_OBJECT

public:
	static void previewIcon(const QString &iconName, const QStringList &themeNames, QWidget *parent = nullptr);

private slots:
	void on_sizeSpinBox_valueChanged(int size);

private:
	Ui::PreviewDialog *ui;
	QString iconName;
	QStringList themeNames;

	explicit PreviewDialog(const QString &iconName, const QStringList &themeNames, QWidget *parent = nullptr);
	~PreviewDialog();
};

#endif // PREVIEWDIALOG_H

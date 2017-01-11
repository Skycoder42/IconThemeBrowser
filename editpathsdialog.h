#ifndef EDITPATHSDIALOG_H
#define EDITPATHSDIALOG_H

#include <QDialog>
#include <QStringListModel>

namespace Ui {
class EditPathsDialog;
}

class EditPathsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit EditPathsDialog(QWidget *parent = nullptr);
	~EditPathsDialog();

	static bool editIconPaths(QWidget *parent = nullptr);

private slots:
	void on_action_Delete_path_triggered();

	void on_action_Add_Path_triggered();

private:
	Ui::EditPathsDialog *ui;
	QStringListModel *model;
};

#endif // EDITPATHSDIALOG_H

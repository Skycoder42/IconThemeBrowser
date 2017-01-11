#include "editpathsdialog.h"
#include "ui_editpathsdialog.h"
#include <dialogmaster.h>

EditPathsDialog::EditPathsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::EditPathsDialog),
	model(new QStringListModel(this))
{
	ui->setupUi(this);
	DialogMaster::masterDialog(this, true);

	ui->listView->addActions({
								 ui->action_Add_Path,
								 ui->action_Delete_path
							 });
	ui->addButton->setDefaultAction(ui->action_Add_Path);
	ui->listView->setModel(model);
}

EditPathsDialog::~EditPathsDialog()
{
	delete ui;
}

bool EditPathsDialog::editIconPaths(QWidget *parent)
{
	EditPathsDialog dialog(parent);
	dialog.model->setStringList(QIcon::themeSearchPaths());
	if(dialog.exec() == Accepted) {
		QIcon::setThemeSearchPaths(dialog.model->stringList());
		return true;
	} else
		return false;
}

void EditPathsDialog::on_action_Delete_path_triggered()
{
	auto index = ui->listView->currentIndex();
	if(index.isValid())
		model->removeRow(index.row());
}

void EditPathsDialog::on_action_Add_Path_triggered()
{
	auto path = DialogMaster::getText(this, tr("Enter an additional theme search path:"), tr("Add Theme path"));
	if(!path.isEmpty() && QDir(path).exists()) {
		auto row = model->rowCount();
		if(model->insertRow(row))
			model->setData(model->index(row), path);
	}
}

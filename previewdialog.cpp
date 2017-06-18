#include "previewdialog.h"
#include "ui_previewdialog.h"
#include "dialogmaster.h"

#include <QIcon>

PreviewDialog::PreviewDialog(const QString &iconName, const QStringList &themeNames, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PreviewDialog),
	iconName(iconName),
	themeNames(themeNames)
{
	ui->setupUi(this);
	DialogMaster::masterDialog(this);
	on_sizeSpinBox_valueChanged(ui->sizeSpinBox->value());
}

PreviewDialog::~PreviewDialog()
{
	delete ui;
}

void PreviewDialog::previewIcon(const QString &iconName, const QStringList &themeNames, QWidget *parent)
{
	PreviewDialog dialog(iconName, themeNames, parent);
	dialog.exec();
}

void PreviewDialog::on_sizeSpinBox_valueChanged(int size)
{
	ui->listWidget->clear();
	ui->listWidget->setIconSize(QSize(size, size));
	ui->listWidget->setGridSize(QSize(qMax(size + 8, 64), size + ui->listWidget->fontMetrics().height() + 8));
	auto currentTheme = QIcon::themeName();
	foreach(auto theme, themeNames) {
		QIcon::setThemeName(theme);
		auto icon = QIcon::fromTheme(iconName).pixmap(size);
		if(!icon.isNull()) {
			auto item = new QListWidgetItem(icon, theme, ui->listWidget);
			item->setToolTip(theme);
		}
	}
	QIcon::setThemeName(currentTheme);
}

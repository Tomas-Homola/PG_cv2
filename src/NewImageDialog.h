#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets>

#include "ui_NewImgDialog.h"

class NewImageDialog : public QDialog
{
	Q_OBJECT

public:
	NewImageDialog(QWidget* parent = Q_NULLPTR) : QDialog(parent), newImgUi(new Ui::DialogNewImg)
	{
		newImgUi->setupUi(this);
	};
	int getWidth() { return newImgUi->spinBoxNewImgWidth->value(); }
	int getHeight() { return newImgUi->spinBoxNewImgHeight->value(); }
	QString getName() { return newImgUi->lineEdit->text(); }
private:
	Ui::DialogNewImg* newImgUi;
};
#ifndef DIALOG_H
#define DIALOG_H
#include <QDialog>
#include "RTreeAppTest.h"

class QLabel;
class QComboBox;
class QLineEdit;
class QPushButton;
class QListWidget;
class QHBoxLayout;
class QVBoxLayout;
class QImage;

class Dialog : public QDialog
{
	Q_OBJECT
public:
	Dialog(QWidget *parent = 0);
	~Dialog();
protected slots:
	void changeDimensionCombo(const QString &x);
	void enableBuildTreeButton();
	bool buildTree();
	void loadQuery();
	void searchImage();
private:
	QLabel *objectLabel, *featureLabel, *dimensionLabel, *searchNumberLabel, *imageLabel;
	QComboBox *objectCombo, *featureCombo, *dimensionCombo;
	QLineEdit *searchNumberEdit;
	QPushButton *buildTreeButton, *imageButton, *searchButton, *exitButton;
	QListWidget *resultList;
	QHBoxLayout *objectLayout, *featureLayout, *dimensionLayout, *searchNumberLayout, *buildTreeLayout, *imageLayout, *imageButtonLayout, *topLayout, *bottomLayout, *mainLayout;
	QVBoxLayout *leftLayout, *rightLayout, *mainLeftLayout, *mainRightLayout;
	QString *query;
	RTreeAppTest<8> *tree8;
	RTreeAppTest<9> *tree9;
	RTreeAppTest<12> *tree12;
	RTreeAppTest<16> *tree16;
	RTreeAppTest<20> *tree20;
	RTreeAppTest<32> *tree32;
	RTreeAppTest<64> *tree64;
};

#endif


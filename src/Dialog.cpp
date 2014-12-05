#include <QtGui>
#include "Dialog.h"

Dialog::Dialog(QWidget *parent)
	: QDialog(parent)
{
	QStringList objectList, featureList, dimensionList;

	objectLabel = new QLabel(tr("Number of objects:"));
	objectCombo = new QComboBox;
	objectList << tr("1000") << tr("2000") << tr("3000") << tr("4000") << tr("5000");
	objectCombo->addItems(objectList);
	objectLayout = new QHBoxLayout;
	objectLayout->addWidget(objectLabel);
	objectLayout->addWidget(objectCombo);

	featureLabel = new QLabel(tr("Feature type:"));
	featureCombo = new QComboBox;
	featureList << tr("Color moment") << tr("Color histogram") << tr("Color correlogram");
	featureCombo->addItems(featureList);
	featureLayout = new QHBoxLayout;
	featureLayout->addWidget(featureLabel);
	featureLayout->addWidget(featureCombo);

	dimensionLabel = new QLabel(tr("Number of dimension:"));
	dimensionCombo = new QComboBox;
	dimensionList << tr("9");
	dimensionCombo->addItems(dimensionList);
	dimensionLayout = new QHBoxLayout;
	dimensionLayout->addWidget(dimensionLabel);
	dimensionLayout->addWidget(dimensionCombo);

	searchNumberLabel = new QLabel(tr("Number to search:"));
	searchNumberEdit = new QLineEdit;
	searchNumberLayout = new QHBoxLayout;
	searchNumberLayout->addWidget(searchNumberLabel);
	searchNumberLayout->addWidget(searchNumberEdit);

	buildTreeButton = new QPushButton(tr("Build R-tree"));
	buildTreeLayout = new QHBoxLayout;
	buildTreeLayout->addStretch();
	buildTreeLayout->addWidget(buildTreeButton);
	buildTreeLayout->addStretch();

	leftLayout = new QVBoxLayout;
	leftLayout->addLayout(objectLayout);
	leftLayout->addLayout(featureLayout);
	leftLayout->addLayout(dimensionLayout);
	leftLayout->addLayout(buildTreeLayout);
	leftLayout->addStretch();
	
	imageLabel = new QLabel;
	imageLabel->setStyleSheet(tr("QLabel{border:1px solid black}"));

	imageLayout = new QHBoxLayout;
	imageLayout->addWidget(imageLabel);

	imageButton = new QPushButton(tr("Load query"));
	imageButtonLayout = new QHBoxLayout;
	imageButtonLayout->addStretch();
	imageButtonLayout->addWidget(imageButton);
	imageButtonLayout->addStretch();

	rightLayout = new QVBoxLayout;
	rightLayout->addLayout(searchNumberLayout);
	rightLayout->addWidget(imageLabel);
	rightLayout->addLayout(imageButtonLayout);

	topLayout = new QHBoxLayout;
	topLayout->addLayout(leftLayout);
	topLayout->addLayout(rightLayout);

	searchButton = new QPushButton(tr("Search"));
	exitButton = new QPushButton(tr("Exit"));
	bottomLayout = new QHBoxLayout;
	bottomLayout->addStretch();
	bottomLayout->addWidget(searchButton);
	bottomLayout->addStretch();
	bottomLayout->addWidget(exitButton);
	bottomLayout->addStretch();

	mainLeftLayout = new QVBoxLayout;
	mainLeftLayout->addLayout(topLayout);
	mainLeftLayout->addLayout(bottomLayout);

	resultList = new QListWidget;
	resultList->setFixedWidth(260);
	resultList->setViewMode(QListView::IconMode);
	resultList->setIconSize(QSize(80, 80));
	resultList->addItem(tr("Search result:"));
	mainRightLayout = new QVBoxLayout;
	mainRightLayout->addWidget(resultList);

	mainLayout = new QHBoxLayout;
	mainLayout->addLayout(mainLeftLayout);
	mainLayout->addLayout(mainRightLayout);

	setLayout(mainLayout);
	resize(800, 400);

	query = new QString;
	tree8 = new RTreeAppTest<8>;
	tree9 = new RTreeAppTest<9>;
	tree12 = new RTreeAppTest<12>;
	tree16 = new RTreeAppTest<16>;
	tree20 = new RTreeAppTest<20>;
	tree32 = new RTreeAppTest<32>;
	tree64 = new RTreeAppTest<64>;

	connect(objectCombo, SIGNAL(activated(const QString &)), this, SLOT(enableBuildTreeButton()));
	connect(featureCombo, SIGNAL(activated(const QString &)), this, SLOT(enableBuildTreeButton()));
	connect(dimensionCombo, SIGNAL(activated(const QString &)), this, SLOT(enableBuildTreeButton()));
	connect(featureCombo, SIGNAL(activated(const QString &)), this, SLOT(changeDimensionCombo(const QString &)));
	connect(buildTreeButton, SIGNAL(clicked()), this, SLOT(buildTree()));
	connect(imageButton, SIGNAL(clicked()), this, SLOT(loadQuery()));
	connect(searchButton, SIGNAL(clicked()), this, SLOT(searchImage()));
	connect(exitButton, SIGNAL(clicked()), qApp, SLOT(quit()));
}

Dialog::~Dialog()
{
	delete objectLabel;
	delete objectCombo;
	delete objectLayout;
	delete featureLabel;
	delete featureCombo;
	delete featureLayout;
	delete dimensionLabel;
	delete dimensionCombo;
	delete dimensionLayout;
	delete searchNumberLabel;
	delete searchNumberEdit;
	delete searchNumberLayout;
	delete buildTreeButton;
	delete buildTreeLayout;
	delete leftLayout;
	delete imageLabel;
	delete imageLayout;
	delete imageButton;
	delete imageButtonLayout;
	delete rightLayout;
	delete topLayout;
	delete searchButton;
	delete exitButton;
	delete bottomLayout;
	delete mainLeftLayout;
	delete resultList;
	delete mainRightLayout;
	delete mainLayout;
	delete query;
	delete tree8;
	delete tree9;
	delete tree12;
	delete tree16;
	delete tree20;
	delete tree32;
	delete tree64;
}

void Dialog::enableBuildTreeButton()
{
	buildTreeButton->setEnabled(true);
}

void Dialog::changeDimensionCombo(const QString &x)
{
	QStringList dimensionList;

	while (dimensionCombo->count() > 0)
		dimensionCombo->removeItem(0);
	if (x == tr("Color moment"))
		dimensionList << tr("9");
	else
	if (x == tr("Color histogram"))
		dimensionList << tr("12") << tr("20") << tr("32");
	else
		dimensionList << tr("8") << tr("16") << tr("64");
	dimensionCombo->addItems(dimensionList);
}

bool Dialog::buildTree()
{
	bool ok;
	int object = objectCombo->currentText().toInt(&ok, 10);
	int dimension = dimensionCombo->currentText().toInt(&ok, 10);
	int feature;
	if (featureCombo->currentText() == tr("Color moment"))
		feature = 0;
	else
	if (featureCombo->currentText() == tr("Color histogram"))
		feature = 1;
	else
		feature = 2;

	switch (dimension)
	{
	case 8:
		ok = tree8->mb_createRTree(object, feature);
		break;
	case 9:
		ok = tree9->mb_createRTree(object, feature);
		break;
	case 12:
		ok = tree12->mb_createRTree(object, feature);
		break;
	case 16:
		ok = tree16->mb_createRTree(object, feature);
		break;
	case 20:
		ok = tree20->mb_createRTree(object, feature);
		break;
	case 32:
		ok = tree32->mb_createRTree(object, feature);
		break;
	case 64:
		ok = tree64->mb_createRTree(object, feature);
		break;
	}

	if (!ok)
	{
		QMessageBox::warning(this, tr("ImageSearch"), tr("R-tree built failed. Please make sure the image lists and feature lists exist in the program's directory."));
		return false;
	}

	QMessageBox::information(this, tr("ImageSearch"), tr("R-tree built successfully."));
	buildTreeButton->setEnabled(false);
	return true;
}

void Dialog::loadQuery()
{
	QImage image;
	QString t;

	t = QFileDialog::getOpenFileName(this, tr("Open an image."), ".", tr("JPEG files (*.jpg *.jpeg)\n""All files (*.*)"));
	if (!t.isEmpty())
		if (image.load(t))
		{
			*query = t;
			imageLabel->setPixmap(QPixmap::fromImage(image).scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
		}
		else
			QMessageBox::warning(this, tr("ImageSearch"), tr("Image loading failed."));
}

void Dialog::searchImage()
{
	bool ok;
	int k = searchNumberEdit->text().toInt(&ok, 10);
	if ((k <= 0) || (!ok))
	{
		QMessageBox::warning(this, tr("ImageSearch"), tr("The number to search must be an integer no less than 1."));
		return;
	}

	if (query->isNull())
	{
		QMessageBox::warning(this, tr("ImageSearch"), tr("Please load an image as query."));
		return;
	}

	if (buildTreeButton->isEnabled())
		if (!buildTree())
			return;

	while (resultList->count() > 1)
	{
		QListWidgetItem *t = resultList->takeItem(1);
		delete t;
	}

	QStringList filenameList;
	int dimension = dimensionCombo->currentText().toInt(&ok, 10);
	double rate;
	int searchCount;

	switch (dimension)
	{
	case 8:
		rate = tree8->mb_singleQuery(*query, filenameList, searchCount, k);
		break;
	case 9:
		rate = tree9->mb_singleQuery(*query, filenameList, searchCount, k);
		break;
	case 12:
		rate = tree12->mb_singleQuery(*query, filenameList, searchCount, k);
		break;
	case 16:
		rate = tree16->mb_singleQuery(*query, filenameList, searchCount, k);
		break;
	case 20:
		rate = tree20->mb_singleQuery(*query, filenameList, searchCount, k);
		break;
	case 32:
		rate = tree32->mb_singleQuery(*query, filenameList, searchCount, k);
		break;
	case 64:
		rate = tree64->mb_singleQuery(*query, filenameList, searchCount, k);
		break;
	}

	foreach (QString fileName, filenameList)
	{
		fileName = tr("./image/") + fileName;
		resultList->addItem(new QListWidgetItem(QIcon(fileName), fileName));
	}

	QMessageBox::information(this, tr("ImageSearch"), tr("Exact rate:") + QString::number(rate * 100) + tr("%"));
}


#include   "ViewerWidget.h"

void ViewerWidget::drawDDAHorizontalLine(QPoint point1, QPoint point2, QColor color)
{
	int x1 = 0;
	int x2 = 0;
	int y = point1.y();

	if (point1.x() < point2.x())
	{
		x1 = point1.x();
		x2 = point2.x();
	}
	else if (point1.x() > point2.x())
	{
		x1 = point2.x();
		x2 = point1.x();
	}

	while (x1 <= x2)
	{
		setPixel(x1, y, color);
		x1++;
	}

	update();
}

void ViewerWidget::drawDDAVerticalLine(QPoint point1, QPoint point2, QColor color)
{
	int x = point1.x();
	int y1 = 0;
	int y2 = 0;

	if (point1.y() < point2.y())
	{
		y1 = point1.y();
		y2 = point2.y();
	}
	else if (point1.y() > point2.y())
	{
		y1 = point2.y();
		y2 = point1.y();
	}

	while (y1 <= y2)
	{
		setPixel(x, y1, color);
		y1++;
	}

	update();
}

void ViewerWidget::drawDDAChosenX(QPoint point1, QPoint point2, QColor color)
{
	double slope = (static_cast<double>(point2.y()) - static_cast<double>(point1.y())) / (static_cast<double>(point2.x()) - static_cast<double>(point1.x()));
	
	int x = point1.x();
	double y = static_cast<double>(point1.y());

	setPixel(x, y, color);

	while (x < point2.x())
	{
		x++;
		y += slope;
		setPixel(x, static_cast<int>(y), color);
	}

	update();
}

void ViewerWidget::drawDDAChosenY(QPoint point1, QPoint point2, QColor color)
{
	if (point1.y() > point2.y())
	{
		int temp = point1.x();
		point1.setX(point2.x());
		point2.setX(temp);

		temp = point1.y();
		point1.setY(point2.y());
		point2.setY(temp);
	}
	
	double slope = (static_cast<double>(point2.y()) - static_cast<double>(point1.y())) / (static_cast<double>(point2.x()) - static_cast<double>(point1.x()));

	double x = static_cast<double>(point1.x());
	int y = point1.y();

	setPixel(x, y, color);

	while (y < point2.y())
	{
		y++;
		x += 1 / slope;
		setPixel(static_cast<int>(x), y, color);
	}

	update();
}

ViewerWidget::ViewerWidget(QString viewerName, QSize imgSize, QWidget* parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StaticContents);
	setMouseTracking(true);
	name = viewerName;
	if (imgSize != QSize(0, 0)) {
		img = new QImage(imgSize, QImage::Format_ARGB32);
		img->fill(Qt::white);
		resizeWidget(img->size());
		setPainter();
		setDataPtr();
	}
}
ViewerWidget::~ViewerWidget()
{
	delete painter;
	delete img;
}
void ViewerWidget::resizeWidget(QSize size)
{
	this->resize(size);
	this->setMinimumSize(size);
	this->setMaximumSize(size);
}

//Image functions
bool ViewerWidget::setImage(const QImage& inputImg)
{
	if (img != nullptr) {
		delete img;
	}
	img = new QImage(inputImg);
	if (!img) {
		return false;
	}
	resizeWidget(img->size());
	setPainter();
	update();

	return true;
}
bool ViewerWidget::isEmpty()
{
	if (img->size() == QSize(0, 0)) {
		return true;
	}
	return false;
}

//Data function
void ViewerWidget::setPixel(int x, int y, const QColor& color)
{
	if (isInside(x, y)) {
		data[x + y * img->width()] = color.rgb();
	}
}
void ViewerWidget::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
	if (isInside(x, y)) {
		QColor color(r, g, b);
		setPixel(x, y, color);
	}
}

void ViewerWidget::clear(QColor color)
{
	for (size_t x = 0; x < img->width(); x++)
	{
		for (size_t y = 0; y < img->height(); y++)
		{
			setPixel(x, y, color);
		}
	}
	update();
}

void ViewerWidget::drawLineDDA(QPoint point1, QPoint point2, QColor color)
{
	char chosenAxis = 'x';

	if (point1.x() > point2.x()) // ak by bol zadany prvy bod viac v pravo
	{
		int temp = point1.x();
		point1.setX(point2.x());
		point2.setX(temp);

		temp = point1.y();
		point1.setY(point2.y());
		point2.setY(temp);
	}

	//qDebug() << "DDA\nstartPoint:" << point1 << "\nendPoint" << point2;

	int deltaX = point2.x() - point1.x();
	int deltaY = point2.y() - point1.y();
	double slope = 0.0;

	if (deltaX == 0) // ak je rozdiel x suradnic 0 -> vertikalna usecka
		drawDDAVerticalLine(point1, point2, color);
	else // ak ten rozdiel nie je 0 -> mozme delit
		slope = static_cast<double>(deltaY) / static_cast<double>(deltaX);

	qDebug() << "slope:" << slope;

	if (deltaY == 0) // ak je rozdiel y suradnic 0 -> horizontalna usecka
	{
		drawDDAHorizontalLine(point1, point2, color);
		update();
		return;
	}

	if (qAbs(slope) < 1) chosenAxis = 'x';
	else if (qAbs(slope) > 1) chosenAxis = 'y';
	else chosenAxis = 'x';

	if (chosenAxis == 'x')
		drawDDAChosenX(point1, point2, color);
	else if (chosenAxis == 'y')
		drawDDAChosenY(point1, point2, color);

	update();
}

void ViewerWidget::drawLineBresenham(QPoint point1, QPoint point2, QColor color)
{
	qDebug() << "Bresenham line:" << point1 << point2 << "with color:" << color.name();
	painter->drawLine(point1, point2);
	update();
}

void ViewerWidget::drawCircumference(QPoint point1, QPoint point2, QColor color)
{
	qDebug() << "Circumference line:" << point1 << point2 << "with color:" << color.name();

	update();
}

//Slots
void ViewerWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QRect area = event->rect();
	painter.drawImage(area, *img, area);
}
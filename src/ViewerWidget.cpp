#include   "ViewerWidget.h"

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
	qDebug() << "DDA line:" << point1 << point2 << "with color:" << color.name();
	int deltaX = qAbs(point1.x() - point2.x());
	int deltaY = qAbs(point1.y() - point2.y());

	int x = point1.x(); int y = point1.y();

	if (deltaX == 0)
	{
		while (y != point2.y())
		{
			setPixel(x, y, color);
			y++;
		}
	}
	else if (deltaY == 0)
	{
		while (x != point2.x())
		{
			setPixel(x, y, color);
			x++;
		}
	}

	update();
}

void ViewerWidget::drawLineBresenham(QPoint point1, QPoint point2, QColor color)
{
	qDebug() << "Bresenham line:" << point1 << point2 << "with color:" << color.name();

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
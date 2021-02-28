#include   "ViewerWidget.h"

void ViewerWidget::swapPoints(QPoint& point1, QPoint& point2)
{
	int temp = point1.x();
	point1.setX(point2.x());
	point2.setX(temp);
	
	temp = point1.y();
	point1.setY(point2.y());
	point2.setY(temp);
}

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
	if (point1.x() > point2.x())
		swapPoints(point1, point2);
	
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
		swapPoints(point1, point2);
	
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

void ViewerWidget::drawBresenhamChosenX(QPoint point1, QPoint point2, QColor color)
{
	if (point1.x() > point2.x()) // ak sa klikol prvy bod viac vpravo
		swapPoints(point1, point2);

	int deltaX = point2.x() - point1.x();
	int deltaY = point2.y() - point1.y();
	double slope = static_cast<double>(deltaY) / static_cast<double>(deltaX);
	int k1 = 0;
	int k2 = 0;
	int p = 0;
	int x = 0;
	int y = 0;

	if (slope >= 0 && slope < 1)
	{
		k1 = 2 * deltaY; k2 = 2 * (deltaY - deltaX);
		p = 2 * deltaY - deltaX;

		x = point1.x(); y = point1.y();

		setPixel(x, y, color);

		while (x < point2.x())
		{
			x++;
			if (p > 0)
			{
				y++;
				p += k2;
			}
			else
				p += k1;

			setPixel(x, y, color);
		}
	}
	else if (slope > -1 && slope <= 0)
	{
		k1 = 2 * deltaY; k2 = 2 * (deltaY + deltaX);
		p = 2 * deltaY + deltaX;

		x = point1.x(); y = point1.y();

		setPixel(x, y, color);

		while (x < point2.x())
		{
			x++;
			if (p < 0)
			{
				y--;
				p += k2;
			}
			else
				p += k1;

			setPixel(x, y, color);
		}
	}
}

void ViewerWidget::drawBresenhamChosenY(QPoint point1, QPoint point2, QColor color)
{
	if (point1.y() > point2.y()) // ak sa klikol prvy bod nizsie ako druhy bod
		swapPoints(point1, point2);

	int deltaX = point2.x() - point1.x();
	int deltaY = point2.y() - point1.y();

	double slope = static_cast<double>(deltaY) / static_cast<double>(deltaX);
	int k1 = 0;
	int k2 = 0;
	int p = 0;
	int x = 0;
	int y = 0;

	if (slope >= 1)
	{
		k1 = 2 * deltaX; k2 = 2 * (deltaX - deltaY);
		p = 2 * deltaX - deltaY;

		x = point1.x(); y = point1.y();

		setPixel(x, y, color);

		while (y < point2.y())
		{
			y++;

			if (p > 0)
			{
				x++;
				p += k2;
			}
			else
				p += k1;

			setPixel(x, y, color);
		}
	}
	else if (slope <= -1)
	{
		k1 = 2 * deltaX; k2 = 2 * (deltaX + deltaY);
		p = 2 * deltaX + deltaY;

		x = point1.x(); y = point1.y();

		setPixel(x, y, color);

		while (y < point2.y())
		{
			y++;

			if (p < 0)
			{
				x--;
				p += k2;
			}
			else
				p += k1;

			setPixel(x, y, color);
		}
	}
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

	int deltaX = point2.x() - point1.x();
	int deltaY = point2.y() - point1.y();
	double slope = 0.0;

	if (deltaX == 0) // ak je rozdiel x suradnic 0 -> vertikalna usecka
		drawDDAVerticalLine(point1, point2, color);
	else // ak ten rozdiel nie je 0 -> mozme delit
		slope = static_cast<double>(deltaY) / static_cast<double>(deltaX);

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
	int deltaX = point2.x() - point1.x();
	int deltaY = point2.y() - point1.y();
	double slope = static_cast<double>(deltaY) / static_cast<double>(deltaX);

	if ((slope > 0 && slope < 1) || (slope > -1 && slope < 0)) // riadiaca os x
		drawBresenhamChosenX(point1, point2, color);
	else if ((slope > 1) || (slope < -1)) // riadiaca os y
		drawBresenhamChosenY(point1, point2, color);
	else
		drawBresenhamChosenX(point1, point2, color);
	
	update();
}

void ViewerWidget::drawCircumference(QPoint point1, QPoint point2, QColor color)
{
	int deltaX = point2.x() - point1.x();
	int deltaY = point2.y() - point1.y();

	int radius = static_cast<int>(qSqrt(static_cast<double>(deltaX) * deltaX + static_cast<double>(deltaY) * deltaY));
	int p = 1 - radius;
	int x = 0;
	int y = radius;
	int dvaX = 3;
	int dvaY = 2 * radius + 2;
	int Sx = point1.x();
	int Sy = point1.y();
	int pixels = 0;

	while (x <= y)
	{
		// pixely hore
		if (isInside(Sx - y, Sy + x)) { setPixel(Sx - y, Sy + x, color); pixels++; }
		if (isInside(Sx - y, Sy - x)) { setPixel(Sx - y, Sy - x, color); pixels++; }
		
		// pixely dole
		if (isInside(Sx + y, Sy + x)) { setPixel(Sx + y, Sy + x, color); pixels++; }
		if (isInside(Sx + y, Sy - x)) { setPixel(Sx + y, Sy - x, color); pixels++; }

		// pixely vpravo
		if (isInside(Sx + x, Sy - y)) { setPixel(Sx + x, Sy - y, color); pixels++; }
		if (isInside(Sx + x, Sy + y)) { setPixel(Sx + x, Sy + y, color); pixels++; }

		// pixely na lavo
		if (isInside(Sx - x, Sy - y)) { setPixel(Sx - x, Sy - y, color); pixels++; }
		if (isInside(Sx - x, Sy + y)) { setPixel(Sx - x, Sy + y, color); pixels++; }

		if (p > 0)
		{
			p -= dvaY;
			y--;
			dvaY -= 2;
		}

		p += dvaX;
		dvaX += 2;
		x++;
	}

	qDebug() << "drawn pixels:" << pixels;
	update();
}

//Slots
void ViewerWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QRect area = event->rect();
	painter.drawImage(area, *img, area);
}
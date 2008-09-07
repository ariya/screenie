/* This file is part of the Screenie project.
   Screenie is a simple fancy screenshot composer.

   Copyright (C) 2008 Ariya Hidayat <ariya.hidayat@gmail.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <QtGui>

#include "ui_parameters.h"
#include "ui_help.h"

// Create a reflected version of the image, along with gradient translucency
static QPixmap reflected(const QPixmap& img, int offsetPercent)
{
    QLinearGradient gradient(QPoint(0, 0), QPoint(0, img.height()));
    gradient.setColorAt(qMin(1.0, offsetPercent/100.0), Qt::black);
    gradient.setColorAt(0, Qt::white);

    QImage mask = img.toImage();
    QPainter painter(&mask);
    painter.fillRect(mask.rect(), gradient);
    painter.end();

    QImage reflection = img.toImage().mirrored();
    reflection.setAlphaChannel(mask);

    return QPixmap::fromImage(reflection);
}

// Add a reflection to the image and return a new image
static QPixmap reflectionAdded(const QPixmap& img, int opacityPercent, int offsetPercent)
{
    QPixmap result(img.width(), img.height()*2);
    result.fill(Qt::transparent);

    QPainter painter(&result);
    painter.drawPixmap(0, 0, img);
    painter.setOpacity(qMin(1.0, opacityPercent/100.0));
    painter.drawPixmap(0, img.height(), reflected(img, offsetPercent));
    painter.end();

    return result;
}

// Create a stub image with big '?' in the middle
static QPixmap defaultImage()
{
    QPixmap m_defaultImage = QPixmap(400, 300);

    QLinearGradient gradient(QPoint(0, 0), QPoint(0, m_defaultImage.height()));
    gradient.setColorAt(0, QColor(192, 192, 192));
    gradient.setColorAt(1, QColor(128, 128, 128));
    QPainter p(&m_defaultImage);
    p.fillRect(m_defaultImage.rect(), gradient);
    p.setPen(QPen(QColor(96, 96, 96), 3, Qt::SolidLine));
    p.drawRect(m_defaultImage.rect());
    QFont font;
    font.setPixelSize(100);
    p.setFont(font);
    p.setPen(Qt::white);
    p.drawText(m_defaultImage.rect(), Qt::AlignCenter, "?");
    p.end();

    return m_defaultImage;
}

class Screenie: public QWidget
{
public:
    Screenie();
    ~Screenie();
    void loadImage(int index, const QPixmap& image);
    void dragEnterEvent(QDragEnterEvent*);
    void dropEvent(QDropEvent* event);
    void paintEvent(QPaintEvent*);
    void resizeEvent(QResizeEvent*);
    void mouseReleaseEvent(QMouseEvent*);

    void setupUI();
    void recreateReflection();
    void render(QPainter*);

private:
    QPixmap m_centerImage;
    QPixmap m_leftImage;
    QPixmap m_rightImage;

    void paintLeftImage(QPainter*, const int, const int);
    void paintCenterImage(QPainter*, const int, const int);
    void paintRightImage(QPainter*, const int, const int);

    Ui::ParametersForm* parameters;

    bool m_useReflection;
    int m_reflectionOpacity;
    int m_reflectionOffset;
    bool m_useBackground;
    QColor m_backgroundColor;
    QBrush m_checkerBoard;
};

Screenie::Screenie(): m_useReflection(true), m_reflectionOpacity(100), m_reflectionOffset(40),
m_useBackground(true)
{
    m_centerImage = reflectionAdded(defaultImage(), m_reflectionOpacity, m_reflectionOffset);
    m_leftImage = m_rightImage = m_centerImage;

    setAcceptDrops(true);

    setAttribute(Qt::WA_StaticContents, true);

    setupUI();
    resize(900, 500);
    update();
}

Screenie::~Screenie()
{
    delete parameters;
}

void Screenie::setupUI()
{
    parameters = new Ui::ParametersForm;
    QWidget* toolWidget = new QWidget(this);
    toolWidget->setWindowFlags(Qt::Tool | Qt::WindowTitleHint);
    parameters->setupUi(toolWidget);
    toolWidget->show();
    toolWidget->adjustSize();

    QWidget* helpWidget = new QWidget(this);
    helpWidget->setWindowFlags(Qt::Tool | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
    Ui::HelpForm helpForm;
    helpForm.setupUi(helpWidget);
    helpWidget->show();
    helpWidget->adjustSize();

    connect(parameters->leftImageBox, SIGNAL(toggled(bool)), SLOT(update()));
    connect(parameters->centerImageBox, SIGNAL(toggled(bool)), SLOT(update()));
    connect(parameters->rightImageBox, SIGNAL(toggled(bool)), SLOT(update()));
    connect(parameters->reflectionBox, SIGNAL(toggled(bool)), SLOT(update()));
    connect(parameters->backgroundBox, SIGNAL(toggled(bool)), SLOT(update()));

    connect(parameters->leftOffsetSlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(parameters->leftDistanceSlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(parameters->leftAngleSlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(parameters->centerOffsetSlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(parameters->centerDistanceSlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(parameters->centerAngleSlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(parameters->rightOffsetSlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(parameters->rightDistanceSlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(parameters->rightAngleSlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(parameters->reflectionOpacitySlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(parameters->reflectionOffsetSlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(parameters->backgroundRedSlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(parameters->backgroundGreenSlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(parameters->backgroundBlueSlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(parameters->redSpinBox, SIGNAL(valueChanged(int)), SLOT(update()));
    connect(parameters->greenSpinBox, SIGNAL(valueChanged(int)), SLOT(update()));
    connect(parameters->blueSpinBox, SIGNAL(valueChanged(int)), SLOT(update()));
}

QPixmap upperHalf(const QPixmap& img)
{
    return img.copy(0, 0, img.width(), img.height()/2);
}

void Screenie::recreateReflection()
{
    if (m_useReflection) {
        m_leftImage = reflectionAdded(upperHalf(m_leftImage), m_reflectionOpacity, m_reflectionOffset);
        m_centerImage = reflectionAdded(upperHalf(m_centerImage), m_reflectionOpacity, m_reflectionOffset);
        m_rightImage = reflectionAdded(upperHalf(m_rightImage), m_reflectionOpacity, m_reflectionOffset);
    } else {
        m_leftImage = reflectionAdded(upperHalf(m_leftImage), 1, 0);
        m_centerImage = reflectionAdded(upperHalf(m_centerImage), 1, 0);
        m_rightImage = reflectionAdded(upperHalf(m_rightImage), 1, 0);
    }
}

void Screenie::loadImage(int index, const QPixmap& image)
{
    QPixmap img = image;
    if (img.isNull()) {
        qDebug() << "Can not load null image!";
        return;
    }

    if ((img.width() > 640) || (img.height() > 480)) {
        qDebug() << "Image is too large. Rescaling....";
        img = img.scaled(640, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    img = reflectionAdded(img, m_reflectionOpacity, m_reflectionOffset);
    setWindowTitle("Screenshot Composer");

    switch(index) {
        case 0:
             m_leftImage = img;
             break;
        case 1:
             m_centerImage = img;
             break;
        case 2:
             m_rightImage = img;
             break;
        default:
             m_centerImage = img;
             break;
    }

    update();
}

void Screenie::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void Screenie::dropEvent(QDropEvent* event)
{
    int index = 1;
    int cx = width()/2;
    int cw = m_centerImage.width();
    if (event->pos().x() < cx - cw/2)
        index = 0;
    if (event->pos().x() > cx + cw/2)
        index = 2;

    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.count()) {
        QPixmap img;
        QString fname = urls[0].toLocalFile();

        if (event->mimeData()->hasImage())
            img = qvariant_cast<QPixmap>(event->mimeData()->imageData());
        else
            img = QPixmap(fname);

        if (img.isNull())
            setWindowTitle(QString("Can not load %1").arg(fname));
        else
            loadImage(index, img);

        event->acceptProposedAction();
    }
}

void Screenie::paintEvent(QPaintEvent*)
{
    QBrush bgbrush;

    m_useBackground = parameters->backgroundBox->isChecked();
    if (m_useBackground) {
        int red = parameters->backgroundRedSlider->value();
        int green = parameters->backgroundGreenSlider->value();
        int blue = parameters->backgroundBlueSlider->value();
        m_backgroundColor = QColor(red, green, blue);
        bgbrush = QBrush(m_backgroundColor);
    } else {
        if (m_checkerBoard.style() == Qt::NoBrush) {
            QImage checker(16, 16, QImage::Format_ARGB32_Premultiplied);
            QPainter painter(&checker);
            painter.fillRect(checker.rect(), QColor(153, 153, 153));
            painter.fillRect(0, 0, 8, 8, QColor(102, 102, 102));
            painter.fillRect(8, 8, 8, 8, QColor(102, 102, 102));
            painter.end();
            m_checkerBoard.setTextureImage(checker);
        }
        bgbrush = m_checkerBoard;
    }

    QPalette pal = palette();
    pal.setBrush(backgroundRole(), bgbrush);
    setPalette(pal);

    if ((m_reflectionOpacity != parameters->reflectionOpacitySlider->value()) ||
        (m_reflectionOffset != parameters->reflectionOffsetSlider->value()) ||
        (m_useReflection != parameters->reflectionBox->isChecked())) {
        m_reflectionOpacity = parameters->reflectionOpacitySlider->value();
        m_reflectionOffset = parameters->reflectionOffsetSlider->value();
        m_useReflection = parameters->reflectionBox->isChecked();
        recreateReflection();
    }

    QPainter painter(this);
    render(&painter);

    parameters->leftOffsetSlider->setMaximum(width()/2);
    parameters->rightOffsetSlider->setMaximum(width()/2);
}

void Screenie::render(QPainter* painter)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    int cx = width()/2;
    int cy = height()*2/3;

    int leftDistance = parameters->leftDistanceSlider->value();
    int centerDistance = parameters->centerDistanceSlider->value();
    int rightDistance = parameters->rightDistanceSlider->value();

    if (leftDistance > centerDistance && leftDistance > rightDistance) {
        paintLeftImage(painter, cx, cy);
        if (centerDistance > rightDistance) {
            paintCenterImage(painter, cx, cy);
            paintRightImage(painter, cx, cy);
        } else {
            paintRightImage(painter, cx, cy);
            paintCenterImage(painter, cx, cy);
        }
    } else if(centerDistance > leftDistance && centerDistance > rightDistance) {
        paintCenterImage(painter, cx, cy);
        if (leftDistance > rightDistance) {
            paintLeftImage(painter, cx, cy);
            paintRightImage(painter, cx, cy);
        } else {
            paintRightImage(painter, cx, cy);
            paintLeftImage(painter, cx, cy);
        }
    } else {
        paintRightImage(painter, cx, cy);
        if (leftDistance > centerDistance) {
            paintLeftImage(painter, cx, cy);
            paintCenterImage(painter, cx, cy);
        } else {
            paintCenterImage(painter, cx, cy);
            paintLeftImage(painter, cx, cy);
        }
    }
}

void Screenie::paintLeftImage(QPainter *painter, const int cx, const int cy)
{
    if (parameters->leftImageBox->isChecked()) {
        QTransform transform;
        painter->save();
        qreal leftScale = 1 - (qreal)parameters->leftDistanceSlider->value()/200.0;
        transform = QTransform().scale(leftScale, leftScale);
        transform.translate(0, -m_leftImage.height()/4);
        transform.rotate(parameters->leftAngleSlider->value(), Qt::YAxis);
        transform.translate(0, m_leftImage.height()/4);
        int dx = parameters->leftOffsetSlider->value();
        int dy = parameters->leftDistanceSlider->value();
        painter->setTransform(transform * QTransform().translate(cx-dx,cy-dy), true);
        painter->drawPixmap(-m_leftImage.width()/2, -m_leftImage.height()/2, m_leftImage);
        painter->restore();
    }
}

void Screenie::paintCenterImage(QPainter *painter, const int cx, const int cy)
{
    if (parameters->centerImageBox->isChecked()) {
        QTransform transform;
        painter->save();
        qreal centerScale = 1 - (qreal)parameters->centerDistanceSlider->value()/200.0;
        transform = QTransform().scale(centerScale, centerScale);
        transform.translate(0, -m_centerImage.height()/4);
        transform.rotate(parameters->centerAngleSlider->value(), Qt::YAxis);
        transform.translate(0, m_centerImage.height()/4);
        int dx = parameters->centerOffsetSlider->value();
        int dy = parameters->centerDistanceSlider->value();
        painter->setTransform(transform * QTransform().translate(cx+dx,cy-dy), true);
        painter->drawPixmap(-m_centerImage.width()/2, -m_centerImage.height()/2, m_centerImage);
        painter->restore();
    }
}

void Screenie::paintRightImage(QPainter *painter, const int cx, const int cy)
{
    if (parameters->rightImageBox->isChecked()) {
        QTransform transform;
        painter->save();
        qreal rightScale = 1 - (qreal)parameters->rightDistanceSlider->value()/200.0;
        transform = QTransform().scale(rightScale, rightScale);
        transform.translate(0, -m_rightImage.height()/4);
        transform.rotate(parameters->rightAngleSlider->value(), Qt::YAxis);
        transform.translate(0, m_rightImage.height()/4);
        int dx = parameters->rightOffsetSlider->value();
        int dy = parameters->rightDistanceSlider->value();
        painter->setTransform(transform * QTransform().translate(cx+dx,cy-dy), true);
        painter->drawPixmap(-m_rightImage.width()/2, -m_rightImage.height()/2, m_rightImage);
        painter->restore();
    }
}

void Screenie::resizeEvent(QResizeEvent*)
{
    QTimer::singleShot(500, this, SLOT(update()));
}

void Screenie::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() != Qt::RightButton)
        return;

    QString filter = "Portable Network Graphics (*.png)";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save"), QString(), filter);
    if (fileName.isNull())
        return;

    QImage image(width(), height(), QImage::Format_ARGB32_Premultiplied);
    image.fill(0);
    QPainter painter(&image);
    if (m_useBackground)
        painter.fillRect(image.rect(), m_backgroundColor);
    render(&painter);
    painter.end();
    image.save(fileName, "png");
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Screenie widget;
    widget.show();

    switch(argc) {
        case 2:
            widget.loadImage(1, QPixmap(argv[1]));
            break;
        case 3:
            widget.loadImage(0, QPixmap(argv[1]));
            widget.loadImage(1, QPixmap(argv[2]));
            break;
        case 4:
            widget.loadImage(0, QPixmap(argv[1]));
            widget.loadImage(1, QPixmap(argv[2]));
            widget.loadImage(2, QPixmap(argv[3]));
            break;
    }

    return app.exec();
}

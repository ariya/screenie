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

    QGroupBox* m_leftImageBox;
    QGroupBox* m_centerImageBox;
    QGroupBox* m_rightImageBox;
    QGroupBox* m_reflectionBox;
    QGroupBox* m_backgroundBox;

    QSlider* m_leftOffsetSlider;
    QSlider* m_leftDistanceSlider;
    QSlider* m_leftAngleSlider;
    QSlider* m_centerAngleSlider;
    QSlider* m_rightOffsetSlider;
    QSlider* m_rightDistanceSlider;
    QSlider* m_rightAngleSlider;

    QSlider* m_reflectionOpacitySlider;
    QSlider* m_reflectionOffsetSlider;
    bool m_useReflection;
    int m_reflectionOpacity;
    int m_reflectionOffset;

    QSlider* m_backgroundRedSlider;
    QSlider* m_backgroundGreenSlider;
    QSlider* m_backgroundBlueSlider;
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
    update();
}

void Screenie::setupUI()
{
    QWidget* toolWidget = new QWidget(this);
    toolWidget->setWindowFlags(Qt::Tool | Qt::WindowTitleHint);
    Ui::ParametersForm parameters;
    parameters.setupUi(toolWidget);
    toolWidget->show();
    toolWidget->adjustSize();

    QWidget* helpWidget = new QWidget(this);
    helpWidget->setWindowFlags(Qt::Tool | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
    Ui::HelpForm helpForm;
    helpForm.setupUi(helpWidget);
    helpWidget->show();
    helpWidget->adjustSize();

    m_leftImageBox = qFindChild<QGroupBox*>(toolWidget, "leftImageBox");
    m_centerImageBox = qFindChild<QGroupBox*>(toolWidget, "centerImageBox");
    m_rightImageBox = qFindChild<QGroupBox*>(toolWidget, "rightImageBox");
    m_reflectionBox = qFindChild<QGroupBox*>(toolWidget, "reflectionBox");
    m_backgroundBox = qFindChild<QGroupBox*>(toolWidget, "backgroundBox");

    m_leftOffsetSlider = qFindChild<QSlider*>(toolWidget, "leftOffsetSlider");
    m_leftDistanceSlider = qFindChild<QSlider*>(toolWidget, "leftDistanceSlider");
    m_leftAngleSlider = qFindChild<QSlider*>(toolWidget, "leftAngleSlider");
    m_centerAngleSlider = qFindChild<QSlider*>(toolWidget, "centerAngleSlider");
    m_rightOffsetSlider = qFindChild<QSlider*>(toolWidget, "rightOffsetSlider");
    m_rightDistanceSlider = qFindChild<QSlider*>(toolWidget, "rightDistanceSlider");
    m_rightAngleSlider = qFindChild<QSlider*>(toolWidget, "rightAngleSlider");
    m_reflectionOpacitySlider = qFindChild<QSlider*>(toolWidget, "reflectionOpacitySlider");
    m_reflectionOffsetSlider = qFindChild<QSlider*>(toolWidget, "reflectionOffsetSlider");
    m_backgroundRedSlider = qFindChild<QSlider*>(toolWidget, "backgroundRedSlider");
    m_backgroundGreenSlider = qFindChild<QSlider*>(toolWidget, "backgroundGreenSlider");
    m_backgroundBlueSlider = qFindChild<QSlider*>(toolWidget, "backgroundBlueSlider");

    connect(m_leftImageBox, SIGNAL(toggled(bool)), SLOT(update()));
    connect(m_centerImageBox, SIGNAL(toggled(bool)), SLOT(update()));
    connect(m_rightImageBox, SIGNAL(toggled(bool)), SLOT(update()));
    connect(m_reflectionBox, SIGNAL(toggled(bool)), SLOT(update()));
    connect(m_backgroundBox, SIGNAL(toggled(bool)), SLOT(update()));

    connect(m_leftOffsetSlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(m_leftDistanceSlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(m_leftAngleSlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(m_centerAngleSlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(m_rightOffsetSlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(m_rightDistanceSlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(m_rightAngleSlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(m_reflectionOpacitySlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(m_reflectionOffsetSlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(m_backgroundRedSlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(m_backgroundGreenSlider, SIGNAL(sliderMoved(int)), SLOT(update()));
    connect(m_backgroundBlueSlider, SIGNAL(sliderMoved(int)), SLOT(update()));
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

    m_useBackground = m_backgroundBox->isChecked();
    if (m_useBackground) {
        int red = m_backgroundRedSlider->value();
        int green = m_backgroundGreenSlider->value();
        int blue = m_backgroundBlueSlider->value();
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

    QPainter painter(this);
    render(&painter);
}

void Screenie::render(QPainter* painter)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    int cx = width()/2;
    int cy = height()*2/3;

    if ((m_reflectionOpacity != m_reflectionOpacitySlider->value()) ||
        (m_reflectionOffset != m_reflectionOffsetSlider->value()) || 
        (m_useReflection != m_reflectionBox->isChecked())) {
        m_reflectionOpacity = m_reflectionOpacitySlider->value();
        m_reflectionOffset = m_reflectionOffsetSlider->value();
        m_useReflection = m_reflectionBox->isChecked();
        recreateReflection();
    }

    QColor color(m_backgroundRedSlider->value(), m_backgroundGreenSlider->value(),
        m_backgroundBlueSlider->value());
    if (m_backgroundColor != color) {
        m_backgroundColor = color;
        QPalette pal = palette();
        pal.setColor(backgroundRole(), color);
        setPalette(pal);
    }

    m_leftOffsetSlider->setMaximum(cx);
    m_rightOffsetSlider->setMaximum(cx);

    QTransform transform;

    if (m_leftImageBox->isChecked()) {
        painter->save();
        qreal leftScale = 1 - (qreal)m_leftDistanceSlider->value()/200.0;
        transform = QTransform().scale(leftScale, leftScale);
        transform.translate(0, -m_leftImage.height()/4);
        transform.rotate(m_leftAngleSlider->value(), Qt::YAxis);
        transform.translate(0, m_leftImage.height()/4);
        int dx = m_leftOffsetSlider->value();
        int dy = m_leftDistanceSlider->value();
        painter->setTransform(transform * QTransform().translate(cx-dx,cy-dy), true);
        painter->drawPixmap(-m_leftImage.width()/2, -m_leftImage.height()/2, m_leftImage);
        painter->restore();
    }

    if (m_rightImageBox->isChecked()) {
        painter->save();
        qreal rightScale = 1 - (qreal)m_rightDistanceSlider->value()/200.0;
        transform = QTransform().scale(rightScale, rightScale);
        transform.translate(0, -m_rightImage.height()/4);
        transform.rotate(m_rightAngleSlider->value(), Qt::YAxis);
        transform.translate(0, m_rightImage.height()/4);
        int dx = m_rightOffsetSlider->value();
        int dy = m_rightDistanceSlider->value();
        painter->setTransform(transform * QTransform().translate(cx+dx,cy-dy), true);
        painter->drawPixmap(-m_rightImage.width()/2, -m_rightImage.height()/2, m_rightImage);
        painter->restore();
    }

    if (m_centerImageBox->isChecked()) {
        QPoint corner(-m_centerImage.width()/2, -m_centerImage.height()/2);
        painter->save();
        transform = QTransform().translate(0, -m_centerImage.height()/4);
        transform.rotate(m_centerAngleSlider->value(), Qt::YAxis);
        transform.translate(0, m_centerImage.height()/4);
        painter->setTransform(transform * QTransform().translate(cx,cy), true);
        painter->drawPixmap(corner, m_centerImage);
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

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save"), QString(), "*.png");
    if (fileName.isNull())
        return;

    QImage image(width(), height(), QImage::Format_ARGB32_Premultiplied);
    image.fill(0);
    QPainter painter(&image);
    if (m_useBackground)
        painter.fillRect(image.rect(), m_backgroundColor);
    render(&painter);
    painter.end();
    image.save(fileName);
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

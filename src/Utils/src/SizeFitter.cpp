#include <limits>

#include <QtCore/QSize>
#include <QtCore/QRect>
#include <QtCore/QBitArray>

#include "SizeFitter.h"

class SizeFitterPrivate
{
public:
    SizeFitterPrivate(QSize theTargetSize, SizeFitter::FitMode theFitMode)
        : targetSize(theTargetSize),
          fitMode(theFitMode)
    {}

    SizeFitterPrivate(const SizeFitterPrivate &other)
    {
        copy(other);
    }

    SizeFitterPrivate operator=(const SizeFitterPrivate &other)
    {
        copy(other);
        return *this;
    }

    QSize targetSize;
    SizeFitter::FitMode fitMode;
    QBitArray fitOptions;

private:
    void copy(const SizeFitterPrivate &other)
    {
        fitMode = other.fitMode;
        fitOptions = other.fitOptions;
        targetSize = other.targetSize;
    }
};

const int SizeFitter::InvalidSize = std::numeric_limits<int>::min();

// public

SizeFitter::SizeFitter(QSize targetSize, FitMode fitMode)
    : d(new SizeFitterPrivate(targetSize, fitMode))
{
    setDefaultFitOptions();
}

SizeFitter::SizeFitter()
    : d(new SizeFitterPrivate(QSize(640, 480), ExactFit))
{
    setDefaultFitOptions();
}

SizeFitter::SizeFitter(const SizeFitter &other)
    : QObject(),
      d(new SizeFitterPrivate(*other.d))
{
}

SizeFitter::~SizeFitter()
{
    delete d;
}

void SizeFitter::setTargetSize(QSize newTargetSize)
{
    if (d->targetSize != newTargetSize) {
        d->targetSize = newTargetSize;
        emit changed();
    }
}

QSize SizeFitter::getTargetSize() const
{
    return d->targetSize;
}

void SizeFitter::setTargetWidth(int newTargetWidth)
{
    if (d->targetSize.width() != newTargetWidth) {
        d->targetSize.setWidth(newTargetWidth);
        emit changed();
    }
}

void SizeFitter::setTargetHeight(int newTargetHeight)
{
    if (d->targetSize.height() != newTargetHeight) {
        d->targetSize.setHeight(newTargetHeight);
        emit changed();
    }
}

void SizeFitter::setFitMode(FitMode newFitMode)
{
    if (d->fitMode != newFitMode) {
        d->fitMode = newFitMode;
        emit changed();
    }
}

SizeFitter::FitMode SizeFitter::getFitMode() const
{
    return d->fitMode;
}

void SizeFitter::setFitOptionEnabled(FitOption fitOption, bool enable)
{
    if (d->fitOptions.testBit(fitOption) != enable) {
        d->fitOptions.setBit(fitOption, enable);
        emit changed();
    }
}

bool SizeFitter::isFitOptionEnabled(FitOption fitOption) const
{
    return d->fitOptions.testBit(fitOption);
}

QBitArray SizeFitter::getFitOptions() const
{
    return d->fitOptions;
}

void SizeFitter::setFitOptions(const QBitArray &fitOptions)
{
    if (d->fitOptions != fitOptions) {
        d->fitOptions = fitOptions;
        emit changed();
    }
}

bool SizeFitter::fit(QSize size, QSize &fittedSize, QRect *clippedRect) const
{
    bool result;

    // fit at all?
    if (d->fitMode != NoFit) {

        switch (d->fitMode) {
        case Fit:
            result = this->fitIt(size, fittedSize, clippedRect);
            break;

        case FitToWidth:
            result = this->fitToWidth(size, fittedSize, clippedRect);
            break;

        case FitToHeight:
            result = this->fitToHeight(size, fittedSize, clippedRect);
            break;

        case ExactFit:
            result = this->exactFit(size, fittedSize, clippedRect);
            break;

        default:
            result = false;
#ifdef DEBUG
            qWarning("SizeFitter::fit: Fit mode '%d' not implemented!", d->fitMode);
#endif
            break;
        }
    } else {
        result = false;
    }

    return result;
}

SizeFitter SizeFitter::operator=(const SizeFitter &other)
{
    *d = *other.d;
    return *this;
}

// private

void SizeFitter::setDefaultFitOptions()
{
    d->fitOptions.resize(NofFitOptions),
    d->fitOptions.setBit(RespectOrientation, true);
    d->fitOptions.setBit(Enlarge, false);
}

QSize SizeFitter::getOrientedTargetSize(const QSize &size) const
{
    QSize orientedTargetSize;

    if (this->isFitOptionEnabled(RespectOrientation) == true &&
        size.width() < size.height() &&
        d->targetSize.width() > d->targetSize.height()) {
        // make 'size' and 'target' have the same orientation
        orientedTargetSize.setWidth(d->targetSize.height());
        orientedTargetSize.setHeight(d->targetSize.width());
    } else {
        orientedTargetSize = d->targetSize;
    }

    return orientedTargetSize;
}

bool SizeFitter::fitIt(QSize size, QSize &fittedSize, QRect *clippedArea) const
{
    float scaleFactor;
    bool  result;
    QSize orientedTargetSize = this->getOrientedTargetSize(size);

    orientedTargetSize = this->getOrientedTargetSize(size);

    // enlarge?
    if (this->isFitOptionEnabled(Enlarge) == false &&
        size.width() <= orientedTargetSize.width() &&
        size.height() <= orientedTargetSize.height()) {
        fittedSize = size;
        if (clippedArea != 0) {
            clippedArea->setTopLeft(QPoint(0, 0));
            clippedArea->setSize(fittedSize);
        }
        return false;
    }

    result = false;
    if (size.width() > size.height()) {
        // landscape        
        scaleFactor = (float)orientedTargetSize.width() / (float)size.width();
        fittedSize.setWidth(qRound(size.width() * scaleFactor));
        fittedSize.setHeight(qRound(size.height() * scaleFactor));

        if (scaleFactor != 1.0f) {
            result = true;
        }

        // the new height might still be too large...
        if (fittedSize.height() > orientedTargetSize.height()) {
            // ...so scale again
            scaleFactor = (float)orientedTargetSize.height() / (float)fittedSize.height();
            fittedSize.setWidth (qRound(fittedSize.width() * scaleFactor));
            fittedSize.setHeight(qRound(fittedSize.height() * scaleFactor));
            
            if (scaleFactor != 1.0f) {
                result = true ;
            }
        }
    } else {
        // portrait        
        scaleFactor = (float)orientedTargetSize.height() / (float)size.height();
        fittedSize.setWidth (qRound(size.width() * scaleFactor));
        fittedSize.setHeight (qRound(size.height() * scaleFactor));

        if (scaleFactor != 1.0f) {
            result = true;
        }

        // the new width might still be too large...
        if (fittedSize.width() > orientedTargetSize.width()) {

            // ...so scale again
            scaleFactor = (float)orientedTargetSize.width() / (float)fittedSize.width();
            fittedSize.setWidth (qRound(fittedSize.width() * scaleFactor));
            fittedSize.setHeight (qRound(fittedSize.height() * scaleFactor));

            if (scaleFactor != 1.0f) {
                result = true ;
            }            
        }
    }

    if (clippedArea != 0) {
        // no clipping, select entire image
        clippedArea->setTopLeft(QPoint(0, 0));
        clippedArea->setSize(size);
    }

    return result;

}

bool SizeFitter::fitToWidth(QSize size, QSize &fittedSize, QRect *clippedArea) const
{
    float aspectRatio;
    bool  result;

    // enlarge?
    if (this->isFitOptionEnabled(Enlarge) == false &&
        size.width() <= d->targetSize.width()) {
        fittedSize = size;
        if (clippedArea != 0) {
            clippedArea->setTopLeft(QPoint(0, 0));
            clippedArea->setSize(fittedSize);
        }
        return false;
    }

    if (size.width() != d->targetSize.width()) {
        result = true;
        aspectRatio = (float)size.width() / (float)size.height();
        fittedSize.setWidth(d->targetSize.width());
        fittedSize.setHeight(qRound(d->targetSize.width() / aspectRatio));
    } else {
        result = false;
    }

    if (clippedArea != 0) {
        // no clipping, select entire image
        clippedArea->setTopLeft(QPoint(0, 0));
        clippedArea->setSize(size);
    }

    return result;
}

bool SizeFitter::fitToHeight(QSize size, QSize &fittedSize, QRect *clippedArea) const
{
    float aspectRatio;
    bool  result;

    // enlarge?
    if (this->isFitOptionEnabled(Enlarge) == false &&
        size.height() <= d->targetSize.height()) {
        fittedSize = size;
        if (clippedArea != 0) {
            clippedArea->setTopLeft(QPoint(0, 0));
            clippedArea->setSize(fittedSize);
        }
        return false;
    }

    if (size.height() != d->targetSize.height()) {
        result = true;
        aspectRatio = (float)size.height() / (float)size.width();
        fittedSize.setWidth(qRound(d->targetSize.height() / aspectRatio));
        fittedSize.setHeight(d->targetSize.height());
    } else {
        result = false;
    }

    if (clippedArea != 0) {
        // no clipping, select entire image
        clippedArea->setTopLeft(QPoint(0, 0));
        clippedArea->setSize(size);
    }

    return result;
}

bool SizeFitter::exactFit(QSize size, QSize &fittedSize, QRect *clippedArea) const
{
    bool  result;
    float aspectRatio, targetAspectRatio;
    int   newWidth, newHeight;
    QSize newSize;
    QSize orientedTargetSize = this->getOrientedTargetSize(size);
    
    fittedSize = orientedTargetSize;
    
    // enlarge?
    if (this->isFitOptionEnabled(Enlarge) == false &&
        size.width() <= orientedTargetSize.width() &&
        size.height() <= orientedTargetSize.height()) {
        fittedSize = size;
        if (clippedArea != 0) {
            clippedArea->setTopLeft(QPoint(0, 0));
            clippedArea->setSize(fittedSize);
        }
        return false;
    }
    
    if (size != orientedTargetSize) {
        result = true;
    } else {
        result = false;
    }

    if (clippedArea != 0) {
        // center the visible area
        /*!\todo Implement CENTER, N, NE, E, SE etc. gravity */
        aspectRatio       = (float)size.width() / (float)size.height();
        targetAspectRatio = (float)orientedTargetSize.width() / (float)orientedTargetSize.height();
        if (aspectRatio < targetAspectRatio) {
            // height too large
            newHeight = qRound(size.width() / targetAspectRatio);
            clippedArea->setTopLeft(QPoint(0, qRound((size.height() - newHeight) / 2.0f)));
            newSize.setWidth(size.width());
            newSize.setHeight(newHeight);
            clippedArea->setSize(newSize);
        } else {
            // width too large
            newWidth = qRound(size.height() * targetAspectRatio);
            clippedArea->setTopLeft(QPoint(qRound((size.width() - newWidth) / 2.0f), 0));
            newSize.setWidth(newWidth);
            newSize.setHeight(size.height());
            clippedArea->setSize(newSize);
        }
    }
    return result;
}


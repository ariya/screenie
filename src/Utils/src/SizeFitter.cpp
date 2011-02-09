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
          maxTargetSize(theTargetSize),
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
    QSize maxTargetSize;
    SizeFitter::FitMode fitMode;
    QBitArray fitOptions;

private:
    void copy(const SizeFitterPrivate &other)
    {
        targetSize = other.targetSize;
        maxTargetSize = other.maxTargetSize;
        fitMode = other.fitMode;
        fitOptions = other.fitOptions;
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

QSize SizeFitter::getTargetSize() const
{
    return d->targetSize;
}

void SizeFitter::setTargetSize(QSize targetSize)
{
    if (d->targetSize != targetSize) {
        d->targetSize = targetSize;
        // adjust maxTargetSize
        if (d->maxTargetSize.width() < d->targetSize.width()) {
            d->maxTargetSize.setWidth(d->targetSize.width());
        }
        if (d->maxTargetSize.height() < d->targetSize.height()) {
            d->maxTargetSize.setHeight(d->targetSize.height());
        }
        emit changed();
    }
}

void SizeFitter::setTargetWidth(int targetWidth)
{
    if (d->targetSize.width() != targetWidth) {
        d->targetSize.setWidth(targetWidth);
        // adjust maxTargetSize
        if (d->maxTargetSize.width() < d->targetSize.width()) {
            d->maxTargetSize.setWidth(d->targetSize.width());
        }
        emit changed();
    }
}

void SizeFitter::setTargetHeight(int targetHeight)
{
    if (d->targetSize.height() != targetHeight) {
        d->targetSize.setHeight(targetHeight);
        // adjust maxTargetSize
        if (d->maxTargetSize.height() < d->targetSize.height()) {
            d->maxTargetSize.setHeight(d->targetSize.height());
        }
        emit changed();
    }
}

QSize SizeFitter::getMaxTargetSize() const
{
    return d->maxTargetSize;
}

void SizeFitter::setMaxTargetSize(QSize maxTargetSize)
{
    if (d->maxTargetSize != maxTargetSize) {
        d->maxTargetSize = maxTargetSize;
        // adjust targetSize
        if (d->targetSize.width() > d->maxTargetSize.width()) {
            d->targetSize.setWidth(d->maxTargetSize.width());
        }
        if (d->targetSize.height() > d->maxTargetSize.height()) {
            d->targetSize.setHeight(d->maxTargetSize.height());
        }
        emit changed();
    }
}

SizeFitter::FitMode SizeFitter::getFitMode() const
{
    return d->fitMode;
}

void SizeFitter::setFitMode(FitMode fitMode)
{
    if (d->fitMode != fitMode) {
        d->fitMode = fitMode;
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

void SizeFitter::setFitOptionEnabled(FitOption fitOption, bool enable)
{
    if (d->fitOptions.testBit(fitOption) != enable) {
        d->fitOptions.setBit(fitOption, enable);
        emit changed();
    }
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
            result = fitIt(size, fittedSize, clippedRect);
            break;

        case FitToWidth:
            result = fitToWidth(size, fittedSize, clippedRect);
            break;

        case FitToHeight:
            result = fitToHeight(size, fittedSize, clippedRect);
            break;

        case ExactFit:
            result = exactFit(size, fittedSize, clippedRect);
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
    QSize result;

    if (isFitOptionEnabled(RespectOrientation) &&
        size.width() < size.height() &&
        d->targetSize.width() > d->targetSize.height()) {
        // make 'size' and 'target' have the same orientation
        result.setWidth(d->targetSize.height());
        result.setHeight(d->targetSize.width());
    } else {
        result = d->targetSize;
    }

    return result;
}

bool SizeFitter::fitIt(QSize size, QSize &fittedSize, QRect *clippedRect) const
{
    float scaleFactor;
    bool  result;
    QSize orientedTargetSize = getOrientedTargetSize(size);

    // enlarge?
    if (!isFitOptionEnabled(Enlarge) &&
        size.width() <= orientedTargetSize.width() &&
        size.height() <= orientedTargetSize.height()) {
        fittedSize = size;
        if (clippedRect != 0) {
            clippedRect->setTopLeft(QPoint(0, 0));
            clippedRect->setSize(fittedSize);
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

    if (clippedRect != 0) {
        // no clipping, select entire image
        clippedRect->setTopLeft(QPoint(0, 0));
        clippedRect->setSize(size);
    }

    return result;

}

bool SizeFitter::fitToWidth(QSize size, QSize &fittedSize, QRect *clippedRect) const
{
    float aspectRatio;
    bool result;

    // enlarge?
    if (!isFitOptionEnabled(Enlarge) &&
        size.width() <= d->targetSize.width()) {
        fittedSize = size;
        if (clippedRect != 0) {
            clippedRect->setTopLeft(QPoint(0, 0));
            clippedRect->setSize(fittedSize);
        }
        return false;
    }

    if (size.width() != d->targetSize.width()) {
        aspectRatio = (float)size.width() / (float)size.height();
        fittedSize.setWidth(d->targetSize.width());
        fittedSize.setHeight(qRound(d->targetSize.width() / aspectRatio));
        result = true;
    } else {
        result = false;
    }

    if (clippedRect != 0) {
        if (isFitOptionEnabled(RespectMaxTargetSize) && fittedSize.height() > d->maxTargetSize.height()) {
            fittedSize.setHeight(d->maxTargetSize.height());
            clip(size, fittedSize, clippedRect);
        } else {
            // no clipping
            clippedRect->setTopLeft(QPoint(0, 0));
            clippedRect->setSize(size);
        }
    }

    return result;
}

bool SizeFitter::fitToHeight(QSize size, QSize &fittedSize, QRect *clippedRect) const
{
    float aspectRatio;
    bool result;

    // enlarge?
    if (isFitOptionEnabled(Enlarge) == false &&
        size.height() <= d->targetSize.height()) {
        fittedSize = size;
        if (clippedRect != 0) {
            clippedRect->setTopLeft(QPoint(0, 0));
            clippedRect->setSize(fittedSize);
        }
        return false;
    }

    if (size.height() != d->targetSize.height()) {
        aspectRatio = (float)size.height() / (float)size.width();
        fittedSize.setWidth(qRound(d->targetSize.height() / aspectRatio));
        fittedSize.setHeight(d->targetSize.height());
        result = true;
    } else {
        result = false;
    }

    if (clippedRect != 0) {
        if (isFitOptionEnabled(RespectMaxTargetSize) && fittedSize.width() > d->maxTargetSize.width()) {
            fittedSize.setWidth(d->maxTargetSize.width());
            clip(size, fittedSize, clippedRect);
        } else {
            // no clipping, select entire image
            clippedRect->setTopLeft(QPoint(0, 0));
            clippedRect->setSize(size);
        }
    }

    return result;
}

bool SizeFitter::exactFit(QSize size, QSize &fittedSize, QRect *clippedRect) const
{
    bool result;

    QSize orientedTargetSize = getOrientedTargetSize(size);

    // enlarge?
    if (!isFitOptionEnabled(Enlarge) &&
        size.width() <= orientedTargetSize.width() &&
        size.height() <= orientedTargetSize.height()) {
        fittedSize = size;
        if (clippedRect != 0) {
            clippedRect->setTopLeft(QPoint(0, 0));
            clippedRect->setSize(fittedSize);
        }
        return false;
    }
    
    fittedSize = orientedTargetSize;
    if (size != fittedSize) {
        result = true;
    } else {
        result = false;
    }

    if (clippedRect != 0) {
        clip(size, orientedTargetSize, clippedRect);
    }
    return result;
}

void SizeFitter::clip(const QSize &size, const QSize &targetSize, QRect *clippedRect) const
{
    float aspectRatio, targetAspectRatio;
    int newWidth, newHeight;
    QSize newSize;

    // center the visible area
    /*!\todo Implement CENTER, N, NE, E, SE etc. gravity */
    aspectRatio       = (float)size.width() / (float)size.height();
    targetAspectRatio = (float)targetSize.width() / (float)targetSize.height();
    if (aspectRatio < targetAspectRatio) {
        // height too large
        newHeight = qRound(size.width() / targetAspectRatio);
        clippedRect->setTopLeft(QPoint(0, qRound((size.height() - newHeight) / 2.0f)));
        newSize.setWidth(size.width());
        newSize.setHeight(newHeight);
        clippedRect->setSize(newSize);
    } else {
        // width too large
        newWidth = qRound(size.height() * targetAspectRatio);
        clippedRect->setTopLeft(QPoint(qRound((size.width() - newWidth) / 2.0f), 0));
        newSize.setWidth(newWidth);
        newSize.setHeight(size.height());
        clippedRect->setSize(newSize);
    }
}


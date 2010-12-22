#include <limits>

#include <QtCore/QSize>
#include <QtCore/QRect>
#include <QtCore/QBitArray>

#include "SizeFitter.h"

const int SizeFitter::InvalidSize = std::numeric_limits<int>::min();

// public

SizeFitter::SizeFitter(QSize targetSize, FitMode fitMode)
    : m_targetSize(targetSize),
    m_fitMode(fitMode)
{
    this->setDefaultFitOptions();
}

SizeFitter::SizeFitter()
    : m_targetSize(QSize(640, 480)),
    m_fitMode(ExactFit)
{
    this->setDefaultFitOptions();
}

SizeFitter::SizeFitter(const SizeFitter &other)
{
    copy(other);
}

SizeFitter::~SizeFitter()
{}

void SizeFitter::setTargetSize(QSize newTargetSize)
{
    if (m_targetSize != newTargetSize) {
        m_targetSize = newTargetSize;
        emit changed();
    }
}

QSize SizeFitter::getTargetSize() const
{
    return m_targetSize;
}

void SizeFitter::setTargetWidth(int newTargetWidth)
{
    if (m_targetSize.width() != newTargetWidth) {
        m_targetSize.setWidth(newTargetWidth);
        emit changed();
    }
}

void SizeFitter::setTargetHeight(int newTargetHeight)
{
    if (m_targetSize.height() != newTargetHeight) {
        m_targetSize.setHeight(newTargetHeight);
        emit changed();
    }
}

void SizeFitter::setFitMode(FitMode newFitMode)
{
    if (m_fitMode != newFitMode) {
        m_fitMode = newFitMode;
        emit changed();
    }
}

SizeFitter::FitMode SizeFitter::getFitMode() const
{
    return m_fitMode;
}

void SizeFitter::setFitOptionEnabled(FitOption fitOption, bool enable)
{
    if (m_fitOptionArray.testBit(fitOption) != enable) {
        m_fitOptionArray.setBit(fitOption, enable);
        emit changed();
    }
}

bool SizeFitter::isFitOptionEnabled(FitOption fitOption) const
{
    return m_fitOptionArray.testBit(fitOption);
}

bool SizeFitter::fit(QSize size, QSize &fittedSize, QRect *clippedArea) const
{
    bool result;

    // fit at all?
    if (m_fitMode == NoFit) {
        return false;
    }

    switch (m_fitMode) {
    case Fit:
        result = this->fitIt(size, fittedSize, clippedArea);
        break;

    case FitToWidth:
        result = this->fitToWidth(size, fittedSize, clippedArea);
        break;

    case FitToHeight:
        result = this->fitToHeight(size, fittedSize, clippedArea);
        break;

    case ExactFit:
        result = this->exactFit(size, fittedSize, clippedArea);
        break;

    default:
        result = false;
#ifdef DEBUG
        qWarning("SizeFitter::fit: Fit mode '%d' not implemented!", m_fitMode);
#endif
        break;
    }

    return result;
}

SizeFitter SizeFitter::operator=(const SizeFitter &other)
{
    copy(other);
    return *this;
}

// private

void SizeFitter::setDefaultFitOptions()
{
    m_fitOptionArray.resize(NofFitOptions),
    m_fitOptionArray.setBit(RespectOrientation, true);
    m_fitOptionArray.setBit(Enlarge, false);
}

QSize SizeFitter::getOrientedTargetSize(const QSize &size) const
{
    QSize orientedTargetSize;

    if (this->isFitOptionEnabled(RespectOrientation) == true &&
        size.width() < size.height() &&
        m_targetSize.width() > m_targetSize.height()) {
        // make 'size' and 'target' have the same orientation
        orientedTargetSize.setWidth(m_targetSize.height());
        orientedTargetSize.setHeight(m_targetSize.width());
    } else {
        orientedTargetSize = m_targetSize;
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
        size.width() <= m_targetSize.width()) {
        fittedSize = size;
        if (clippedArea != 0) {
            clippedArea->setTopLeft(QPoint(0, 0));
            clippedArea->setSize(fittedSize);
        }
        return false;
    }

    if (size.width() != m_targetSize.width()) {
        result = true;
        aspectRatio = (float)size.width() / (float)size.height();
        fittedSize.setWidth(m_targetSize.width());
        fittedSize.setHeight(qRound(m_targetSize.width() / aspectRatio));
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
        size.height() <= m_targetSize.height()) {
        fittedSize = size;
        if (clippedArea != 0) {
            clippedArea->setTopLeft(QPoint(0, 0));
            clippedArea->setSize(fittedSize);
        }
        return false;
    }

    if (size.height() != m_targetSize.height()) {
        result = true;
        aspectRatio = (float)size.height() / (float)size.width();
        fittedSize.setWidth(qRound(m_targetSize.height() / aspectRatio));
        fittedSize.setHeight(m_targetSize.height());
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

void SizeFitter::copy(const SizeFitter &other)
{
    m_fitMode = other.m_fitMode;
    m_fitOptionArray = other.m_fitOptionArray;
    m_targetSize = other.m_targetSize;
}

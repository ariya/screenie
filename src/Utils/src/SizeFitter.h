#ifndef SIZEFITTER_H
#define SIZEFITTER_H

#include <QtCore/QSize>
#include <QtCore/QRect>
#include <QtCore/QBitArray>
#include <QtCore/QObject>

#include "UtilsLib.h"

class SizeFitterPrivate;

/*!
 * Calculates the size of the scaled and fitted source size. The size
 * is fitted and clipped according to FitMode.
 */
class SizeFitter : public QObject
{
    Q_OBJECT
public:

    UTILS_API static const int InvalidSize;

    enum FitMode {
        NoFit       = 0,
        Fit         = 1,
        FitToWidth  = 2,
        FitToHeight = 3,
        ExactFit    = 4
    };

    enum FitOption {
        RespectOrientation   = 0,
        Enlarge              = 1,
        RespectMaxTargetSize = 2, /*!< Respects the values as set by #setMaxTargetSize() by clipping the resulting image as needed. */
        NofFitOptions
    };

    UTILS_API SizeFitter(QSize targetSize, FitMode fitMode = Fit);

    /*!
     * Copy c'tor.
     */
    UTILS_API SizeFitter(const SizeFitter &other);
    UTILS_API SizeFitter();
    UTILS_API virtual ~SizeFitter();

    UTILS_API QSize getTargetSize() const;

    /*!
     * Adjusts the \em maximum target size, if necessary.
     *
     * \sa #setMaxTargetSize(QSize)
     */
    UTILS_API void setTargetSize(QSize targetSize);

    UTILS_API void setTargetWidth(int targetWidth);
    UTILS_API void setTargetHeight(int targetHeight);

    UTILS_API QSize getMaxTargetSize() const;

    /*!
     * Adjusts the \em target size, if necessary.
     *
     * \sa #setTargetSize(QSize)
     */
    UTILS_API void setMaxTargetSize(QSize maxTargetSize);

    UTILS_API void setFitMode(FitMode fitMode);
    UTILS_API FitMode getFitMode() const;

    UTILS_API bool isFitOptionEnabled(FitOption fitOption) const;
    UTILS_API QBitArray getFitOptions() const;
    UTILS_API void setFitOptionEnabled(FitOption fitOption, bool enable);
    UTILS_API void setFitOptions(const QBitArray &fitOptions);

    /*!
     * \param size
     *        the QSize to be fit into the \em target size
     * \param fittedSize
     *        the resulting fitted QSize
     * \param clippedRect
     *        the QRect which contains the visible area which has been clipped;
     *        the QRect coordinates refer to the original image \c size; may be set to 0
     * \return \c true if the \c size has been changed to the \em target size; \c false else
     */
    UTILS_API bool fit(QSize size, QSize &fittedSize, QRect *clippedRect = 0) const;

    UTILS_API SizeFitter operator=(const SizeFitter &other);

signals:
    void changed();

private:
    SizeFitterPrivate *d;

    void setDefaultFitOptions();
    // returns the oriented target size, that is with the same orientation as the 'size' (portrait or landscape),
    // IF the 'RespectOrientation' option is set; else the returned target size is simply 'targetSize'
    QSize getOrientedTargetSize(const QSize &size) const;

    bool fitIt(QSize size, QSize &fittedSize, QRect *clippedRect) const;
    bool fitToWidth(QSize size, QSize &fittedSize, QRect *clippedRect) const;
    bool fitToHeight(QSize size, QSize &fittedSize, QRect *clippedRect) const;
    bool exactFit(QSize size, QSize &fittedSize, QRect *clippedRect) const;

    void clip(const QSize &size, const QSize &targetSize, QRect *clippedRect) const;
};

#endif // SIZEFITTER_H


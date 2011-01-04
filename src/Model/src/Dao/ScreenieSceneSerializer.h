#ifndef SCREENIESCENESERIALISATION_H
#define SCREENIESCENESERIALISATION_H

#include <QtCore/QByteArray>

class ScreenieScene;

class ScreenieSceneSerializer
{
public:
    enum Mode {
        FullScene,
        SelectedItems
    };

    virtual ~ScreenieSceneSerializer() {}

    virtual QByteArray serialize(const ScreenieScene &screenieScene, Mode mode) = 0;

    virtual ScreenieScene *deserialize(QByteArray &data) const = 0;
};

#endif // SCREENIESCENESERIALISATION_H

#include <QtCore/QString>
#include "Version.h"

class VersionPrivate
{
public:
    static const int Major;
    static const int Minor;
    static const int SubMinor;
    static const QString CodeName;
    static const QString ApplicationTitle;
};

const int VersionPrivate::Major = 0;
const int VersionPrivate::Minor = 1;
const int VersionPrivate::SubMinor = 0;
const QString VersionPrivate::CodeName = QString ("Anarchic Amoeba");
const QString VersionPrivate::ApplicationTitle = "Screenie";

// public

int Version::getMajor()
{
    return VersionPrivate::Major;
}

int Version::getMinor()
{
    return VersionPrivate::Minor;
}

int Version::getSubMinor()
{
    return VersionPrivate::SubMinor;
}

QString Version::getCodeName()
{
    return VersionPrivate::CodeName;
}

QString Version::getApplicationName()
{
    return VersionPrivate::ApplicationTitle;
}

QString Version::toString()
{
    return QString("%1.%2.%3").arg(VersionPrivate::Major).arg(VersionPrivate::Minor).arg(VersionPrivate::SubMinor);
}

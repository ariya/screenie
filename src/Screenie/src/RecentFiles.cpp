#include <QtCore/QObject>
#include <QtCore/QFileInfo>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QKeySequence>

#include "../../Utils/src/Settings.h"
#include "RecentFiles.h"

namespace
{
    const Qt::Key ActionKeys[] = {Qt::Key_1, Qt::Key_2, Qt::Key_3, Qt::Key_4, Qt::Key_5, Qt::Key_6, Qt::Key_7, Qt::Key_8, Qt::Key_9, Qt::Key_0};
}

// **************
// public methods
// **************

RecentFiles::RecentFiles()
    : m_recentFilesActionGroup(new QActionGroup(this)),
    m_ignoreSettingsChanged(false)
{
    initialise();
    frenchConnection();
}

RecentFiles::~RecentFiles()
{}

QActionGroup &RecentFiles::getRecentFilesActionGroup() const
{
    return *m_recentFilesActionGroup;
}

// ***************
// private methods
// ***************

void RecentFiles::initialise()
{
    QAction *action;
    Settings &settings = Settings::getInstance();
    for (int i = 0; i < settings.getMaxRecentFiles(); i++) {
        action = new QAction(this);
        action->setVisible(false);
        connect(action, SIGNAL(triggered()),
                this, SLOT(handleRecentFilesAction()));
        m_recentFilesActionGroup->addAction(action);
    }
    updateRecentFilesActions();

    QAction *separator = new QAction(this);
    separator->setSeparator(true);
    m_recentFilesActionGroup->addAction(separator);

    m_clearRecentFilesAction = new QAction(tr("Clear &menu"), this);
    m_recentFilesActionGroup->addAction(m_clearRecentFilesAction);
}

void RecentFiles::frenchConnection()
{
    Settings &settings = Settings::getInstance();

    connect(&settings, SIGNAL(changed()),
            this, SLOT(updateRecentFilesActions()));
    connect(m_clearRecentFilesAction, SIGNAL(triggered()),
            this, SLOT(clearRecentFilesMenu()));

}

// *************
// private slots
// *************

void RecentFiles::updateRecentFilesActions()
{
    Settings &settings = Settings::getInstance();
    int i;

    if (m_ignoreSettingsChanged) {
        return;
    }

    QStringList recentFiles = settings.getRecentFiles();

    const int originalSize = recentFiles.count();
    int nofRecentFiles = qMin(recentFiles.count(), settings.getMaxRecentFiles());
    const QList<QAction *> recentFilesActions = m_recentFilesActionGroup->actions();

    for (i = 0; i < nofRecentFiles; ++i) {
        const QFileInfo fileInfo(recentFiles[i]);

        if (fileInfo.exists() == false) {
            recentFiles.removeAt(i);
            --i;
            nofRecentFiles = qMin(recentFiles.count(), settings.getMaxRecentFiles());
            continue;
        }
        const QString text = fileInfo.fileName();
        recentFilesActions[i]->setText(text);
        recentFilesActions[i]->setData(recentFiles[i]);
        recentFilesActions[i]->setVisible(true);
        recentFilesActions[i]->setShortcut(QKeySequence(ActionKeys[i] | Qt::CTRL));
    }

    // set all not yet used actions to invisible
    for (i = nofRecentFiles; i < settings.getMaxRecentFiles(); ++i) {
        recentFilesActions[i]->setVisible(false);
    }

    // If there's been a change, write it back
    if (originalSize != recentFiles.count()) {
        m_ignoreSettingsChanged = true;
        settings.setRecentFiles(recentFiles);
        m_ignoreSettingsChanged = false;
    }
}

void RecentFiles::handleRecentFilesAction()
{
    Settings &settings = Settings::getInstance();
    if (const QAction *action = qobject_cast<const QAction *>(sender())) {
        QString recentFilePath = action->data().toString();
        m_ignoreSettingsChanged = true;
        settings.removeRecentFile(recentFilePath);
        m_ignoreSettingsChanged = false;
        settings.addRecentFile(recentFilePath);
#ifdef DEBUG
        qDebug("RecentFiles::handleRecentFilesAction: %s", qPrintable(recentFilePath));
#endif
        emit openRecentFile(recentFilePath);
    }
}

void RecentFiles::clearRecentFilesMenu()
{
    Settings &settings = Settings::getInstance();
    settings.setRecentFiles(QStringList());
}

#ifndef RECENTFILES_H
#define RECENTFILES_H

#include <QtCore/QObject>

class QActionGroup;
class QAction;

/*!
 * \brief The recently opened files.
 *
 * Provides a menu with QActions which give access to the recently opened files.
 */
class RecentFiles : public QObject
{
    Q_OBJECT

public:
    RecentFiles();
    virtual ~RecentFiles();

    QActionGroup &getRecentFilesActionGroup() const;

signals:
    void openRecentFile(const QString &filePath);

private:
    QActionGroup *m_recentFilesActionGroup;
    bool m_ignoreSettingsChanged;
    QAction *m_clearRecentFilesAction;

    void initialise();
    void frenchConnection();

private slots:
    void updateRecentFilesActions();
    void handleRecentFilesAction();
    void clearRecentFilesMenu();
};

#endif // RECENTFILES_H

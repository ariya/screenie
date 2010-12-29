#ifndef TEMPLATEORGANIZER_H
#define TEMPLATEORGANIZER_H

#include <QtCore/QObject>
#include <QtCore/QList>

class ScreenieScene;
class ScreenieModelInterface;
class ScreenieTemplateModel;
class TemplateOrganizerPrivate;

class TemplateOrganizer : public QObject
{
    Q_OBJECT
public:
    TemplateOrganizer(const ScreenieScene &screenieScene);
    ~TemplateOrganizer();

    QList<ScreenieTemplateModel *> getOrderedTemplates() const;

private:
    TemplateOrganizerPrivate *d;

    void frenchConnection();

private slots:
    void handleModelAdded(ScreenieModelInterface &screenieModel);
    void handleModelRemoved(ScreenieModelInterface &screenieModel);
};

#endif // TEMPLATEORGANIZER_H

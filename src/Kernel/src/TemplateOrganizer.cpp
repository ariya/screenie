#include <QtCore/QtAlgorithms>
#include <QtCore/QList>

#include "../../Model/src/ScreenieScene.h"
#include "../../Model/src/ScreenieTemplateModel.h"
#include "TemplateOrganizer.h"

namespace
{
    bool orderSort(const ScreenieTemplateModel *item1, const ScreenieTemplateModel *item2)
    {
        // lower order comes first in the sorted list
        return item1->getOrder() < item2->getOrder();
    }
}; // anonymous

class TemplateOrganizerPrivate
{
public:
    TemplateOrganizerPrivate(const ScreenieScene &theScreenieScene)
        : screenieScene(theScreenieScene),
          ordered(false)
    {}

    const ScreenieScene &screenieScene;
    QList<ScreenieTemplateModel *> templates;
    bool ordered;
};

// public

TemplateOrganizer::TemplateOrganizer(const ScreenieScene &screenieScene)
    : d(new TemplateOrganizerPrivate(screenieScene))
{
    foreach (ScreenieModelInterface *screenieModel, screenieScene.getModels()) {
        handleModelAdded(*screenieModel);
    }

    frenchConnection();
}

TemplateOrganizer::~TemplateOrganizer()
{
    delete d;
}

QList<ScreenieTemplateModel *> TemplateOrganizer::getOrderedTemplates() const
{
    if (!d->ordered) {
        ::qSort(d->templates.begin(), d->templates.end(), orderSort);
        d->ordered = true;
    }
    return d->templates;
}

// private

void TemplateOrganizer::frenchConnection()
{
    connect(&d->screenieScene, SIGNAL(modelAdded(ScreenieModelInterface &)),
            this, SLOT(handleModelAdded(ScreenieModelInterface &)));
    connect(&d->screenieScene, SIGNAL(modelRemoved(ScreenieModelInterface &)),
            this, SLOT(handleModelRemoved(ScreenieModelInterface &)));
}

// private slots

void TemplateOrganizer::handleModelAdded(ScreenieModelInterface &screenieModel)
{
    ScreenieTemplateModel *screenieTemplateModel = qobject_cast<ScreenieTemplateModel *>(&screenieModel);
    if (screenieTemplateModel != 0) {
        d->templates.append(screenieTemplateModel);
        screenieTemplateModel->setOrder(d->templates.count());
        d->ordered = false;
    }
}

void TemplateOrganizer::handleModelRemoved(ScreenieModelInterface &screenieModel)
{
    ScreenieTemplateModel *screenieTemplateModel = qobject_cast<ScreenieTemplateModel *>(&screenieModel);
    if (screenieTemplateModel != 0) {
        int index = d->templates.indexOf(screenieTemplateModel, 0);
        if (index != -1) {
            d->templates.removeAt(index);
        }
    }
}

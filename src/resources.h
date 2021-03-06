#pragma once

#include <OsmAndCore/ResourcesManager.h>
#include <QObject>
#include <memory>

class ResourceModel;

class Resources: public QObject {
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> resourcesInRepository READ resourcesInRepository NOTIFY repositoryUpdated)

public:
    Resources(QObject *parent = 0);
    virtual ~Resources();

    std::shared_ptr<OsmAnd::ResourcesManager> resourcesManager;

    QList<QObject *> resourcesInRepository();

    void downloadIfNecessary(const QList<QString>& resourceIds);

signals:
    void repositoryUpdated();
    void localResourceChanged(QString);

public slots:
};

class ResourceModel: public QObject {
    Q_OBJECT
    Q_PROPERTY(QString id READ id NOTIFY resourceChanged)
    Q_PROPERTY(uint64_t size READ size NOTIFY resourceChanged)
    Q_PROPERTY(QDateTime timestamp READ timestamp NOTIFY resourceChanged)
    Q_PROPERTY(bool installed READ installed NOTIFY resourceChanged)
    Q_PROPERTY(bool upToDate READ upToDate NOTIFY resourceChanged)

public:
    ResourceModel(Resources *res, const std::shared_ptr<const OsmAnd::ResourcesManager::ResourceInRepository>& resourceInRepo);
    virtual ~ResourceModel();

    QString id() const { return resourceInRepo->id; }
    uint64_t size() const { return resourceInRepo->size; }
    QDateTime timestamp() const {
        QDateTime dateTime;
        dateTime.setMSecsSinceEpoch(resourceInRepo->timestamp);
        return dateTime;
    }
    bool installed() const {
        return res->resourcesManager->isResourceInstalled(id());
    }
    bool upToDate() const {
        return res->resourcesManager->isInstalledResourceOutdated(id());
    }

    Q_INVOKABLE void install() { res->resourcesManager->installFromRepository(id()); }
    Q_INVOKABLE void update() { res->resourcesManager->updateFromRepository(id()); }

signals:
    void resourceChanged();

private:
    Resources *res;
    std::shared_ptr<const OsmAnd::ResourcesManager::ResourceInRepository> resourceInRepo;
};

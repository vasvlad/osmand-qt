#include <OsmAndCore/Logging.h>
#include <OsmAndCore/CoreResourcesEmbeddedBundle.h>
#include <OsmAndCore.h>

#include <QDebug>
#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>

#include "map.h"
#include "resources.h"

using namespace OsmAnd;
using namespace std;

extern "C" void __CoreResourcesEmbeddedBundle__FakeReferences();

int main(int argc, char *argv[]) {
  Q_INIT_RESOURCE(resources);

  QGuiApplication app(argc, argv);
  app.setApplicationName("osmand-qt");
  app.setApplicationVersion("0.1");

  __CoreResourcesEmbeddedBundle__FakeReferences();
  shared_ptr<const CoreResourcesEmbeddedBundle> bundle =
    CoreResourcesEmbeddedBundle::loadFromCurrentExecutable();
  InitializeCore(bundle);

  OsmAnd::LogPrintf(OsmAnd::LogSeverityLevel::Info, "hello world from osmand");
  qDebug() << "hello world from qt";

  qmlRegisterType<Map>("OsmAndQt", 1, 0, "Map");
  qmlRegisterType<Resources>("OsmAnd", 1, 0, "Resources");

  Resources resources;
  resources.makeSureWorldMapIsInstalled();

  QQuickView view;
  view.rootContext()->setContextProperty("contextResources", &resources);
  view.setResizeMode(QQuickView::SizeRootObjectToView);
  view.setSource(QUrl("qrc:/src/map.qml"));
  view.show();

  return app.exec();
}

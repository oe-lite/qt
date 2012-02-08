#include <QCoreApplication>
#include <QDeclarativeItem>
#include <QDeclarativeComponent>
#include <QDeclarativeEngine>
#include <QDeclarativeProperty>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
 
int main(int argc, char ** argv) {

  QApplication app(argc, argv);
  app.setApplicationName("Calculator demo");
  QGraphicsView *view = new QGraphicsView;
  view->setGeometry(QRect(0, 0, 800, 480));
  QGraphicsScene *scene = new QGraphicsScene(0, 0, 800, 480);
  view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view->setFrameStyle(QFrame::NoFrame);
  view->setScene(scene);

  QDeclarativeEngine    engine;
  QDeclarativeComponent component(&engine, QUrl::fromLocalFile("calculator.qml"));
  QDeclarativeItem      *pItem = qobject_cast<QDeclarativeItem *>(component.create());

  scene->addItem(pItem);

  view->show();

  return app.exec();
}

#include <QApplication>
#include <QGraphicsView>
#include "PlayingArea.hpp"

int main(int argc, char ** argv)
{
  // QApplication app( argc, argv );
  
  // QGraphicsScene scene( QRect( -50, -50, 400, 200 ) );
  
  // QGraphicsRectItem *rectItem = new QGraphicsRectItem( QRect( -25, 25, 200, 40 ), 0, &scene );
  // rectItem->setPen( QPen( Qt::red, 3, Qt::DashDotLine ) );
  // rectItem->setBrush( Qt::gray );
    
  // QGraphicsSimpleTextItem *textItem = new QGraphicsSimpleTextItem( "Foundations of Qt", 0, &scene );
  // textItem->setPos( 50, 0 );
  
  // QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem( QRect( 170, 20, 100, 75 ), 0, &scene );
  // ellipseItem->setPen( QPen(Qt::darkBlue) );
  // ellipseItem->setBrush( Qt::blue );
    
  // QGraphicsPolygonItem *polygonItem = new QGraphicsPolygonItem( QPolygonF( QVector<QPointF>() << QPointF( 10, 10 ) << QPointF( 0, 90 ) << QPointF( 40, 70 ) << QPointF( 80, 110 ) << QPointF( 70, 20 ) ), 0, &scene );
  // polygonItem->setPen( QPen(Qt::darkGreen) );
  // polygonItem->setBrush( Qt::yellow );
  
  // QGraphicsView view;
  // view.setScene( &scene );
  // view.show();

  // return app.exec();

  QApplication app(argc, argv);

  PlayingArea area;
  QGraphicsView view;

    view.setScene( area.scene() );
    view.show();

    return app.exec();
}

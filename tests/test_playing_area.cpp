#include <QGraphicsView>
#include "PlayingArea.hpp""

int main()
{
    PlayingArea area;
    QGraphicsView view;

    view.setScene( area.scene() );
    view.show();
}

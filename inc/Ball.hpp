#ifndef _Ball
#define _Ball

class Ball : public QObject
{
  Q_OBJECT

  public:
  Ball(int xcoord, int ycoord, int radius);

private:
  int _abs;
  int _ord;
  int _rad;
}

#endif

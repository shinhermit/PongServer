#ifndef _PlayingArea
#define _PlayingArea

class PlayingArea : public QObject
{
Q_OBJECT

public:
  PlayingArea(const int & renderAreaWidth,
	      const Ball & ball);

private:
  int renderAreaWidth;
  Ball ball;
};

#endif

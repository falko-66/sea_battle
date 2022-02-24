#include "cell.h"

Cell::Cell(QObject *parent):
    _line(-1),
    _column(-1)
{
}

void Cell::setCoordinates(int line, int column)
{
    _line = line;
    _column = column;
}

void Cell::mousePressEvent(QMouseEvent *event)
{
    emit cell_clicked (_line, _column);
}

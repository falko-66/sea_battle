#ifndef CELL_H
#define CELL_H

#include <QPushButton>

class Cell:public QPushButton
{
    Q_OBJECT

private:
    int _line;
    int _column;

public:
    explicit Cell(QObject *parent = nullptr);
    void setCoordinates(int line, int column);

signals:
    void cell_clicked(int line, int column);

protected:
    void mousePressEvent(QMouseEvent *event);

};

#endif // CELL_H

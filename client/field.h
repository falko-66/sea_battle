#ifndef FIELD_H
#define FIELD_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <cell.h>

#define FIELD_LENGTH 10
#define FIELD_WIDTH 10

class Field : public QWidget
{
    Q_OBJECT

private:
    Cell *_cells[FIELD_LENGTH][FIELD_WIDTH];

private slots:
    void get_coordinates(int line, int column);

public:
    explicit Field(QWidget *parent = nullptr);
    void update_matrix(unsigned short **matrix);

signals:
    void send_move(int line, int column);
};

#endif // FIELD_H

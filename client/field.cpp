#include "field.h"
#include <game.h>

void Field::get_coordinates(int line, int column)
{
    emit send_move (line, column);
}

Field::Field(QWidget *parent) : QWidget(parent)
{
    QGridLayout *gridLayout = new QGridLayout(this);

    for(int i = 1; i < 11; ++i){

        QLabel *label = new QLabel(this);

        label->setText (QString("%1").arg (i - 1));

        label->setMinimumSize (100, 100);

        label->setAlignment (Qt::AlignCenter);

        gridLayout->addWidget (label, i , 0, 1, 1,Qt::AlignCenter);
    }

    for(int i = 1; i < 11; ++i){

        QLabel *label = new QLabel(this);

        char c = i + 64;

        label->setText (QString("%1").arg (c));

        label->setMinimumSize (100, 100);

        label->setAlignment (Qt::AlignCenter);

        gridLayout->addWidget (label, 0, i, 1, 1, Qt::AlignCenter);
    }

    for (int i =  1; i < FIELD_LENGTH + 1; ++i ) {
        for (int j = 1; j < FIELD_WIDTH + 1; ++j) {

            Cell *cell = new Cell(this);

            cell->setStyleSheet ("background-color: green");

            cell->setCoordinates (i - 1, j - 1);

            cell->setMinimumSize (100, 100);

            gridLayout->addWidget(cell, i + 1,  j + 1, 1, 1);

            _cells[i - 1][j - 1] = cell;

            connect (_cells[i - 1][j - 1], SIGNAL(cell_clicked(int,int)),
                    this, SLOT(get_coordinates(int,int)));
        }
    }
}


void Field::update_matrix(unsigned short **matrix)
{
    for (int i = 0; i < FIELD_LENGTH; ++i) {
        for(int j = 0; j < FIELD_WIDTH; ++j){

            if(matrix[i][j] == Empty)
                _cells[i][j]->setStyleSheet ("background-color: green");

            else if(matrix[i][j] == Neighbor)
                _cells[i][j]->setStyleSheet ("background-color: yellow");

            else if(matrix[i][j] == Engaged)
                _cells[i][j]->setStyleSheet ("background-color: blue");

            else if(matrix[i][j] == Miss)
                _cells[i][j]->setStyleSheet ("background-color: red");

            else if(matrix[i][j] == Hit)
                _cells[i][j]->setStyleSheet ("background-color: purple");

            else if(matrix[i][j] == Killed)
                _cells[i][j]->setStyleSheet ("background-color: rgb(100, 0, 0)");

        }
    }
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,_game(new Game())
    ,_timer (new QTimer(this))
{
    _start_pos.line = -1;
    _start_pos.column = -1;

    _finish_pos.line = -1;
    _finish_pos.column = -1;

    _count_ships.decks_1 = 4;
    _count_ships.decks_2 = 3;

    _count_ships.decks_3 = 2;
    _count_ships.decks_4 = 1;

    ui->setupUi(this);
    _timer->setInterval(1);
    _timer->start();

    connect(_timer, SIGNAL(timeout()), this, SLOT(_on_timer_tick()));

    connect (ui->field_ships, SIGNAL(send_move(int,int)),
             this, SLOT(_on_get_coordinates(int,int)));

    connect (_game, SIGNAL(state_changed(int)), this,
             SLOT(_on_state_changed(int)));

    connect (_game, &Game::got_username,
             this, &MainWindow::_on_set_username);

    connect (_game, SIGNAL(ships_changed(unsigned short**)), this,
             SLOT(_on_ships_changed(unsigned short**)));

    connect (_game, SIGNAL(hits_changed(unsigned short**)), this,
             SLOT(_on_hits_changed(unsigned short**)));

    QMessageBox :: information(this,
                               QString("Rules"),
                               QString("Before sending the username, you must arrange all "
"the ships (1 - four-decker, 2 - three-decks, 3 - double-decks , 4 - single-decks)."
" To install a ship, click on the field that is the beginning of the ship, and then "
"on the field that is the end of the ship. If you want to reset the start coordinate,"
" press the Esk key. If you want to send the username, press the Enter key."));
}

void MainWindow::reset_coordinates()
{
    _start_pos.line = -1;
    _start_pos.column = -1;
    _finish_pos.line = -1;
    _finish_pos.column = -1;
}

void MainWindow::setting_ship(int line, int column)
{
    if(_start_pos.line == -1 && _start_pos.column == -1){
        _start_pos.line = line;
        _start_pos.column = column;

    } else if(_start_pos.line != -1 && _start_pos.column != -1){

        _finish_pos.line = line;
        _finish_pos.column = column;

        if (_finish_pos.column < _start_pos.column){
            int tmp = _finish_pos.column;
            _finish_pos.column = _start_pos.column;
            _start_pos.column = tmp;
        }

        if(_finish_pos.line < _start_pos.line){
            int tmp = _finish_pos.line;
            _finish_pos.line = _start_pos.line;
            _start_pos.line = tmp;
        }

        if(!_game->is_it_possible_to_put_the_ship (_start_pos.line, _finish_pos.line,
                                                   _start_pos.column, _finish_pos.column)){

            QMessageBox::warning (this, "Error",
                                  "You enter incorrect coordinates, try again");

            reset_coordinates();

        }else if(_finish_pos.line - _start_pos.line == 3
                 || _finish_pos.column - _start_pos.column == 3){

            if(_count_ships.decks_4 > 0){

                _game->set_ship (_start_pos.line, _finish_pos.line,
                                 _start_pos.column, _finish_pos.column);
                ui->label_four_decker->setText
                        (QString("Four - decker %1/1").arg (2 - _count_ships.decks_4));
                ui->field_ships->update_matrix (_game->ships ());

                --_count_ships.decks_4;

                if(_count_ships.decks_4 == 0){
                    QFont font;
                    font.setStrikeOut(true);
                    ui->label_four_decker->setFont (font);
                }
                reset_coordinates();

            } else {

                QMessageBox::warning (this, "Error",
                                      "You have installed enough ships with lenght 4, "
                                          "choose another ship");

                reset_coordinates();
            }
        } else if(_finish_pos.line - _start_pos.line == 2
                  || _finish_pos.column - _start_pos.column == 2){

            if(_count_ships.decks_3 > 0){

                _game->set_ship (_start_pos.line, _finish_pos.line,
                                 _start_pos.column, _finish_pos.column);

                ui->label_three_deck->setText
                        (QString("Three - deck %1/2").arg (3 - _count_ships.decks_3));
                ui->field_ships->update_matrix (_game->ships ());

                --_count_ships.decks_3;

                if(_count_ships.decks_3 == 0){
                    QFont font;
                    font.setStrikeOut(true);
                    ui->label_three_deck->setFont (font);
                }
                reset_coordinates();

            } else {

                QMessageBox::warning (this, "Error",
                                      "You have installed enough ships with lenght 3, "
                                          "choose another ship");

                reset_coordinates();
            }

        } else if(_finish_pos.line - _start_pos.line == 1
                  || _finish_pos.column - _start_pos.column == 1){

            if(_count_ships.decks_2 > 0){

                _game->set_ship (_start_pos.line, _finish_pos.line,
                                 _start_pos.column, _finish_pos.column);
                ui->label_double_decker->setText
                        (QString("Double - decker %1/3").arg (4 - _count_ships.decks_2));
                ui->field_ships->update_matrix (_game->ships ());

                --_count_ships.decks_2;

                if(_count_ships.decks_2 == 0){
                    QFont font;
                    font.setStrikeOut(true);
                    ui->label_double_decker->setFont (font);
                }

                reset_coordinates();

            } else {

                QMessageBox::warning (this, "Error",
                                      "You have installed enough ships with lenght 2, "
                                          "choose another ship");

                reset_coordinates();
            }

        } else if(_finish_pos.line - _start_pos.line == 0
                  && _finish_pos.column - _start_pos.column == 0){

            if(_count_ships.decks_1 > 0){

                _game->set_ship (_start_pos.line, _finish_pos.line,
                                 _start_pos.column, _finish_pos.column);

                ui->label_single_deck->setText
                        (QString("Single - deck %1/4").arg (5 - _count_ships.decks_1));

                ui->field_ships->update_matrix (_game->ships ());

                --_count_ships.decks_1;

                if(_count_ships.decks_1 == 0){
                    QFont font;
                    font.setStrikeOut(true);
                    ui->label_single_deck->setFont (font);
                }

                reset_coordinates();

            } else {

                QMessageBox::warning (this, "Error",
                                      "You have installed enough ships with lenght 1, "
                                          "choose another ship");

                reset_coordinates();
            }

        } else {

            QMessageBox::warning (this, "Error",
                                  "You try to set ship with len more then 4, "
                                      "choose another ship");

            reset_coordinates();
        }
    }

    if (_count_ships.decks_4 == 0 && _count_ships.decks_3 == 0 &&
            _count_ships.decks_2 == 0 && _count_ships.decks_1 == 0){

        _game->set_ships_spaced (true);

        disconnect (ui->field_ships, SIGNAL(send_move(int,int)),
                    this, SLOT(_on_get_coordinates(int,int)));
        ui->label_state->setText ("Ready to send username");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _game;
}

void MainWindow::on_pushButton_send_username_clicked()
{
    if(_game->ships_spaced ()){

        ui->lineEdit->setEnabled (false);

        ui->pushButton_send_username->setEnabled (false);

        QString username = ui->lineEdit->text ();
        QByteArray ba = username.toLocal8Bit ();

        _game->receive_username (ba.data ());
    } else
        QMessageBox :: warning(this, "Error",
                               "You must set all of your ships before send username");
}

void MainWindow::_on_get_coordinates(int line, int column)
{
    if(!_game->ships_spaced ())
        setting_ship (line, column);

    else if(_game->is_valid_move (line, column))
        _game->make_move (line, column);

    else if(!_game->is_valid_move (line, column))
        QMessageBox::warning (this, "Error",
                              "You try to shoot in field which is not empty ");
}

void MainWindow::_on_set_username(char username[MAXLINE])
{
    ui->label_other_username->
            setText (QString("Opponent username: %1").arg (username));

    _on_disconnect_cells_hits (false);
}

void MainWindow::_on_ships_changed(unsigned short **matrix)
{
    ui->field_ships->update_matrix (matrix);
}

void MainWindow::_on_hits_changed(unsigned short **matrix)
{
    ui->field_hits->update_matrix (matrix);
}

void MainWindow::_on_state_changed(int state)
{
    if (state == Making_move){
        ui->label_state->setText ("Making move");

        if(_game->username_received ())
            _on_disconnect_cells_hits (false);
    }

    if (state == Waiting_move){
        ui->label_state->setText ("Waiting move");
        _on_disconnect_cells_hits (true);
    }

    if (state == Winner){
        QMessageBox::information (this,
                                  QString("Congratulations"),
                                  QString("You won this match!!!"));

        ui->label_state->setText ("Winner");
        _on_disconnect_cells_hits (true);
    }

    if (state == Loser){
        QMessageBox::information (this,
                                  QString("Result of match"),
                                  QString("You lose this match!!!"));

        ui->label_state->setText ("Loser");
        _on_disconnect_cells_hits (true);
    }
}

void MainWindow::_on_disconnect_cells_hits(bool flag)
{
    if(!flag)
        connect (ui->field_hits, SIGNAL(send_move(int,int)),
                 this, SLOT(_on_get_coordinates(int,int)));

    else
        disconnect (ui->field_hits, SIGNAL(send_move(int,int)),
                    this, SLOT(_on_get_coordinates(int,int)));
}

void MainWindow::_on_timer_tick()
{
    if(!_game->username_received ())
        _game->receive_username ();

    else if(_game->state() == Waiting_move)
        if(_game->receive_move ())
            _game->process_move ();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key ()== Qt::Key_Escape){
        if(!_game->ships_spaced ())
            reset_coordinates ();

    }else if(event->key() == Qt::Key_Return){

        if(_game->ships_spaced ()){

            ui->lineEdit->setEnabled (false);

            ui->pushButton_send_username->setEnabled (false);

            QString username = ui->lineEdit->text ();
            QByteArray ba = username.toLocal8Bit ();

            _game->receive_username (ba.data ());
        } else
            QMessageBox :: warning(this, "Error",
                                   "You must set all of your ships before send username");
    }
}

void MainWindow::on_toolButton_clicked()
{
    QMessageBox :: information(this,
                               QString("Rules"),
                               QString("Before sending the username, you must arrange all "
"the ships (1 - four-decker, 2 - three-decks, 3 - double-decks , 4 - single-decks)."
" To install a ship, click on the field that is the beginning of the ship, and then "
"on the field that is the end of the ship. If you want to reset the start coordinate,"
" press the Esk key. If you want to send the username, press the Enter key."));
}

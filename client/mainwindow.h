#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMessageBox>
#include <QString>
#include <QByteArray>
#include <QTimer>
#include <field.h>
#include <game.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:

    struct CountShips
    {
        int decks_4;
        int decks_3;
        int decks_2;
        int decks_1;
    };

    MainWindow(QWidget *parent = nullptr);

    void reset_coordinates();

    void setting_ship(int line, int column);

    ~MainWindow();

private slots:
    void on_pushButton_send_username_clicked();

    void _on_get_coordinates(int line, int column);

    void _on_set_username(char username[MAXLINE]);

    void _on_ships_changed(unsigned short **matrix);

    void _on_hits_changed(unsigned short ** matrix);

    void _on_state_changed(int state);

    void _on_disconnect_cells_hits(bool flag);

    void _on_timer_tick();

    void on_toolButton_clicked();

private:

    Ui::MainWindow *ui;

    Game *_game;

    Move _start_pos;

    Move _finish_pos;

    CountShips _count_ships;

    QTimer *_timer;

protected:
    void keyPressEvent(QKeyEvent *event);
};
#endif // MAINWINDOW_H

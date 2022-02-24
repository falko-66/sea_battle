#ifndef GAME_H
#define GAME_H
#include <QWidget>
#include<_game.h>

class Game : public QWidget
{
    Q_OBJECT

private:

    unsigned short **_ships;

    unsigned short **_hits;

    int _state;

    char _my_username[MAXLINE];

    char _other_username[MAXLINE];

    NetworkContext *_network_context;

    Move _move;

    Answer _answer;

    bool _ships_spaced;

    bool _username_received;

public:

    explicit Game(QWidget *parent = nullptr);

    void receive_username(char username[MAXLINE]);

    void set_ship(int str1, int str2, int column1, int column2);

    void make_move(int line, int column);

    bool is_valid_move(int line, int column);

    bool receive_move();

    bool receive_username();

    void process_move();

    void set_ships_spaced(bool ships_spaced);

    bool is_it_possible_to_put_the_ship(int str1, int str2,
                                        int column1, int column2);


    int state() const;

    unsigned short **ships() const;

    bool ships_spaced() const;

    bool username_received() const;

signals:

    void got_username(char username[MAXLINE]);

    void ships_changed(unsigned short **matrix);

    void hits_changed(unsigned short **matrix);

    void state_changed(int state);

    void disconnect_cells(bool flag);
};

#endif // GAME_H

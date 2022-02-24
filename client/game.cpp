#include "game.h"
#include<_game.h>

Game::Game(QWidget *parent):
    QWidget (parent)
    ,_state(Unknown)
    ,_my_username("")
    ,_other_username("")
    ,_ships_spaced(false)
    ,_username_received(false)
{
    _move.column = -1;
    _move.line = -1;

    _answer.is_any_live_ship = true;
    _answer.result_of_shoot = -1;

    _ships = _create_matrix ();

    _hits = _create_matrix ();

    _network_context = _network_init (PORT);
}

bool Game::username_received() const
{
    return _username_received;
}

bool Game::ships_spaced() const
{
    return _ships_spaced;
}

bool Game::is_it_possible_to_put_the_ship( int str1,
                                           int str2, int column1, int column2)
{
    return _is_it_possible_to_put_the_ship(_ships, str1, str2, column1, column2);
}

bool Game::is_valid_move(int line, int column)
{
    return _is_valid_move(_hits, line, column);
}

unsigned short **Game::ships() const
{
    return _ships;
}

bool Game::receive_move()
{    
    _move = _receive_move(_network_context);
    return !(_move.line < 0 || _move.column < 0);
}

bool Game::receive_username()
{
    _receive_username (_network_context, _other_username);

    if(strcmp (_other_username, "") == 0)
        return false;

    emit got_username (_other_username);
    _username_received = true;

    return true;
}

void Game::process_move()
{
    _answer.result_of_shoot = _result_of_shot (_ships, _move.line, _move.column);

    _answer.is_any_live_ship = true;

    _ships[_move.line][_move.column] = _answer.result_of_shoot;

    if(_answer.result_of_shoot == Killed){
        _change_after_kill (_ships, _move.line, _move.column);
        _answer.is_any_live_ship = _is_any_live_ship (_ships);
    }

    emit ships_changed (_ships);

    send_answer(_network_context, _answer);

    if(_answer.result_of_shoot != Hit && _answer.result_of_shoot != Killed){
        _state = Making_move;
        emit state_changed (_state);
    }

    if (!_is_any_live_ship (_ships)){
        _state = Loser;
        emit state_changed (_state);
    }

}

void Game::set_ships_spaced(bool ships_spaced){_ships_spaced = ships_spaced;}

int Game::state() const{return _state;}

void Game::receive_username(char username[MAXLINE])
{
    strcpy (_my_username, username);

    _send_username(_network_context, _my_username);

    _state = receive_state(_network_context);

    emit state_changed (_state);

}

void Game::set_ship(int str1, int str2, int column1, int column2)
{_set_ship (_ships, str1, str2, column1, column2);}

void Game::make_move(int line, int column)
{
    _move.line = line;
    _move.column = column;

    while (true){
        send_move(_network_context, _move);

        if (receive_confirm(_network_context) > 0)
            break;

    }

    _answer = receive_answer(_network_context);

    _hits[_move.line][_move.column ] = _answer.result_of_shoot;

    if (_answer.result_of_shoot == Killed){
        _change_after_kill (_hits, _move.line, _move.column);

        if(_answer.is_any_live_ship == 0){
            _state = Winner;
            emit state_changed (_state);
        }
    }

    emit hits_changed (_hits);

    if(_answer.result_of_shoot != Hit && _answer.result_of_shoot != Killed){
        _state = Waiting_move;
        emit state_changed (_state);
    }
}

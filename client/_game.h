#ifndef _GAME_H
#define _GAME_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT	 8080
#define MAXLINE 1024

enum Condition
{
    ERROR = -1,
    Empty = 0,
    Neighbor = 1,
    Engaged = 2,
    Miss = 3,
    Hit = 4,
    Killed = 5
};

enum ProcessingMode
{
    Horizontal = 0,
    Vertical = 1,
    Circular_right = 2,
    Circular_left = 3,
    Circular_top = 4,
    Circular_down = 5
};

enum State
{
    Unknown = 0,
    Making_move = 1,
    Waiting_move = 2,
    Winner = 3,
    Loser = 4
};

struct Move
{
    int line;
    int column;
};

typedef struct Move Move;

struct Answer
{
    int result_of_shoot;
    int is_any_live_ship;
};

typedef struct Answer Answer;

struct NetworkContext
{
    int sockfd;
    struct sockaddr_in servaddr;
};

typedef struct NetworkContext NetworkContext;

unsigned short **_create_matrix();

NetworkContext *_network_init(uint16_t port);

bool _is_any_live_ship(unsigned short **matrix);

void _change_around_killed(unsigned short **matrix, int line, int column,
                           int processing_mode);

bool _is_horizontal(unsigned short **matrix, int line, int column);

void _change_after_kill(unsigned short **matrix, int line, int column);

bool _is_killed(unsigned short **matrix, int line, int column);

int _result_of_shot (unsigned short **matrix, int line, int column);

void _set_ship(unsigned short **matrix, int str1, int str2, int column1, int column2);

bool _is_it_possible_to_put_the_ship(unsigned short **matrix, int str1, int str2,
                                     int column1, int column2);

bool _is_valid_move(unsigned short **matrix, int line, int column);

void send_move(NetworkContext *ctx, Move move);

int receive_confirm(NetworkContext *ctx);

Answer receive_answer(NetworkContext *ctx);

Move _receive_move(NetworkContext *ctx);

void send_answer(NetworkContext *ctx, Answer answer);

void _send_username(NetworkContext *ctx, char username[MAXLINE]);

int receive_state(NetworkContext *ctx);

void _receive_username(NetworkContext *ctx, char username[MAXLINE]);

#ifdef __cplusplus
}
#endif

#endif // _GAME_H

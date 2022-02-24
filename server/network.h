#ifndef NETWORK_H
#define NETWORK_H


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAXLINE 1024
#define PORT	 8080

enum CurrentPlayer
{
    First_player = 1,
    Second_player = 2,
    Winner_is_determined = 3
};

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

struct Move
{
    int line;
    int column;
};

typedef struct Move Move ;

struct Answer
{
    int result_of_shoot;
    int is_any_live_ship;
};

typedef struct Answer Answer;

struct Player
{
    char username[MAXLINE];
    struct sockaddr_in cliaddr;
    int len;
};

typedef struct Player Player;

struct NetworkContext
{
    int sockfd;
    struct sockaddr_in servaddr;
};

typedef struct NetworkContext NetworkContext;

NetworkContext *_network_init(uint16_t port);

Player receive_username(NetworkContext *ctx);

Move receive_move(NetworkContext *ctx, Player player);

Answer receive_answer(NetworkContext *ctx, Player player);

void send_state(int state, Player player, NetworkContext *ctx);

void send_username(char username[MAXLINE], NetworkContext *ctx, Player player);

void send_confirm(int res, NetworkContext *ctx, Player player);

void send_move(Move move, NetworkContext *ctx, Player player);

void send_answer(Answer answer, NetworkContext *ctx, Player player);

#endif // NETWORK_H

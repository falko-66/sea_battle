#include "_game.h"

unsigned short **_create_matrix()
{
    unsigned short **matrix = (unsigned short**) malloc(10 * sizeof (unsigned short *));

    for(int  i = 0; i < 10; ++i)
        matrix[i] = (unsigned short *) malloc (10 * sizeof (unsigned short));

    for(int i = 0; i < 10; ++i)
        for(int j = 0; j < 10; ++j)
            matrix[i][j] = Empty;

    return matrix;
}

NetworkContext *_network_init(uint16_t port)
{
    NetworkContext *ctx =  (NetworkContext *)malloc (sizeof (NetworkContext));

    int sock = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sock < 0){
        perror("socket creation failed");
    }

    ctx->sockfd = sock;
    memset(&ctx->servaddr, 0, sizeof(ctx->servaddr));

    ctx->servaddr.sin_family = AF_INET;
    ctx->servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    ctx->servaddr.sin_port = htons(port);

    return ctx;
}

bool _is_any_live_ship(unsigned short **matrix)
{
    for (int i = 0; i < 10; ++i)
        for(int j = 0; j < 10; ++j)
            if(matrix[i][j] == Engaged)
                return true;

    return false;
}

void _change_around_killed(unsigned short **matrix, int line, int column,
                          int processing_mode)
{
    switch (processing_mode) {

    case Horizontal:
        if(column - 1 >= 0)
            if(matrix[line][column - 1] == Empty)
                matrix[line][column - 1] = Neighbor;

        if(column + 1 < 10)
            if(matrix[line][column + 1] == Empty)
                matrix[line][column + 1] = Neighbor;
        break;

    case Vertical:
        if(line - 1 >= 0)
            if(matrix[line - 1][column] == Empty)
                matrix[line - 1][column] = Neighbor;

        if(line + 1 < 10)
            if(matrix[line + 1][column] == Empty)
                matrix[line + 1][column] = Neighbor;
        break;

    case Circular_down:
        if(column - 1 >= 0){
            if(matrix[line][column - 1] == Empty)
                matrix[line][column - 1] = Neighbor;

            if(line + 1 < 10)
                if(matrix[line + 1][column - 1] == Empty)
                    matrix[line + 1][column - 1] = Neighbor;
        }

        if(column + 1 < 10){
            if(matrix[line][column + 1] == Empty)
                matrix[line][column + 1] = Neighbor;

            if(line + 1 < 10)
                if(matrix[line + 1][column + 1] == Empty)
                    matrix[line + 1][column + 1] = Neighbor;
        }

        if(line + 1 < 10)
            if(matrix[line + 1][column] == Empty)
                matrix[line + 1][column] = Neighbor;
        break;

    case Circular_top:
        if(column - 1 >= 0){
            if(matrix[line][column - 1] == Empty)
                matrix[line][column - 1] = Neighbor;

            if(line - 1 >= 0)
                if(matrix[line - 1][column - 1] == Empty)
                    matrix[line - 1][column - 1] = Neighbor;
        }

        if(column + 1 < 10){
            if(matrix[line][column + 1] == Empty)
                matrix[line][column + 1] = Neighbor;

            if(line - 1 >= 0)
                if(matrix[line - 1][column + 1] == Empty)
                    matrix[line - 1][column + 1] = Neighbor;
        }

        if(line - 1 >= 0)
            if(matrix[line - 1][column] == Empty)
                matrix[line - 1][column] = Neighbor;
        break;

    case Circular_left:
        if(line - 1 >= 0){
            if(matrix[line - 1][column] == Empty)
                matrix[line - 1][column] = Neighbor;

            if(column - 1 >= 0)
                if(matrix[line - 1][column - 1] == Empty)
                    matrix[line - 1][column - 1] = Neighbor;
        }

        if(line + 1 < 10){
            if(matrix[line + 1][column] == Empty)
                matrix[line + 1][column] = Neighbor;

            if(column - 1 >= 0)
                if(matrix[line + 1][column - 1] == Empty)
                    matrix[line + 1][column - 1] = Neighbor;
        }

        if(column - 1 >= 0)
            if(matrix[line][column - 1] == Empty)
                matrix[line][column - 1] = Neighbor;
        break;

    case Circular_right:
        if(line - 1 >= 0){
            if(matrix[line - 1][column] == Empty)
                matrix[line - 1][column] = Neighbor;

            if(column + 1 < 10)
                if(matrix[line - 1][column + 1] == Empty)
                    matrix[line - 1][column + 1] = Neighbor;
        }

        if(line + 1 < 10){
            if(matrix[line + 1][column] == Empty)
                matrix[line + 1][column] = Neighbor;

            if(column + 1 < 10)
                if(matrix[line + 1][column + 1] == Empty)
                    matrix[line + 1][column + 1] = Neighbor;
        }

        if(column + 1 < 10)
            if(matrix[line][column + 1] == Empty)
                matrix[line][column + 1] = Neighbor;
        break;
    }
}
bool _is_horizontal(unsigned short **matrix, int line, int column)
{
    if(column - 1 >= 0)
        if(matrix[line][column - 1] == Hit || matrix[line][column - 1] == Killed)
            return true;

    if(column + 1 < 10)
        if(matrix[line][column + 1] == Hit || matrix[line][column + 1] == Killed)
            return true;

    return false;
}

void _change_after_kill(unsigned short **matrix, int line, int column)
{
    if(_is_horizontal (matrix, line, column)){

        _change_around_killed (matrix, line, column, Vertical);

        bool flag_left = true;
        bool flag_right = true;

        if(column - 1 >= 0){
            if (matrix[line][column - 1] == Hit || matrix[line][column - 1] == Killed){
                matrix[line][column - 1] = Killed;
                _change_around_killed (matrix, line, column - 1, Vertical);

            }else{

                flag_left = false;
                _change_around_killed (matrix, line, column, Circular_left);
            }

            if(column - 2 >= 0 && flag_left){
                if(matrix[line][column - 2] == Hit || matrix[line][column - 2] == Killed){
                    matrix[line][column - 2] = Killed;
                    _change_around_killed (matrix, line, column - 2, Vertical);

                }else{

                    flag_left = false;
                    _change_around_killed (matrix, line, column - 1, Circular_left);
                }

                if(column - 3 >= 0 && flag_left){
                    if(matrix[line][column - 3] == Hit|| matrix[line][column - 3] == Killed){
                        matrix[line][column - 3] = Killed;
                        _change_around_killed (matrix, line, column - 3, Circular_left);

                    }else

                        _change_around_killed (matrix, line, column - 2, Circular_left);
                }
            }
        }

        if(column + 1 < 10){

            if(matrix[line][column + 1] == Hit|| matrix[line][column + 1] == Killed){
                matrix[line][column + 1] = Killed;
                _change_around_killed (matrix, line, column + 1, Vertical);

            }else{

                flag_right = false;
                _change_around_killed (matrix, line, column, Circular_right);
            }

            if(column + 2 < 10 && flag_right){
                if(matrix[line][column + 2] == Hit|| matrix[line][column + 2] == Killed){
                    matrix[line][column + 2] = Killed;
                    _change_around_killed (matrix, line, column + 2, Vertical);

                }else{

                    flag_right = false;
                    _change_around_killed (matrix, line, column + 1, Circular_right);
                }

                if(column + 3 < 10 && flag_right){
                    if(matrix[line][column + 3] == Hit|| matrix[line][column + 3] == Killed){
                        matrix[line][column + 3] = Killed;
                        _change_around_killed (matrix, line, column + 3, Circular_right);

                    }else

                        _change_around_killed (matrix, line, column + 2, Circular_right);
                }
            }
        }
    }

    if(!_is_horizontal (matrix, line, column)){

        bool flag_top = true;
        bool flag_down = true;

        _change_around_killed (matrix, line, column, Horizontal);

        if(line - 1 >= 0){
            if(matrix[line - 1][column] == Hit|| matrix[line - 1][column] == Killed){
                matrix[line - 1][column] = Killed;
                _change_around_killed (matrix, line - 1, column, Horizontal);

            }else{

                flag_top = false;
                _change_around_killed (matrix, line, column, Circular_top);
            }

            if(line - 2 >= 0 && flag_top){
                if(matrix[line - 2][column] == Hit|| matrix[line - 2][column] == Killed){
                    matrix[line - 2][column] = Killed;
                    _change_around_killed (matrix, line - 2, column, Horizontal);

                }else{

                    flag_top = false;
                    _change_around_killed (matrix, line - 1, column, Circular_top);
                }

                if(line - 3 >= 0 && flag_top){
                    if(matrix[line - 3][column] == Hit|| matrix[line - 3][column] == Killed){
                        matrix[line - 3][column] = Killed;
                        _change_around_killed (matrix, line - 3, column, Circular_top);

                    }else

                        _change_around_killed (matrix, line - 2, column, Circular_top);
                }
            }
        }
        if(line + 1 < 10){

            if(matrix[line + 1][column] == Hit|| matrix[line + 1][column] == Killed){
                matrix[line + 1][column] = Killed;
                _change_around_killed (matrix, line + 1, column, Horizontal);

            }else{

                _change_around_killed (matrix, line, column, Circular_down);
                flag_down = false;
            }

            if(line + 2 < 10 && flag_down){
                if(matrix[line + 2][column] == Hit|| matrix[line + 2][column] == Killed){
                    matrix[line + 2][column] = Killed;
                    _change_around_killed (matrix, line + 2, column, Horizontal);

                }else{

                    _change_around_killed (matrix, line + 1, column, Circular_down);
                    flag_down = false;
                }

                if(line + 3 < 10 && flag_down){
                    if(matrix[line + 3][column] == Hit|| matrix[line + 3][column] == Killed){
                        matrix[line + 3][column] = Killed;
                        _change_around_killed (matrix, line + 3, column, Circular_down);

                    }else

                        _change_around_killed (matrix, line + 2, column, Circular_down);
                }
            }
        }
    }
}

bool _is_killed(unsigned short **matrix, int line, int column)
{
    bool flag_left = true;
    bool flag_right = true;
    bool flag_top = true;
    bool flag_down = true;

    if(column - 1 >= 0){
        if(matrix[line][column - 1] == Engaged)
            return false;

        else if (matrix[line][column - 1] != Hit)
            flag_left = false;

        if(column - 2 >= 0 && flag_left){
            if(matrix[line][column - 2] == Engaged)
                return false;

            else if(matrix[line][column - 2] != Hit)
                flag_left = false;
        }

        if(column - 3 >= 0 && flag_left){
            if(matrix[line][column - 3] == Engaged)
                return false;
        }
    }

    if(column + 1 < 10){

        if(matrix[line][column + 1] == Engaged)
            return false;

        else if (matrix[line][column + 1] != Hit)
            flag_right = false;

        if(column + 2 < 10 && flag_right){
            if(matrix[line][column + 2] == Engaged)
                return false;

            else if(matrix[line][column + 2] != Hit)
                flag_right = false;

            if(column + 3 < 10 && flag_right){
                if(matrix[line][column + 3] == Engaged)
                    return false;
            }
        }
    }

    if(line - 1 >= 0){

        if(matrix[line - 1][column] == Engaged)
            return false;

        else if (matrix[line - 1][column] != Hit)
            flag_top = false;

        if(line - 2 >= 0 && flag_top){
            if(matrix[line - 2][column] == Engaged)
                return false;

            else if(matrix[line - 2][column] != Hit)
                flag_top = false;

            if(line - 49 - 3 >= 0 && flag_top){
                if(matrix[line - 3][column] == Engaged)
                    return false;
            }
        }
    }

    if(line + 1 < 10){

        if(matrix[line + 1][column] == Engaged)
            return false;

        else if(matrix[line + 1][column] != Hit)
            flag_down = false;

        if(line + 2 < 10 && flag_down){
            if(matrix[line + 2][column] == Engaged)
                return false;

            else if(matrix[line + 2][column] != Hit)
                flag_down = false;

            if(line + 3 < 10 && flag_down){
                if(matrix[line + 3][column] == Engaged)
                    return false;
            }
        }
    }
    return true;

}

int _result_of_shot (unsigned short **matrix, int line, int column)
{
    if(matrix[line][column] == Engaged && _is_killed (matrix, line, column))
        return Killed;

    if(matrix[line][column] == Engaged && !_is_killed (matrix, line, column))
        return Hit;

    if(matrix[line][column] == Empty ||
            matrix[line][column] == Neighbor)
        return Miss;

    return ERROR;
}

void _set_ship(unsigned short **matrix, int str1, int str2, int column1, int column2)
{
    for (int i = str1; i <= str2; ++i){
        for(int j = column1; j <= column2; ++j){

            matrix[i][j] = Engaged;

            if (i - 1 >= 0 && str1 == str2)
                matrix[i - 1][j] = Neighbor;

            if (i + 1 <= 9 && str1 == str2)
                matrix[i + 1][j] = Neighbor;

            if (j - 1 >= 0 && column1 == column2)
                matrix[i][j - 1] = Neighbor;

            if (j + 1 <= 9 && column1 == column2)
                matrix[i][j + 1] = Neighbor;
        }
    }

    if(column1 - 1 >= 0 && str1 == str2){

        matrix[str1][column1 - 1] = Neighbor;

        if(str1 - 1 >= 0)
            matrix[str1 - 1][column1 - 1] = Neighbor;

        if(str1 + 1 <= 9 )
            matrix[str1 + 1][column1 - 1] = Neighbor;
    }

    if(column2 + 1 <= 9 && str1 == str2){

        matrix[str1][column2 + 1] = Neighbor;

        if(str1 - 1 >= 0)
            matrix[str1 - 1][column2 + 1] = Neighbor;

        if(str1 + 1 <= 9 )
            matrix[str1 + 1][column2 + 1] = Neighbor;
    }

    if(str1 - 1 >= 0 && column1 == column2){

        matrix[str1 - 1][column1] = Neighbor;

        if(column1 - 1 >= 0)
            matrix[str1 - 1][column1 - 1] = Neighbor;

        if(column1 + 1 <= 9 )
            matrix[str1 - 1][column1 + 1] = Neighbor;
    }

    if(str2 + 1 <= 9 && column1 == column2){

        matrix[str2 + 1][column1] = Neighbor;

        if(column1 - 1 >= 0)
            matrix[str2 + 1][column1 - 1] = Neighbor;

        if(column1 + 1 <= 9)
            matrix[str2 + 1][column1 + 1] = Neighbor;
    }
}


bool _is_it_possible_to_put_the_ship(unsigned short **matrix, int str1, int str2,
                                     int column1, int column2)
{
    if (str1 > 9 || str1 < 0 || str2 > 9 || str2 < 0 || str1 > str2)
        return false;

    if (column1 > 9 || column1 < 0 || column2 > 9 || column2 < 0 || column1 > column2)
        return false;

    if(str1 != str2 && column1 != column2)
        return false;

    for (int i = str1; i <= str2; ++i)
        for(int j = column1; j <= column2; ++j)
            if (matrix[i][j] != Empty)
                return false;

    return true;
}

bool _is_valid_move(unsigned short **matrix, int line, int column)
{
    if(matrix[line][column] != Empty)
        return false;

    return true;
}

void send_move(NetworkContext *ctx, Move move)
{
    sendto(ctx->sockfd,
           &move,
           sizeof (Move),
           0,
           (const struct sockaddr *) &ctx->servaddr,
           sizeof(ctx->servaddr));
}

void send_answer(NetworkContext *ctx, Answer answer)
{
    sendto(ctx->sockfd,
           &answer,
           sizeof (Answer),
           0,
           (const struct sockaddr *) &ctx->servaddr,
           sizeof(ctx->servaddr));
}

void _send_username(NetworkContext *ctx, char username[])
{
    sendto(ctx->sockfd,
           (const char *) username,
           strlen(username),
           0,
           (const struct sockaddr *) &ctx->servaddr,
           sizeof(ctx->servaddr));
}

int receive_confirm(NetworkContext *ctx)
{
    int res = 0;
    int len = sizeof (ctx->servaddr);

    recvfrom (ctx->sockfd,
              &res,
              sizeof (Answer),
              MSG_WAITALL,
              (struct sockaddr *) &ctx->servaddr,
              (socklen_t *)&len);

    return res;
}

Answer receive_answer(NetworkContext *ctx)
{
    Answer answer;
    int len = sizeof (ctx->servaddr);

    recvfrom (ctx->sockfd,
              &answer,
              sizeof (Answer),
              MSG_WAITALL,
              (struct sockaddr *) &ctx->servaddr,
              (socklen_t *)&len);

    return answer;
}

Move _receive_move(NetworkContext *ctx)
{
    Move move;
    int len = sizeof (ctx->servaddr);

    int result = recvfrom (ctx->sockfd,
                           &move,
                           sizeof (Move),
                           MSG_DONTWAIT,
                           (struct sockaddr *) &ctx->servaddr,
                           (socklen_t *)&len);

    if(result < 0){
        move.line = -1;
        move.column = -1;
    }

    return move;
}

int receive_state(NetworkContext *ctx)
{
    int state;
    int len = sizeof (ctx->servaddr);

    recvfrom (ctx->sockfd,
              &state,
              sizeof (int),
              MSG_WAITALL,
              (struct sockaddr *) &ctx->servaddr,
              (socklen_t *)&len);
    return state;
}

 void _receive_username(NetworkContext *ctx, char username[MAXLINE])
{
    int len = sizeof (ctx->servaddr);

    int res  = recvfrom (ctx->sockfd,
              (char *)username,
              MAXLINE,
              MSG_DONTWAIT,
              (struct sockaddr *) &ctx->servaddr,
              (socklen_t *)&len);

    if(res < 0)
        strcpy (username, "");
}

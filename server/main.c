#include<network.h>

int main()
{
    NetworkContext *network_context = _network_init (PORT);

    if(!network_context){
        printf ("Failed to initialize server\n");
        return -1;
    }

    Player player1 = receive_username (network_context);

    printf ("Adding the first player with username: %s\n", player1.username);

    int current_player = First_player;

    send_state (current_player, player1, network_context);

    Player player2 = receive_username(network_context);

    printf ("Adding the second player with username: %s\n", player2.username);

    send_username (player2.username, network_context, player1);

    current_player = Second_player;

    send_state (current_player, player2, network_context);

    send_username (player1.username, network_context, player2);

    current_player = First_player;

    Move move;
    Answer answer;

    for(;;){
        switch (current_player){

        case First_player:

            while(1){
                move = receive_move (network_context, player1);

                int res;

                if(move.line >= 0 && move.column >= 0 && move.line < 10 && move.column < 10)
                    res = 1;
                else res = -1;

                send_confirm(res, network_context, player1);

                if (res > 0)
                    break;
            }

            printf ("%s: %d:%d\n", player1.username, move.line, move.column);

            send_move (move, network_context, player2);

            answer = receive_answer (network_context, player2);
            send_answer(answer, network_context, player1);

            printf ("%s: result ", player2.username);

            switch (answer.result_of_shoot) {

            case Miss:
                printf ("Miss\n");
                break;

            case Hit:
                printf ("Hit\n");
                break;

            case Killed:
                printf ("Killed\n");
                break;

            default:
                printf ("%d\n", answer.result_of_shoot);
                break;
            }

            if (answer.result_of_shoot != Hit && answer.result_of_shoot != Killed){
                printf ("\nMove goes to %s\n\n", player2.username);
                current_player = Second_player;
            }

            if(answer.result_of_shoot == Killed && answer.is_any_live_ship == 0)
                current_player = Winner_is_determined;

            break;


        case Second_player:

            while(1){
                move = receive_move (network_context, player2);

                int res;

                if(move.line >= 0 && move.column >= 0 && move.line < 10 && move.column < 10)
                    res = 1;
                else res = -1;

                send_confirm (res, network_context, player2);

                if (res > 0)
                    break;
            }
            printf ("%s: %d:%c\n", player2.username, move.line, move.column + 65);

            send_move (move, network_context, player1);

            answer = receive_answer (network_context, player1);
            send_answer (answer, network_context, player2);

            printf ("%s: result ", player1.username);

            switch (answer.result_of_shoot) {

            case Miss:
                printf ("Miss\n");
                break;

            case Hit:
                printf ("Hit\n");
                break;

            case Killed:
                printf ("Killed\n");
                break;

            default:
                printf ("%d\n", answer.result_of_shoot);
                break;
            }

            if (answer.result_of_shoot != Hit && answer.result_of_shoot != Killed){
                current_player = First_player;
                printf ("\nMove goes to %s\n\n", player1.username);
            }

            if(answer.result_of_shoot == Killed && answer.is_any_live_ship == 0)
                current_player = Winner_is_determined;

            break;

        }

        if(current_player == Winner_is_determined)
            break;

    }

    close (network_context->sockfd);
    free(network_context);
    return 0;
}


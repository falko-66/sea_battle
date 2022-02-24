#include "network.h"
NetworkContext *_network_init(uint16_t port)
{

    NetworkContext *ctx = malloc (sizeof (NetworkContext));
    if(!ctx)
        goto error1;

    int sock = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(sock < 0){
        perror("socket creation failed");
        goto error2;
    }

    ctx->sockfd = sock;
    memset(&ctx->servaddr, 0, sizeof(ctx->servaddr));
    ctx->servaddr.sin_family = AF_INET; // IPv4
    ctx->servaddr.sin_addr.s_addr = INADDR_ANY;
    ctx->servaddr.sin_port = htons(port);

    int bind_status = bind (ctx->sockfd,
                            (const struct sockaddr *) &ctx->servaddr,
                            sizeof(ctx->servaddr));

    if(bind_status < 0){
        perror("bind failed");
        goto error3;
    }

    return ctx;

error3:
    close (ctx->sockfd);

error2:
    free(ctx);

error1:
    return NULL;
}


Player receive_username(NetworkContext *ctx)
{
    Player player;
    memset(&player.cliaddr, 0, sizeof(player.cliaddr));
    player.len = sizeof (player.cliaddr);

    recvfrom (ctx->sockfd,
              (char *)player.username,
              MAXLINE,
              MSG_WAITALL,
              (struct sockaddr *)&player.cliaddr,
              (socklen_t *)&player.len);
    return player;
}

Move receive_move(NetworkContext *ctx, Player player)
{
    Move move;

    recvfrom (ctx->sockfd,
              &move,
              sizeof (Move),
              0,
              (struct sockaddr *)&player.cliaddr,
              (socklen_t *)&player.len);

    return move;
}

Answer receive_answer(NetworkContext *ctx, Player player)
{
    Answer answer;

    recvfrom (ctx->sockfd,
              &answer,
              sizeof (Answer),
              0,
              (struct sockaddr *)&player.cliaddr,
              (socklen_t *)&player.len);

    return answer;
}

void send_state(int state, Player player, NetworkContext *ctx)
{
    sendto (ctx->sockfd,
            &state,
            sizeof (int),
            MSG_WAITALL,
            (const struct sockaddr *)&player.cliaddr,
            player.len);
}

void send_username(char username[MAXLINE], NetworkContext *ctx, Player player)
{
    sendto (ctx->sockfd,
            (const char *) username,
            strlen (username),
            MSG_WAITALL,
            (const struct sockaddr *)&player.cliaddr,
            player.len);
}

void send_confirm(int res, NetworkContext *ctx, Player player)
{
    sendto (ctx->sockfd,
            &res,
            sizeof (int),
            0,
            (const struct sockaddr *)&player.cliaddr,
            player.len);
}

void send_move(Move move, NetworkContext *ctx, Player player)
{
    sendto (ctx->sockfd,
            &move,
            sizeof (Move),
            0,
            (const struct sockaddr *)&player.cliaddr,
            player.len);
}

void send_answer(Answer answer, NetworkContext *ctx, Player player)
{
    sendto (ctx->sockfd,
            &answer,
            sizeof (Answer),
            0,
            (const struct sockaddr *)&player.cliaddr,
            player.len);
}

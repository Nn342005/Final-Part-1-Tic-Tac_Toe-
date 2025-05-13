#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "game.h"

#define BROKER_IP "34.9.239.30"
#define TOPIC_BOARD "tictactoe/board"
#define TOPIC_MOVE_X "tictactoe/moveX"
#define TOPIC_MOVE_O "tictactoe/moveO"

// ----- MQTT Functions -----

void publish_board() {
    char board_str[10] = "";
    for (int i = 0; i < 9; i++) {
        board_str[i] = (board[i] == ' ') ? '_' : board[i];
    }
    board_str[9] = '\0';

    printf("DEBUG: Sending board string: [%s]\n", board_str);

    char cmd[512];
    snprintf(cmd, sizeof(cmd),
             "mosquitto_pub -h %s -t %s -r -m \"%s\"",
             BROKER_IP, TOPIC_BOARD, board_str);
    printf("DEBUG CMD: %s\n", cmd);
    system(cmd);
}

int receive_move(const char *topic) {
    FILE *fp;
    char path[1035];
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "mosquitto_sub -h %s -t %s -C 1", BROKER_IP, topic);
    fp = popen(cmd, "r");
    if (fp == NULL) return -1;
    if (fgets(path, sizeof(path)-1, fp) != NULL) {
        int move = atoi(path);
        pclose(fp);
        return move;
    }
    pclose(fp);
    return -1;
}

// ----- Option 1: Player vs GCP Bot -----

void play_with_bot() {
    reset_board();
    print_board();

    while (1) {
        int move;
        printf("Your move (1-9): ");
        scanf("%d", &move);
        move -= 1;

        if (!is_valid_move(move)) {
            printf("Invalid move.\n");
            continue;
        }

        make_move(move, PLAYER_X);
        print_board();

        if (is_winner(PLAYER_X)) {
            printf("You win!\n");
            break;
        }
        if (is_draw()) {
            printf("It's a draw!\n");
            break;
        }

        publish_board();
        printf("Waiting for bot move...\n");

        int bot_move = receive_move(TOPIC_MOVE_O);
        if (!is_valid_move(bot_move)) {
            printf("Bot made invalid move!\n");
            break;
        }

        make_move(bot_move, PLAYER_O);
        print_board();

        if (is_winner(PLAYER_O)) {
            printf("Bot wins!\n");
            break;
        }
        if (is_draw()) {
            printf("It's a draw!\n");
            break;
        }
    }
}

// ----- Option 2: Local 2 Players -----

void play_two_players() {
    reset_board();
    print_board();

    char current = PLAYER_X;
    int move;

    while (1) {
        printf("Player %c, enter your move (1-9): ", current);
        scanf("%d", &move);
        move -= 1;

        if (!is_valid_move(move)) {
            printf("Invalid move. Try again.\n");
            continue;
        }

        make_move(move, current);
        print_board();

        if (is_winner(current)) {
            printf("Player %c wins!\n", current);
            break;
        }

        if (is_draw()) {
            printf("It's a draw!\n");
            break;
        }

        current = (current == PLAYER_X) ? PLAYER_O : PLAYER_X;
    }
}

// ----- Option 3: C Bot vs Bash Bot -----

void auto_play() {
    reset_board();       
    publish_board();     
    print_board();

    while (1) {
        // === C Bot X ===
        system("ssh nguyennam342005@34.9.239.30 './botX'");
        int moveX = receive_move(TOPIC_MOVE_X);
        if (!is_valid_move(moveX)) {
            printf("C Bot (X) made invalid move\n");
            break;
        }

        make_move(moveX, PLAYER_X);
        printf("AFTER make_move: board[0] = %c, board[1] = %c, board[2] = %c\n", board[0], board[1], board[2]);
        print_board();

        publish_board();

        if (is_winner(PLAYER_X)) {
            printf("C Bot (X) wins!\n");
            break;
        }
        if (is_draw()) {
            printf("It's a draw!\n");
            break;
        }

        // === Bash Bot O ===
        system("ssh nguyennam342005@34.9.239.30 'bash ./playerO.sh'");
        int moveO = receive_move(TOPIC_MOVE_O);
        if (!is_valid_move(moveO)) {
            printf("Bash Bot (O) made invalid move\n");
            break;
        }

        make_move(moveO, PLAYER_O);
        print_board();

        if (is_winner(PLAYER_O)) {
            printf("Bash Bot (O) wins!\n");
            break;
        }
        if (is_draw()) {
            printf("It's a draw!\n");
            break;
        }
    }
}

// ----- Main Menu -----

int main() {
    int choice;

    while (1) {
        printf("\n==== TIC TAC TOE MENU ====\n");
        printf("1. Play vs GCP Bot (Player X)\n");
        printf("2. Play 2 Players (local)\n");
        printf("3. Auto Play (C Bot vs Bash Bot)\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                play_with_bot();
                break;
            case 2:
                play_two_players();
                break;
            case 3:
                auto_play();
                break;
            case 4:
                printf("Bye!\n");
                return 0;
            default:
                printf("Invalid input.\n");
        }
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BROKER "34.9.239.30"
#define BOARD_TOPIC "tictactoe/board"
#define MOVE_TOPIC "tictactoe/moveX"

int main() {
    // Step 1: Get current board
    FILE *fp;
    char cmd[256];
    char board[64];

    snprintf(cmd, sizeof(cmd),
             "mosquitto_sub -h %s -t %s -C 1", BROKER, BOARD_TOPIC);
    fp = popen(cmd, "r");
    if (fp == NULL) {
        perror("Failed to get board");
        return 1;
    }

    fgets(board, sizeof(board), fp);
    pclose(fp);

    // Step 2: Clean the board string
    char clean[10];
    int j = 0;
    for (int i = 0; board[i] != '\0' && j < 9; i++) {
        if (board[i] == 'X' || board[i] == 'O' || board[i] == '_') {
            clean[j++] = board[i];
        }
    }
    clean[j] = '\0';

    if (strlen(clean) != 9) {
        printf("Invalid board length: %s\n", clean);
        return 1;
    }

    // Step 3: Pick random empty position
    int empty[9];
    int count = 0;
    for (int i = 0; i < 9; i++) {
        if (clean[i] == '_') {
            empty[count++] = i;
        }
    }

    if (count == 0) {
        printf("No empty spots. Game might be over.\n");
        return 1;
    }

    srand(time(NULL));
    int choice = empty[rand() % count];

    // Step 4: Publish move
    snprintf(cmd, sizeof(cmd),
             "mosquitto_pub -h %s -t %s -m \"%d\"",
             BROKER, MOVE_TOPIC, choice);
    system(cmd);

    printf("Bot X played at position %d\n", choice);
    return 0;
}
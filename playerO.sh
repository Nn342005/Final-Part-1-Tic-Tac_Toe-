#!/bin/bash

BROKER="34.9.239.30"
TOPIC="tictactoe/board"
MOVE_TOPIC="tictactoe/moveO"

# Get the latest board
RAW_BOARD=$(mosquitto_sub -h "$BROKER" -t "$TOPIC" -C 1)
CLEAN_BOARD=$(echo "$RAW_BOARD" | tr -d '\n\r' | tr -cd 'XO_')

echo "Received board: '$CLEAN_BOARD'"

if [ "${#CLEAN_BOARD}" -ne 9 ]; then
    echo "Invalid board string length."
    exit 1
fi

# Pick first empty spot
for i in $(seq 0 8); do
    CELL="${CLEAN_BOARD:$i:1}"
    if [[ "$CELL" == "_" ]]; then
        echo "Bot O chooses position $i"
        mosquitto_pub -h "$BROKER" -t "$MOVE_TOPIC" -m "$i"
        exit 0
    fi
done

echo "No empty cell. Game might be over."
exit 1
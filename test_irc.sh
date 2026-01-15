#!/bin/bash

HOST=${1:-localhost}
PORT=${2:-4242}

{
    printf "PA"
    sleep 0.3
    printf "SS ale"
    sleep 0.3
    printf "fur\r\n"
    sleep 0.5

    printf "NI"
    sleep 0.3
    printf "CK da"
    sleep 0.3
    printf "vid\r\n"
    sleep 0.5

    printf "US"
    sleep 0.3
    printf "ER david "
    sleep 0.3
    printf "* 0 :Da"
    sleep 0.3
    printf "vid\r\n"
} | nc -C "$HOST" "$PORT"

#!/bin/bash

PORT=4242
PASS=alefur
IP=127.0.0.1

tmux new-session -d -s irc

# Serveur
tmux send-keys -t irc "valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./ircserv $PORT $PASS" C-m

sleep 1

# Client 1 - bob
tmux split-window -h
tmux send-keys "nc -C -N localhost $PORT" C-m
sleep 0.2
tmux send-keys "PASS alefur" C-m
tmux send-keys "NICK bob" C-m
tmux send-keys "USER bob 0 * :Bob" C-m

# Client 2 - alice
tmux split-window -v
tmux send-keys "telnet localhost $PORT" C-m
sleep 0.2
tmux send-keys "PASS alefur" C-m
tmux send-keys "NICK alice" C-m
tmux send-keys "USER alice 0 * :Alice" C-m

# Client 3 - local user
tmux select-pane -t 0
tmux split-window -v
tmux send-keys "irssi -c $IP -p $PORT -w $PASS" C-m

tmux attach -t irc
localhost

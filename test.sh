#!/bin/bash

export LD_LIBRARY_PATH=./mjpg-streamer/

./mjpg-streamer/mjpg_streamer -o "output_http.so -p 8080 -w ./www" -i "input_control.so"

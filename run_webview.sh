#!/bin/bash

# Start the xvfb-run process with cefclient in the background
xvfb-run --server-args="-screen 0 1920x1080x24" ./cmake-build-debug/tests/cefclient/Debug/cefclient --off-screen-rendering-enabled --url=http://192.168.100.6:3000/websiteview &

# Define the loop interval (in seconds)
INTERVAL=5

while true; do
  # Upload the image using curl
  curl -X POST -F "file=@./img.png" http://192.168.100.6:8080/image/upload
  # Wait for the defined interval before next iteration
  sleep $INTERVAL
done
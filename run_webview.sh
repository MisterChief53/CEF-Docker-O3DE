#!/bin/bash

# Start the xvfb-run process with cefclient in the background
xvfb-run --server-args="-screen 0 1024x768x24" ./cmake-build-debug/tests/cefclient/Debug/cefclient --off-screen-rendering-enabled --url=http://<accounts-server-public-ip-here>:3000/websiteview &

# Define the loop interval (in seconds)
INTERVAL=5

while true; do
  # Upload the image using curl
  curl -X POST -F "file=@./img.png" http://<accounts-server-public-ip>:8080/image/upload
  # Wait for the defined interval before next iteration
  sleep $INTERVAL
done
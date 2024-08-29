#!/bin/bash

# Start the xvfb-run process with cefclient in the background
#xvfb-run --server-args="-screen 0 1920x1080x24" /workspace/build/tests/cefclient/Release/cefclient --off-screen-rendering-enabled --url=http://10.103.160.60:3000/websiteview &

xvfb-run --server-args="-screen 0 1920x1080x24" /workspace/build/tests/cefclient/Release/cefclient --off-screen-rendering-enabled --url=http://10.103.160.60/websiteview --no-sandbox &


# Define the loop interval (in seconds)
INTERVAL=5

while true; do
  # Upload the image using curl
  curl -X POST -F "file=@./img.png" http://10.103.160.60:8080/image/upload
  # Wait for the defined interval before next iteration
  sleep $INTERVAL
done

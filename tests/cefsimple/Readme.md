**Metapulse Web View Module**

Small application build with the Chromium Embedded Framework that connects to a webpage, builds a pixel buffer out of the webpage's contents, and sends it somewhere else for rendering.

Right now, it is able to connect to a webpage and extract the pixel buffer. The plan is to use ZeroMQ to send the buffer to an O3DE multiplayer server so that the clients can render it.

Current CEF Binary:

cef_binary_118.6.9+g7e73645+chromium-118.0.5993.119_linux64.tar.bz2

For easy development and collaboration, this should take place of a complete CEF binary release's `./tests/cefsimple` folder.

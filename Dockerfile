FROM debian

LABEL description="Container for working with CEF"


# Some general build dependencies
RUN apt-get update && apt-get install -y gcc rsync zip openssh-server make clang clangd

RUN apt-get install -y build-essential g++ cmake doxygen libx11-dev gdb

RUN apt-get install -y xorg xvfb x11-apps x11-utils dbus libnss3 libnss3 libnspr4 libatk1.0-0 libatk-bridge2.0 libcups2 libdrm2 libxcomposite1 libxdamage1 libxfixes3 libxrandr2 libgbm1 libxkbcommon0 libpango-1.0-0 libcairo2 libasound2 libatspi2.0 libgtk2.0-dev libgtk-3-dev

# For ssh communication with Visual Studio
RUN mkdir -p /var/run/sshd

RUN echo 'PasswordAuthentication yes' >> /etc/ssh/sshd_config && \
	ssh-keygen -A

# Expose port 22
EXPOSE 22

# WORKDIR /source

# start ssh server and setup a user
RUN service ssh start && \
	useradd -m -d /home/dev -s /bin/bash -G sudo dev && \
	echo 'dev:1234' | chpasswd

RUN usermod -aG audio,video dev

# RUN mkdir -p /home/dev/source

ENV DISPLAY=:0

# CMD ["/usr/sbin/sshd", "-D"]



# Start D-Bus and Xvfb
# CMD service dbus start && /usr/sbin/sshd -D && Xvfb :99 -screen 0 1024x768x16 -ac
# CMD service dbus start && /usr/sbin/sshd && Xvfb :99 -screen 0 1024x768x16 -ac
# CMD su - dev -c 'Xvfb :0 -screen 0 1024x768x16 -ac' & service dbus start && xdpyinfo && /usr/sbin/sshd -D
CMD su - dev -c 'Xvfb :0 -screen 0 1024x768x16 -ac' & service dbus start && /usr/sbin/sshd -D
# CMD Xvfb :0 -screen 0 1024x768x16 -ac && sleep 2 && service dbus start && /usr/sbin/sshd -D && xeyes



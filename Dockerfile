FROM ubuntu:focal

WORKDIR /
RUN apt-get update
RUN apt-get install build-essential -y
RUN apt-get install wget -y
RUN apt-get install libsfml-dev -y
RUN wget -qO- "https://cmake.org/files/v3.28/cmake-3.28.3-linux-x86_64.tar.gz" | tar --strip-components=1 -xz -C /usr/local

WORKDIR /home
COPY . /home
RUN cmake .
RUN cmake --build .

ENTRYPOINT ./gom_test

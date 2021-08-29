FROM ubuntu:18.04 AS builder

RUN apt-get -y update
RUN apt-get -y upgrade
RUN apt install -y g++ cmake pkg-config
RUN apt-get install -y libgstreamer1.0-0 \ 
        gstreamer1.0-plugins-base gstreamer1.0-plugins-good \
        gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly \
        gstreamer1.0-libav gstreamer1.0-doc gstreamer1.0-tools \
        gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl \
        gstreamer1.0-gtk3 gstreamer1.0-qt5

ADD . /opt/sources
WORKDIR /opt/sources
RUN mkdir build && \
    cd build && \
    cmake -D CMAKE_BUILD_TYPE=Release .. && \
    make

ENTRYPOINT ["/opt/sources/build/gstreamer_01"]


FROM ubuntu:14.04

ENV GRGSM_GIT=https://github.com/ptrkrysik/gr-gsm
ENV OSMOCORE_GIT=git://git.osmocom.org/libosmocore.git
ENV GR_OSMOSDR_GIT=git://git.osmocom.org/gr-osmosdr

# We know it's going to foul up, so we run this install separately and then ...fix... dpkg status
RUN apt-get update && apt-get install -y uhd-host || \
     sed -i "s/Status: install ok half-configured/Status: install ok installed/g" /var/lib/dpkg/status

RUN apt-get update && apt-get install -y \
    git \
    vim \
    gnuradio-dev \
    gr-osmosdr \
    libgnuradio-osmosdr0.0.0 \
    libosmosdr-dev \
    libosmosdr0 \
    osmo-sdr \
    cmake \
    autoconf \
    libtool \
    python-talloc-dev \
    libtalloc2 \
    libtalloc-dev \
    libpcsclite-dev \
    build-essential \
    libboost-all-dev \
    libcppunit-dev swig \
    doxygen \
    liblog4cpp5-dev \
    python-scipy \
    librtlsdr-dev \
    librtlsdr0 \
    rtl-sdr \
    wget


RUN /usr/bin/git clone $OSMOCORE_GIT && \
  cd libosmocore && \
  autoreconf -i && \
  ./configure && \
  make && \
  make install && \
  ldconfig -i

#RUN /usr/bin/git clone $GR_OSMOSDR_GIT && \
#  cd gr-osmosdr && \
#  mkdir build && \
#  cd build && \
#  cmake .. && \
#  make && \
#  make install && \
#  ldconfig

#RUN /usr/bin/git clone $GRGSM_GIT && \
#  cd gr-gsm && \
#  mkdir build && \
#  cd build && \
#  cmake .. && \
#  make && \
#  sudo make install &&\
#  ldconfig

COPY ./ /src/
RUN ls /src
RUN mkdir /src/build && \
    cd /src/build && \
    cmake .. && \
    make && \
    make install && \
    ldconfig

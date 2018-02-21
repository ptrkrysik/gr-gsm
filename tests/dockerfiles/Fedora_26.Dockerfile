FROM fedora:26

RUN dnf install -y \
        gcc-c++ \
        make \
        cmake \
        pkgconfig \
        boost-devel \
        gnuradio-devel \
        libosmocore-devel \
        gr-osmosdr \
        swig \
        doxygen \
        python2-docutils \
        cppunit-devel

COPY ./ /src/

WORKDIR /src/build
RUN cmake .. && \
        # The parallel build sometimes fails when the .grc_gnuradio
        # and .gnuradio directories do not exist
        mkdir $HOME/.grc_gnuradio/ $HOME/.gnuradio/ && \
        make -j $(nproc) && \
        make install && \
        ldconfig && \
        make test

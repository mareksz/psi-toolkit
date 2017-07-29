FROM ubuntu:16.04

WORKDIR /
ADD psi-toolkit-data.tar.gz .
RUN apt-get update && \
    apt-get -y install \
        bison \
        cmake \
        flex \
        gcc \
        g++ \
        git \
        libboost-all-dev \
        libcmph-dev \
        libmagic-dev \
        libpcre3-dev \
        libz-dev \
        make \
        antiword \
        aspell-en \
        aspell-pl \
        libaspell-dev \
        libdjvulibre-dev \
        libgraphviz-dev \
        libgtk2.0-dev \
        liblog4cpp5-dev \
        libperl-dev \
        libpoppler-dev \
        libpoppler-glib-dev \
        python-all-dev \
        swig

ADD openfst-1.6.2.tar.gz /openfst
WORKDIR /openfst/openfst-1.6.2
RUN ./configure --enable-bin --enable-far --enable-pdt --enable-mpdt && \
    make && \
    make install

ADD thrax-1.2.3.tar.gz /thrax
WORKDIR /thrax/thrax-1.2.3
RUN ./configure && \
    make && \
    make install

WORKDIR /
RUN git clone https://github.com/skorzewski/psi-toolkit.git

WORKDIR /psi-toolkit
RUN mkdir build

WORKDIR /psi-toolkit/build
RUN cmake -DUSE_JAVA=OFF -DDOWNLOAD_DATA=OFF .. && \
    make && \
    ./tests/test_runner

WORKDIR /psi-toolkit-data
RUN make

WORKDIR /psi-toolkit/build
RUN cmake -DUSE_JAVA=OFF -DDOWNLOAD_DATA=ON .. && \
    make && \
    ./tests/test_runner
EXPOSE 3000
ENTRYPOINT ["./psi-pipe-or-server.sh"]

FROM ubuntu:16.04
WORKDIR /
RUN apt-get update
RUN apt-get -y install \
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
        make
RUN git clone https://github.com/skorzewski/psi-toolkit.git
RUN git clone https://github.com/skorzewski/psi-toolkit-data.git
RUN cp psi-toolkit-data/tools/lemmatizers/lamerlemma/data/pl.bin psi-toolkit/tools/lemmatizers/lamerlemma/data/pl.bin
WORKDIR /psi-toolkit
RUN mkdir build
WORKDIR /psi-toolkit/build
RUN cmake -DUSE_JAVA=OFF -DDOWNLOAD_DATA=OFF ..
RUN make
RUN apt-get -y install tree
RUN ./tests/test_runner
ENTRYPOINT ["./framework/psi-pipe"]

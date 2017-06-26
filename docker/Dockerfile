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
RUN apt-get -y install \
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
RUN git clone https://github.com/skorzewski/psi-toolkit.git
RUN git clone https://github.com/skorzewski/psi-toolkit-data.git
RUN cp psi-toolkit-data/tools/lemmatizers/lamerlemma/data/pl.bin psi-toolkit/tools/lemmatizers/lamerlemma/data/pl.bin
WORKDIR /psi-toolkit
RUN mkdir build
WORKDIR /psi-toolkit/build
RUN cmake -DUSE_JAVA=OFF -DDOWNLOAD_DATA=OFF ..
RUN make
RUN ./tests/test_runner
RUN ./framework/psi-pipe --version
EXPOSE 3000
ENTRYPOINT ["./psi-pipe-or-server.sh"]

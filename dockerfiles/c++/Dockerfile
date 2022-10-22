FROM ubuntu:22.04

RUN apt-get update \
  && apt-get install -y git \
                        g++ \
                        make \
                        wget

RUN cd /root && wget http://downloads.sourceforge.net/project/boost/boost/1.80.0/boost_1_80_0.tar.gz \
  && tar xfz boost_1_80_0.tar.gz \
  && rm boost_1_80_0.tar.gz \
  && cd boost_1_80_0 

RUN cd /root && git clone https://github.com/doctest/doctest





  
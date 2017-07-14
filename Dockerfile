FROM al42and/debian-opencv2

RUN apt-get update
RUN apt-get install -y python-dev pkg-config ffmpeg python-numpy libboost-all-dev git scons python-pip
RUN pip install --upgrade pip
RUN pip install sphinx
WORKDIR /boost
RUN git clone https://github.com/boostorg/python.git
RUN cd ./python && scons config
RUN cd ./python && scons
RUN mkdir /vsf
ADD ./src /vsf/src
ADD setup.py /vsf/setup.py
WORKDIR /vsf

RUN python setup.py build

FROM al42and/debian-opencv2

RUN apt-get update
RUN apt-get install -y python-dev python-pip pkg-config libfreetype6-dev libpng-dev libxml2-dev libxslt1-dev ffmpeg python-numpy libboost-python-dev libboost-all-dev
RUN pip install --upgrade pip

ADD ./src /src
ADD setup.py setup.py

RUN python setup.py clean build install

RUN mkdir /vsf
WORKDIR /vsf
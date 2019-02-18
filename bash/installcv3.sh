#!/bin/bash
OPENCV_VERSION='3.2.0'

wget https://github.com/opencv/opencv/archive/${OPENCV_VERSION}.zip
unzip ${OPENCV_VERSION}.zip
rm ${OPENCV_VERSION}.zip
mv opencv-${OPENCV_VERSION} OpenCV
cd OpenCV
mkdir build
cd build
cmake -D WITH_QR=ON -D ENABLE_PRECOMPILED_HEADERS=OFF
make -j1
sudo make install
sudo ldconfig

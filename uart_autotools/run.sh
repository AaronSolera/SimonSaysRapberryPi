#!/bin/bash
. /opt/poky/2.5.1/environment-setup-cortexa7hf-neon-vfpv4-poky-linux-gnueabi
autoreconf --install
cd build
../configure --prefix="/home/olbapd/Documents/Embebidos/Proyecto3/SimonSaysRapberryPi/uart_autotools/build/usr" --host=arm-poky-linux-gnueabi
make 
make install
cd usr && ls
cd ../
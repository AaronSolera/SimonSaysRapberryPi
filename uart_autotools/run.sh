#!/bin/bash
. /opt/poky/2.5.1/environment-setup-cortexa7hf-neon-vfpv4-poky-linux-gnueabi
autoreconf --install

../configure --prefix="/home/<Su usuario de linux>/<Ruta de la carpeta usr>" --host=arm-poky-linux-gnueabi
cd build
make 
make install
cd usr && ls
cd ../
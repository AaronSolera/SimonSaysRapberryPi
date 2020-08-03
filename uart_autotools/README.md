# Taller 10 
Estudiantes: 
- Pablo Garcia
- Marco Fernandez
- Aaron Solera

En la carpeta del proyecto ejecutar los siguientes pasos:

```
. /opt/poky/2.5.1/environment-setup-cortexa7hf-neon-vfpv4-poky-linux-gnueabi
autoreconf --install
mkdir build
cd build
mkdir usr
../configure --prefix="/home/<Su usuario de linux>/<Ruta de la carpeta usr>" --host=arm-poky-linux-gnueabi
make
make install
cd usr
```

Luego copiar los archivos dentro de la carpeta usr a la SD a utilizar en la raspberry pi.
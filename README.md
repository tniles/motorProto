# Motor Serial Comms Prototype with Qt6 GUI
This project enables prototyping exploration of Qt and Arduino to the end of 
controlling motors from a PC.

On a PC, the Qt GUI provides connectivity and commands to communicate to an 
Arduino (Redboard/Uno) over USB-serial and to provide basic commands for the 
Arduino to process.

This software is open source; see the file COPYING for licensing.

## Docker
Beginning with a successful docker system install, you can use the included dockerfile as follows. 

### Build the container
sudo docker build -t "moto" .

### Verify newly built image is recognized
sudo docker images

### Share your X server!
xhost +local:docker

### Run the image
sudo docker run -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix:ro moto


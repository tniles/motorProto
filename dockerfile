# docker container for the motorProto app

# Prescribe base image for container 
FROM ubuntu:latest

# Provide necessary installs and dependencies for container
RUN apt update && apt install -y build-essential qt6-base-dev qt6-serialport-dev

# Include source code into container ("/")
COPY motorProto.pro /motorProto.pro
COPY motorGui* /
COPY receiver* /
COPY sender* /
COPY main* /

# Run all commands to prep binary
RUN qmake6
RUN make all

# Detail to the container how to run the app for execution
CMD ["/motorProto"]


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

# Run all commands to prep containerized binary
RUN qmake6
RUN make all
ENV XDG_RUNTIME_DIR=/tmp/xdg-runtime-root
RUN mkdir -p "$XDG_RUNTIME_DIR" && chmod 700 "$XDG_RUNTIME_DIR"

# Detail to the container how to run the app for execution
CMD ["/motorProto"]


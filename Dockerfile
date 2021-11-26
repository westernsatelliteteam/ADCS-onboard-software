FROM ubuntu:20.04

RUN apt-get -qy update                           \
    && apt-get -y install                        \
        make                                     \
        gcc-arm-none-eabi                        \
    && rm -rf /var/lib/apt/lists/*                           

WORKDIR /stm32

ENTRYPOINT [ "./compile.sh" ]
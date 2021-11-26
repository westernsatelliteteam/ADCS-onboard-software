FROM ubuntu:20.04

RUN apt-get -qy update                           \
    && apt-get -y install                        \
        make=4.2.1-1.2                           \
        gcc=4:9.3.0-1ubuntu2                     \
        g++=4:9.3.0-1ubuntu2                     \
    && rm -rf /var/lib/apt/lists/*                           

WORKDIR /stm32

ENTRYPOINT [ "./compile.sh" ]
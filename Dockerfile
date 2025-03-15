FROM ubuntu:latest

ENV Tz=America \
    DEBIAN_FRONTEND=noninteractive

ENV TOOLCHAIN_DIR=/usr/local/gcc-arm-none-eabi

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    wget \
    unzip \
    build-essential \
    git \
    make \
    cmake \
    python3 \
    xz-utils \
    cppcheck \
    vim

RUN apt-get install cpio libncurses6 -y

# get the toolchain
RUN wget https://developer.arm.com/-/media/Files/downloads/gnu/13.3.rel1/binrel/arm-gnu-toolchain-13.3.rel1-aarch64-arm-none-eabi.tar.xz -O gcc-arm-none-eabi.tar.xz --no-check-certificate

# unpack the archive to a neatly named target directory
RUN mkdir ${TOOLCHAIN_DIR}
RUN mv gcc-arm-none-eabi.tar.xz ${TOOLCHAIN_DIR}/gcc-arm-none-eabi.tar.xz
RUN cd ${TOOLCHAIN_DIR} && unxz gcc-arm-none-eabi.tar.xz

# extract tar file
RUN cd ${TOOLCHAIN_DIR} && tar xf gcc-arm-none-eabi.tar --strip-components 1
# remove the archive
RUN cd ${TOOLCHAIN_DIR} && rm gcc-arm-none-eabi.tar

# create symbolic links for libs needed by gdb
RUN ln -s /usr/lib/aarch64-linux-gnu/libncurses.so.6 /usr/lib/aarch64-linux-gnu/libcursesw.so.5
RUN ln -s /usr/lib/aarch64-linux-gnu/libtinfo.so.6 /usr/lib/aarch64-linux-gnu/libtinfo.so.5

# add the tools to the path
ENV PATH=${PATH}:${TOOLCHAIN_DIR}/bin

WORKDIR /synth


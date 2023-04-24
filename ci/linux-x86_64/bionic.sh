#!/bin/bash
set -eox pipefail

apt update -qq
apt install -yqq --no-install-recommends --no-install-suggests software-properties-common curl ca-certificates wget

add-apt-repository ppa:deadsnakes/ppa -y

apt -yqq install python3.10 python3.10-venv

curl -L -o cmake.sh https://github.com/Kitware/CMake/releases/download/v3.26.0-rc3/cmake-3.26.0-rc3-linux-x86_64.sh
chmod +x cmake.sh
./cmake.sh --skip-license --prefix=/usr
rm cmake.sh

apt-get upgrade --no-install-recommends --no-install-suggests -yqq
apt-get install --no-install-recommends --no-install-suggests -yqq clang unzip git automake autoconf pkg-config libtool build-essential ninja-build llvm-dev virtualenv libgl1-mesa-dev libfontconfig1-dev libfreetype6-dev libx11-dev libxext-dev libxfixes-dev libxi-dev libxrender-dev libxcb1-dev libxkbcommon-dev libx11-xcb-dev libxcb-glx0-dev libxkbcommon-x11-dev libd3dadapter9-mesa-dev libegl1-mesa-dev libgles2-mesa-dev software-properties-common gstreamer1.0-gl gstreamer1.0-plugins-base libgstreamer-gl1.0-0 libgstreamer-plugins-base1.0-0 libgstreamer-plugins-base1.0-dev libgstreamer1.0-0 libgstreamer1.0-dev g++-mingw-w64-x86-64 ccache libxcb-keysyms1-dev libxcb-image0-dev libxcb-shm0-dev libxcb-icccm4-dev libxcb-sync0-dev libxcb-xfixes0-dev libxcb-shape0-dev libxcb-randr0-dev libxcb-render-util0-dev libxcb-util-dev libxcb-xinerama0-dev libxcb-xkb-dev libxcb-xinput-dev libxcb-xinerama0-dev libx11-xcb-dev libxcb-xkb-dev libxcb-xinput-dev libxcb-glx0-dev libxcb-keysyms1-dev libxcb-image0-dev libxcb-shm0-dev libxcb-icccm4-dev libxcb-sync0-dev libxcb-xfixes0-dev libxcb-shape0-dev libxcb-randr0-dev libxcb-render-util0-dev libudev-dev libbluetooth-dev bluez libdbus-1-dev libpulse-dev

curl https://sh.rustup.rs -sSf | sh -s -- -y --default-toolchain 1.65.0
source /root/.cargo/env

add-apt-repository ppa:ubuntu-toolchain-r/test -y
apt-get update -y
apt-get install gcc-9 g++-9 -y
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 60 --slave /usr/bin/g++ g++ /usr/bin/g++-9
update-alternatives --config gcc

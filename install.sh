#!/bin/bash

set -e

LIB_DIR=$(pwd)

echo -e "\033[1;34mBuilding the main library...\033[0m"
rm -rfd "$LIB_DIR/build"
mkdir -p "$LIB_DIR/build"
cd "$LIB_DIR/build"
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build . --target install

read -p $'\033[1;32mDo you want to build the library examples (Y/n)? \033[0m' user_input

if [[ "$user_input" =~ ^[Yy]$ ]]; then
    echo -e "\033[1;34mBuilding examples...\033[0m"
    cmake .. -DBUILD_EXAMPLES=ON
    cmake --build .
else
    echo -e "\033[1;33mExamples will not be built.\033[0m"
fi

read -p $'\033[1;32mDo you want to build the tests (Y/n)? \033[0m' user_input

if [[ "$user_input" =~ ^[Yy]$ ]]; then
    echo -e "\033[1;34mBuilding tests...\033[0m"
    cmake .. -DBUILD_TESTS=ON -Wno-dev
    cmake --build .
else
    echo -e "\033[1;33mExamples will not be built.\033[0m"
fi

echo -e "\033[1;32mMake sure the library is installed at the following locations:\033[0m"
echo -e "  \033[1;36mHeaders: /usr/local/include/qutil\033[0m"
echo -e "  \033[1;36mLibraries: /usr/local/lib/libqutil.a\033[0m"
echo -e "  \033[1;36mExamples: $LIB_DIR/build/examples\033[0m"
echo -e "\033[1;32mGlobal installation completed!\033[0m"


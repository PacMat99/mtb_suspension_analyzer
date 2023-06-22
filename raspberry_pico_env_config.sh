#!/bin/bash

echo "Create pico folder"
cd ~/
mkdir pico
cd pico

echo ""
git clone https://github.com/raspberrypi/pico-sdk.git --branch master
cd pico-sdk
git submodule update --init
cd ..
git clone https://github.com/raspberrypi/pico-examples.git --branch master

echo "Install the toolchain"
apt update
apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential -y

echo "Update the SDK"
cd pico-sdk
git pull
git submodule update

echo "Export sdk path to .profile"
echo 'export PATH="/home/pacmat/pico/pico-sdk:$PATH"' >> ~/.profile

echo "create pico aliases (TO BE MODIFIED based on your environment)"
read -p "Are you using .bashrc? Otherwise it's supposed you're using .zshrc. (Y/n)" answer
bash_file=.bashrc
if [[ $answer -eq "n" || $answer -eq "N" ]]; then
    bash_file=.zshrc
fi
echo 'alias set_path="export PICO_SDK_PATH=/home/* your user */pico/pico-sdk"' >> $bash_file
echo 'alias upload_pico="cp /home/* your_path */mtb_suspension_analyzer/build/main.uf2 /media/* your user */RPI-RP2"' >> $bash_file
echo 'alias open_serial_monitor="sudo minicom -b 115200 -o -D /dev/ttyACM0"' >> $bash_file

echo "Install minicom for serial monitor"
apt install minicom -y

echo "Done! :D"
echo "IMPORTANT: you should update the aliases with your path and your user"

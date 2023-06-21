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
sudo apt update
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential

echo "Update the SDK"
cd pico-sdk
git pull
git submodule update

echo "Export sdk path to .profile"
echo 'export PATH="/home/pacmat/pico/pico-sdk:$PATH"' >> ~/.profile

echo "create pico aliases (TO BE MODIFIED based on your environment)"
read -p "Are you using .bashrc? Otherwise its supposed you're using .zshrc. (Y/n)" answer

if [[ $answer -eq "y" || $answer -eq "Y" ]]; then
echo 'alias set_path="export PICO_SDK_PATH=/home/* your user */pico/pico-sdk"' >> .bashrc
echo 'alias upload_pico="cp /home/* your_path */mtb_suspension_analyzer/build/main.uf2 /media/* your user */RPI-RP2"' >> .bashrc

elif [[ $answer -eq "n" || $answer -eq "N" ]]; then
echo 'alias set_path="export PICO_SDK_PATH=/home/* your user */pico/pico-sdk"' >> .zshrc
echo 'alias upload_pico="cp /home/* your_path */mtb_suspension_analyzer/build/main.uf2 /media/* your user */RPI-RP2"' >> .zshrc

fi

echo "Install minicom for serial monitor"
sudo apt install minicom

echo "Done! :D"

mkdir -p ../releasesrc
cd ../linux/EventListener
g++ main.cpp -o EventListener.out -pthread -lX11 -std=c++11
cp EventListener.out ../../releasesrc/EventListener
cd ../MoveMouse
g++ main.cpp -o MoveMouse.out -lX11 -std=c++11
cp MoveMouse.out ../../releasesrc/MoveMouse

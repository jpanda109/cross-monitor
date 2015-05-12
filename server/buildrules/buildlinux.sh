mkdir -p ../releasesrc
cd ../linux/EventListener
g++ main.cpp -o EventListener.out -lpthread -lX11 -std=c++11
cp EventListener.out ../../releasesrc
cd ../MoveMouse
g++ main.cpp -o MoveMouse.out -lX11 -std=c++11
cp MoveMouse.out ../../releasesrc

cd ../linux/EventListener
g++ main.cpp -o EventListener.out -lpthread -lX11 -std=c++11
mkdir -p ../../releasesrc
cp EventListener.out ../../releasesrc

DUMB: Dumb User Mode Bridge
======

DUMB is a tiny utility that uses libpcap capture and inject to bridge two Ethernet interfaces. In other words it just copies packets back and forth.

To build just type `make`. You will need libpcap development headers installed on Linux. These ship by default on MacOS (assuming you have XCode tools installed). Usage is just `sudo dumb <interface0> <interface1>`.

It's not going to be as fast as native routing or bridging but performance surprisingly is not that bad. Dumb is primarily intended for testing but could also be used in situations where you want to bridge another interfaces (such as a virtual one) without having to reconfigure your machine's entire network stack. As long as the use case is not terribly high bandwidth it should work fine.

(c)2018 [ZeroTier, Inc.](https://www.zerotier.com/) (MIT License)

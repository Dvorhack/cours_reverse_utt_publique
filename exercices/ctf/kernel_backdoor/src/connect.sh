#!/bin/sh
# Exit with CTRL+]
socat file:`tty`,raw,echo=0,rawer,escape=0x1d tcp:10.45.19.201:1337

# Hex #

# About #

Simple and small hexdump tool. Reads from STDIN and prints the
hexdump.

# Usage #

Hexdump utility:

    $ hex file.txt
    $ cat file.txt | hex

xxd's possibility to generate C include files :)

    $ hex -i file.txt >> file.h


# Build #

Just hit

    $ ./autogen.sh
    $ ./configure
    $ sudo make install

# Author #

(C) Kurt Kanzenbach 2015 <kurt@kmk-computers.de>

# License #

GPL Version 3

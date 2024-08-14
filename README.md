# curses-saver

A bouncing DVD-style terminal screensaver using ncurses 

# Build Instructions

```
git clone https://github.com/applearon/curses-saver && cd curses-saver
make
./out/curses-saver [-c config_file] [-f logo_file]
```
To build with debug information, pass the `DEBUG_BUILD=1` environment variable to `make`.

# Keybinds

* h to enable hyperspeed
* g to enable color change on hit
* w to enable rainbow logo
* f4 to enable debug menu
* q/Ctrl-C to exit

# Config
The configuration file has the following format:
```
# Comments
fps = 15
logo = examples/arch
```

# Credits
The examples were made with icons from neofetch, as well as the figlet command

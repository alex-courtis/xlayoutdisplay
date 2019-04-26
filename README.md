# xlayoutdisplay

Detects and arranges outputs for an X display, using [XRandR](https://www.x.org/wiki/Projects/XRandR/) for detection and [xrandr](https://wiki.archlinux.org/index.php/xrandr) for arrangement.

Highest refresh rate of the output's preferred resolution are used.

Left-to-right ordering is used, unless the user specifies mirrorred outputs.

Laptop displays (eDP*) are disabled when the lid is closed.

## Usage

```
Arranges outputs in a left to right manner, using highest resolution and refresh.
DPI is calculated based on the first or primary output's EDID information and rounded to the nearest 12.
Laptop outputs are turned off when the lid is closed.

e.g.  xlayoutdisplay -p DP-4 -o HDMI-0 -o DP-4

CLI:
  -h [ --help ]          print this help text and exit
  -i [ --info ]          print information about current outputs and exit
  -n [ --noop ]          perform a trial run and exit
  -v [ --version ]       print version string

CLI, /etc/xlayoutdisplay and ~/.xlayoutdisplay:
  -d [ --dpi ] arg       DPI override
  -m [ --mirror ]        mirror outputs using the lowest common resolution
  -o [ --order ] arg     order of outputs, repeat as needed
  -p [ --primary ] arg   primary output
  -q [ --quiet ]         suppress feedback
```

## Configuration File

`~/.xlayoutdisplay` then `/etc/xlayoutdisplay` may be used to provide defaults, which will be overwritten by CLI options.

See [xlayoutdisplay](.xlayoutdisplay)

## Sample Output

DP-4 is the only ouput, then HDMI-0 is plugged in.

`xlayoutdisplay -p DP-4 -o HDMI-0 -o DP-4`

HDMI-0 is enabled to the left of DP-4, however DP-4 is still the primary output that determines DPI.

```
DVI-D-0 disconnected
HDMI-0 connected 0cm/0cm
 + 1920x1080 50Hz
   2880x576 50Hz
   2880x576 50Hz
   2880x480 60Hz
   2880x480 60Hz
   1920x1080 60Hz
   1920x1080 60Hz
   1920x1080 24Hz
   1920x1080 60Hz
  !1920x1080 60Hz
   1920x1080 50Hz
   1440x576 50Hz
   1440x480 60Hz
   1280x720 60Hz
   1280x720 50Hz
   720x576 50Hz
   720x480 60Hz
   640x480 60Hz
DP-0 disconnected
DP-1 disconnected
DP-2 disconnected
DP-3 disconnected
DP-4 active 60cm/34cm 2560x1440+0+0 165Hz
 + 2560x1440 60Hz
* !2560x1440 165Hz
   2560x1440 144Hz
   2560x1440 120Hz
   2560x1440 100Hz
   2560x1440 85Hz
   2560x1440 24Hz
   1024x768 60Hz
   800x600 60Hz
   640x480 60Hz
*current +preferred !optimal

laptop lid open or not present

calculated DPI 108 for output DP-4

xrandr \
 --dpi 108 \
 --output HDMI-0 --mode 1920x1080 --rate 60 --pos 0x0 \
 --output DP-4 --mode 2560x1440 --rate 165 --pos 1920x0 --primary \
 --output DVI-D-0 --off \
 --output DP-0 --off \
 --output DP-1 --off \
 --output DP-2 --off \
 --output DP-3 --off
 
echo "Xft.dpi: 108" | xrdb -merge
```

End state:
```
/--------------\/----------------------\
|              ||                      |
|    HDMI-0    ||                      |
|              ||        DP-4          |
|  1920x1080   ||                      |
|              ||     2560x1440        |
\--------------/|                      |
                |                      |
                |                      |
                \----------------------/
```

## Developing

[JetBrains CLion](https://www.jetbrains.com/clion/) highly recommended.

### Build

```
git clone https://github.com/alex-courtis/xlayoutdisplay.git
cd xlayoutdisplay
mkdir build
cd build
cmake ..
make
```

### Test

Install [Google Test](https://github.com/google/googletest) and [Google Mock](https://github.com/google/googlemock).

```
cd build
cmake ..
make
./unitTests
```

## Contributing

PRs very welcome: fork this repo and submit a PR.

## Bugs / Features

Please raise an issue.

Note again that PRs are very welcome ;)

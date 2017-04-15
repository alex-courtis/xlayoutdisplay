# xLayoutDisplays

Detects and arranges linux displays, using [XRandR](https://www.x.org/wiki/Projects/XRandR/) for detection and [xrandr](https://wiki.archlinux.org/index.php/xrandr) (for now...) for arrangement.

## Usage

```
Detects and arranges displays in a left to right manner.
Invokes xrandr to perform arrangement.
Highest resolution and refresh are used for each display.
Displays starting with "eDP" are disabled if the laptop lid is closed as per /proc/acpi/button/lid/.*/state
Displays are ordered via Xrandr default.
The first display will be primary unless -p specified.

Usage: xLayoutDisplays [-h] [-i] [-m] [-n] [-o order] [-p primary] [-q]
  -h  display this help text and exit
  -i  display information about current displays and exit
  -m  mirror displays using the lowest common resolution
  -n  perform a trial run and exit
  -o  order of displays, space/comma delimited
  -p  primary display
  -q  suppress output

e.g.: xLayoutDisplays -o DP-0,HDMI-0 -p HDMI-0
  arranges DP-0 left, HDMI-0 right, with any remaining displays further right, with HDMI-0 as primary
```

## Sample Output

DP-0 was the only active (primary) display. HDMI-0 is plugged in.

`xLayoutDisplays -o "DP-0 HDMI-0" -p HDMI-0`

After invocation, HDMI-0 is enabled, to the right of DP-0, and HDMI-0 is the new primary.

```
DVI-D-0 disconnected
HDMI-0 connected
  !1920x1080 60Hz
   1920x1080 60Hz
   1920x1080 60Hz
   1920x1080 50Hz
   1920x1080 50Hz
   1920x1080 30Hz
   1920x1080 25Hz
   1920x1080 24Hz
 + 1280x720 60Hz
   1280x720 60Hz
   1280x720 50Hz
   1024x768 60Hz
   800x600 60Hz
   720x480 60Hz
   720x480 60Hz
   720x576 50Hz
DP-0 active 2560x1440+0+0 165Hz
* !2560x1440 165Hz
   2560x1440 144Hz
   2560x1440 120Hz
   2560x1440 100Hz
   2560x1440 85Hz
 + 2560x1440 60Hz
   2560x1440 24Hz
   1024x768 60Hz
   800x600 60Hz
   640x480 60Hz
DP-1 disconnected
DP-2 disconnected
DP-3 disconnected
DP-4 disconnected
*current +preferred !optimal

lid open or not present

xrandr \
 --output DP-0 --mode 2560x1440 --rate 165 --pos 0x0 \
 --output HDMI-0 --mode 1920x1080 --rate 60 --pos 2560x0 --primary \
 --output DVI-D-0 --off \
 --output DP-1 --off \
 --output DP-2 --off \
 --output DP-3 --off \
 --output DP-4 --off

```

## Building

Clone and cmake

## TODO

Hotplug event detection... my udev event hacks are too unreliable and shameworthy right now. Maybe a systemd user service?

maybe -v or other arrangement options

-a after e.g. `xmonad --restart`

xrdb Xft.dpi suggestion or setting

# xLayoutDisplays

Detects and arranges displays.

## Usage

```
Detects and arranges displays in a left to right manner.
Invokes xrandr to perform arrangement.
Highest resolution and refresh are used for each display.
Displays starting with "eDP" are disabled if the laptop lid is closed as per /proc/acpi/button/lid/.*/state
Displays are ordered via Xrandr default.
The first display will be primary unless -p specified.

Usage: xLayoutDisplays [-h] [-i] [-n] [-o order] [-p primary] [-q]
  -h  display this help text and exit
  -i  display information about current displays and exit
  -m  *mirror displays using the lowest common resolution (not ready yet, havering about panning)
  -n  perform a trial run and exit
  -o  order of displays, space/comma delimited
  -p  primary display
  -q  suppress output
  -v  *arrange displays in a top to bottom manner (is this needed?)

e.g.: xLayoutDisplays -o DP-0,HDMI-0 -p HDMI-0
  arranges DP-0 left, HDMI-0 right, with any remaining displays further right, with HDMI-0 as primary
```

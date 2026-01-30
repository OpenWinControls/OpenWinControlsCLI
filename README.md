# OpenWinControlsCLI

Multiplatform GPD WinControls replacement, command-line version.

## Features

- Import and export mappings from/to yaml files
- Pretty-print current firmware settings
- Print a list of supported keycodes
- Allows to remap all buttons
- Allows to change controller settings, like deadzone, leds etc.., where available

## Linux

Root permissions are required.

To run without root, allow access to the controller:

Create the file **70-gpd-controller.rules** in **/etc/udev/rules.d**

```text
SUBSYSTEMS=="usb", ATTRS{idVendor}=="2f24", ATTRS{idProduct}=="0135", TAG+="uaccess"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="2f24", ATTRS{idProduct}=="0137", TAG+="uaccess"
```

Load the new rules:

```bash
sudo udevadm control --reload-rules && sudo udevadm trigger
```

## Usage

```text
OpenWinControlsCLI 1.0

Usage: OpenWinControlsCLI command [args]

Commands:

  help
    show help

  keys
    print supported keys

  set option value [..]
    set firmware settings
    Example: set du w dl space [..]

  export file_name.yaml
    export current firmware mapping to a yaml file to share with others or apply back later

  import file_name.yaml
    apply mapping from file

  print
    Print current firmware settings

Options:

  du
    Assign dpad up a key

  dd
    Assign dpad down a key

  dl
    Assign dpad left a key

  dr
    Assign dpad right a key

  a
    Assign A button a key

  b
    Assign B button a key

  x
    Assign X button a key

  y
    Assign Y button a key

  lu
    Assign left analog up a key

  ld
    Assign left analog down a key

  ll
    Assign left analog left a key

  lr
    Assign left analog right a key

  st
    Assign start button a key

  sl
    Assign select button a key

  mu
    Assign menu button a key

  l1
    Assign L1 button a key

  l2
    Assign L2 button a key

  l3
    Assign L3 button a key

  r1
    Assign R1 button a key

  r2
    Assign R2 button a key

  r3
    Assign R3 button a key

  bl1
    Assign left back button slot 1 a key

  bl1d
    Set left back button slot 1 key delay in milliseconds

  bl2
    Assign left back button slot 2 a key

  bl2d
    Set left back button slot 2 key delay in milliseconds

  bl3
    Assign left back button slot 3 a key

  bl3d
    Set left back button slot 3 key delay in milliseconds

  bl4
    Assign left back button slot 4 a key

  bl4d
    Set left back button slot 4 key delay in milliseconds

  br1
    Assign right back button slot 1 a key

  br1d
    Set right back button slot 1 key delay in milliseconds

  br2
    Assign right back button slot 2 a key

  br2d
    Set right back button slot 2 key delay in milliseconds

  br3
    Assign right back button slot 3 a key

  br3d
    Set right back button slot 3 key delay in milliseconds

  br4
    Assign right back button slot 4 a key

  br4d
    Set right back button slot 4 key delay in milliseconds

  rmb
    Set vibration intensity [off, low, high]

  lc
    Adjust left analog deadzone [-10, +10]

  lb
    Adjust left analog boundary [-10, +10]

  rc
    Adjust right analog deadzone [-10, +10]

  rb
    Adjust right analog boundary [-10, +10]

  led
    Set shoulder buttons led mode [off, solid, breathe, rotate]

  ledclr
    Set shoulder buttons led color [R:G:B] [0-255]

Notes:

  Deadzone settings:
     This is composed of two values, center and boundary.
     Center refers to the deadzone itself, 0 is the default value from GPD, roughtly ~15%.
     A value of -10 removes the deadzone.
     Boundary refers to the circularity, 0 is the default value from GPD, roughtly ~13% average error.
     A value of -10 should lessen the average error on circularity tests.

  Export/Import:
    You can export current firmware settings to file or apply mappings from file.
    The format used by this app is yaml.
    Export example: e sims3 (no extension required, this will export your current mapping to sims3.yaml
    Import example: i sims3.yaml (this will read, and apply, mapping from sims3.yaml
```
## How to build

```bash
git clone --recursive https://github.com/OpenWinControls/OpenWinControlsCLI
git submodule update --init --recursive
cmake -B build
make -C build
```

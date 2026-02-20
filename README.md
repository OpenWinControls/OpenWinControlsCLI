# OpenWinControlsCLI

Multiplatform GPD WinControls replacement, command-line version.

## Features

- Import and export mappings from/to yaml files
- Pretty-print current firmware settings
- Print a list of supported keycodes
- Allows to remap all buttons
- Allows to change controller settings, like deadzone, leds etc.., where available

## Known device firmware bugs (must be fixed by GPD)

### Win5

- RT and LT ignore the keycode value in firmware config
- All back button modes don't work as expected
- Back buttons send F14 (for l4) and F15 (for r4) keycodes in addition to the user defined keycode,
  disrupting the correct functionality of the macro mode
- Controller may softlock itself when entering sleep mode by the OS, no longer processing commands,
  requiring the user to manually switch mode back and forth using the physical button on device

## Current controllerV2 limitations

- To apply the changes permanently, switch the controller mode back and forth using the physical button on device

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

**Controller V2 macros**

Chain multiple keys by assigning an incremental start time (**l4d**/**r4d**) and a fixed hold time to each slot.

To simulate a single key press, set one key slot, start time (**l4d**) to 0
and hold time (**l4h**) to something high like **300**.

### Help

```text
OpenWinControlsCLI 2.2

Usage: OpenWinControlsCLI command [args]

Some options only apply to V1 or V2, incompatible options, if provided, are ignored.

Commands:

  help
    show help

  keys
    print supported keyboard[&mouse] mode keys

  xkeys
    print supported xinput mode keys

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

  du [key]
    Assign dpad up a key

  dd [key]
    Assign dpad down a key

  dl [key]
    Assign dpad left a key

  dr [key]
    Assign dpad right a key

  a [key]
    Assign A button a key

  b [key]
    Assign B button a key

  x [key]
    Assign X button a key

  y [key]
    Assign Y button a key

  lu [key]
    Assign left analog up a key

  ld [key]
    Assign left analog down a key

  ll [key]
    Assign left analog left a key

  lr [key]
    Assign left analog right a key

  st [key]
    Assign start button a key

  sl [key]
    Assign select button a key

  mu [key]
    Assign menu button a key

  l1 [key]
    Assign L1 button a key

  l2 [key]
    Assign L2 button a key

  l3 [key]
    Assign L3 button a key

  r1 [key]
    Assign R1 button a key

  r2 [key]
    Assign R2 button a key

  r3 [key]
    Assign R3 button a key

  xdu [xinput button]
    Reassign dpad up button

  xdd [xinput button]
    Reassign dpad down button

  xdl [xinput button]
    Reassign dpad left button

  xdr [xinput button]
    Reassign dpad right button

  xa [xinput button]
    Reassign A button button

  xb [xinput button]
    Reassign B button button

  xx [xinput button]
    Reassign X button button

  xy [xinput button]
    Reassign Y button button

  xlu [xinput button]
    Reassign left analog up button

  xld [xinput button]
    Reassign left analog down button

  xll [xinput button]
    Reassign left analog left button

  xlr [xinput button]
    Reassign left analog right button

  xru [xinput button]
    Reassign right analog up button

  xrd [xinput button]
    Reassign right analog down button

  xrl [xinput button]
    Reassign right analog left button

  xrr [xinput button]
    Reassign right analog right button

  xst [xinput button]
    Reassign start button button

  xsl [xinput button]
    Reassign select button button

  xmu [xinput button]
    Reassign menu button button

  xl1 [xinput button]
    Reassign L1 button button

  xl2 [xinput button]
    Reassign L2 button button

  xl3 [xinput button]
    Reassign L3 button button

  xr1 [xinput button]
    Reassign R1 button button

  xr2 [xinput button]
    Reassign R2 button button

  xr3 [xinput button]
    Reassign R3 button button

  l4 [key1,key2,key3..]
    Comma separated list of keys
    Assign L4 back button

  l4d [time1,time2..]
    Comma separated list of times
    Set L4 back button keys start time in milliseconds

  l4h [time1,time2..]
    Comma separated list of times
    Set L4 back button keys hold time in milliseconds

  l4n [num]
    Manually override L4 macro active slots number [0, 32]

  r4 [key1,key2,key3..]
    Comma separated list of keys
    Assign R4 back button

  r4d [time1,time2..]
    Comma separated list of times
    Set R4 back button keys start time in milliseconds

  r4h [time1,time2..]
    Comma separated list of times
    Set R4 back button keys hold time in milliseconds

  r4n [num]
    Manually override R4 macro active slots number [0, 32]

  rmb [mode]
    Set vibration intensity [0 = off, 1 = low, 2 = high]

  lc [value]
    Adjust left analog deadzone [-10, +10]

  lb [value]
    Adjust left analog boundary [-10, +10]

  rc [value]
    Adjust right analog deadzone [-10, +10]

  rb [value]
    Adjust right analog boundary [-10, +10]

  led [mode]
    Set shoulder buttons led mode [0 = off, 1 = solid, 2 = breathe, 3 = rotate]

  ledclr [R:G:B]
    Set shoulder buttons led color [0-255:0-255:0-255]

Notes:

  Controller V1 features:
     Supports up to 4 key/time slots for back buttons macro.
     If more keys/times are provied, they will be ignored.
     The 4th time slot is special, it sets the whole macro start time.

  Controller V2 features:
     Supports up to 32 key/time/hold slots for back buttons.
     The number of active key slots is automatically updated on write.

  Deadzone settings:
     This is composed of two values, center and boundary.
     Center refers to the deadzone itself, 0 is the default value from GPD, roughtly ~15%.
     A value of -10 removes the deadzone.
     Boundary refers to the circularity, 0 is the default value from GPD, roughtly ~13% average error.
     A value of -10 should lessen the average error on circularity tests.
```
## How to build

```bash
git clone --recursive https://github.com/OpenWinControls/OpenWinControlsCLI
git submodule update --init --recursive
cmake -B build
make -C build
```

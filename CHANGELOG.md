## future

- Fix print showing all values as hex

## 2.7

- Fix initialization for some mini 25 models
- Fix controller version checks (nothing affected by this, but was possible in future)
- Fix displayed version string in some cases

## 2.6

- Fix back buttons not being set after L4
- Support controller v2 L5 and R5 buttons (win5 only implements R5 as the controller mode switch button, mini 25 has none)
- Fix crash when importing from yaml due to inverted args in function call

## 2.5

- Do not crash when importing malformed yamls

## 2.4

- Update libOpenWinControls

## 2.3

- ControllerV2: Fix automatic active slots count for back buttons
- Add reset option

## 2.2

- ControllerV2: allow to override back buttons macro active slots number

## 2.1

- ControllerV2: enable creation of back buttons macro with 5+ keys

## 2.0

- Support win5 and mini 25
- Fix Windows device detection
- Fix invalid function error in Windows
- Improve help text
- Change left/right back button args to l4/r4 and comma separated list of values
- **Breaking change**: update yaml profiles structure
- Allow case insensitive key values in yaml profiles
- code improvements

## 1.0

Public release

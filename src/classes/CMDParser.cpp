/*
 * This file is part of OpenWinControlsCLI.
 * Copyright (C) 2026 kylon
 *
 * OpenWinControlsCLI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenWinControlsCLI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cstring>

#include "CMDParser.h"
#include "../version.h"
#include "../extern/libOpenWinControls/src/include/HIDUsageIDMap.h"
#include "../extern/libOpenWinControls/src/include/XinputUsageIDMap.h"

namespace OWC {
    CMDParser::CMDParser(const int argc, char *argv[]) {
        argC = argc - 1;
        argV = argv + 1;
    }

    void CMDParser::showHelp() const {
        std::cout << APP_NAME << " " << APP_VER_MAJOR << "." << APP_VER_MINOR << "\n\n"
            "Usage: " << APP_NAME << " command [args]\n\n"

            "Some options only apply to V1 or V2, incompatible options, if provided, are ignored.\n\n"

            "Commands:\n\n"
            "  help\n"
            "    show help\n\n"
            "  keys\n"
            "    print supported keyboard[&mouse] mode keys\n\n"
            "  xkeys\n"
            "    print supported xinput mode keys\n\n"
            "  set option value [..]\n"
            "    set firmware settings\n"
            "    Example: set du w dl space [..]\n\n"
            "  export file_name.yaml\n"
            "    export current firmware mapping to a yaml file to share with others or apply back later\n\n"
            "  import file_name.yaml\n"
            "    apply mapping from file\n\n"
            "  print\n"
            "    Print current firmware settings\n\n"

            "Options:\n\n"
            "  du [key]\n"
            "    Assign dpad up a key\n\n"
            "  dd [key]\n"
            "    Assign dpad down a key\n\n"
            "  dl [key]\n"
            "    Assign dpad left a key\n\n"
            "  dr [key]\n"
            "    Assign dpad right a key\n\n"
            "  a [key]\n"
            "    Assign A button a key\n\n"
            "  b [key]\n"
            "    Assign B button a key\n\n"
            "  x [key]\n"
            "    Assign X button a key\n\n"
            "  y [key]\n"
            "    Assign Y button a key\n\n"
            "  lu [key]\n"
            "    Assign left analog up a key\n\n"
            "  ld [key]\n"
            "    Assign left analog down a key\n\n"
            "  ll [key]\n"
            "    Assign left analog left a key\n\n"
            "  lr [key]\n"
            "    Assign left analog right a key\n\n"
            "  st [key]\n"
            "    Assign start button a key\n\n"
            "  sl [key]\n"
            "    Assign select button a key\n\n"
            "  mu [key]\n"
            "    Assign menu button a key\n\n"
            "  l1 [key]\n"
            "    Assign L1 button a key\n\n"
            "  l2 [key]\n"
            "    Assign L2 button a key\n\n"
            "  l3 [key]\n"
            "    Assign L3 button a key\n\n"
            "  r1 [key]\n"
            "    Assign R1 button a key\n\n"
            "  r2 [key]\n"
            "    Assign R2 button a key\n\n"
            "  r3 [key]\n"
            "    Assign R3 button a key\n\n"
            "  xdu [xinput button]\n"
            "    Reassign dpad up button\n\n"
            "  xdd [xinput button]\n"
            "    Reassign dpad down button\n\n"
            "  xdl [xinput button]\n"
            "    Reassign dpad left button\n\n"
            "  xdr [xinput button]\n"
            "    Reassign dpad right button\n\n"
            "  xa [xinput button]\n"
            "    Reassign A button button\n\n"
            "  xb [xinput button]\n"
            "    Reassign B button button\n\n"
            "  xx [xinput button]\n"
            "    Reassign X button button\n\n"
            "  xy [xinput button]\n"
            "    Reassign Y button button\n\n"
            "  xlu [xinput button]\n"
            "    Reassign left analog up button\n\n"
            "  xld [xinput button]\n"
            "    Reassign left analog down button\n\n"
            "  xll [xinput button]\n"
            "    Reassign left analog left button\n\n"
            "  xlr [xinput button]\n"
            "    Reassign left analog right button\n\n"
            "  xru [xinput button]\n"
            "    Reassign right analog up button\n\n"
            "  xrd [xinput button]\n"
            "    Reassign right analog down button\n\n"
            "  xrl [xinput button]\n"
            "    Reassign right analog left button\n\n"
            "  xrr [xinput button]\n"
            "    Reassign right analog right button\n\n"
            "  xst [xinput button]\n"
            "    Reassign start button button\n\n"
            "  xsl [xinput button]\n"
            "    Reassign select button button\n\n"
            "  xmu [xinput button]\n"
            "    Reassign menu button button\n\n"
            "  xl1 [xinput button]\n"
            "    Reassign L1 button button\n\n"
            "  xl2 [xinput button]\n"
            "    Reassign L2 button button\n\n"
            "  xl3 [xinput button]\n"
            "    Reassign L3 button button\n\n"
            "  xr1 [xinput button]\n"
            "    Reassign R1 button button\n\n"
            "  xr2 [xinput button]\n"
            "    Reassign R2 button button\n\n"
            "  xr3 [xinput button]\n"
            "    Reassign R3 button button\n\n"
            "  l4 [key1,key2,key3..]\n"
            "    Comma separated list of keys\n"
            "    Assign L4 back button\n\n"
            "  l4d [time1,time2..]\n"
            "    Comma separated list of times\n"
            "    Set L4 back button keys start time in milliseconds\n\n"
            "  l4h [time1,time2..]\n"
            "    Comma separated list of times\n"
            "    Set L4 back button keys hold time in milliseconds\n\n"
            "  l4n [num]\n"
            "    Manually override L4 macro active slots number [0, 32]\n\n"
            "  r4 [key1,key2,key3..]\n"
            "    Comma separated list of keys\n"
            "    Assign R4 back button\n\n"
            "  r4d [time1,time2..]\n"
            "    Comma separated list of times\n"
            "    Set R4 back button keys start time in milliseconds\n\n"
            "  r4h [time1,time2..]\n"
            "    Comma separated list of times\n"
            "    Set R4 back button keys hold time in milliseconds\n\n"
            "  r4n [num]\n"
            "    Manually override R4 macro active slots number [0, 32]\n\n"
            "  rmb [mode]\n"
            "    Set vibration intensity [0 = off, 1 = low, 2 = high]\n\n"
            "  lc [value]\n"
            "    Adjust left analog deadzone [-10, +10]\n\n"
            "  lb [value]\n"
            "    Adjust left analog boundary [-10, +10]\n\n"
            "  rc [value]\n"
            "    Adjust right analog deadzone [-10, +10]\n\n"
            "  rb [value]\n"
            "    Adjust right analog boundary [-10, +10]\n\n"
            "  led [mode]\n"
            "    Set shoulder buttons led mode [0 = off, 1 = solid, 2 = breathe, 3 = rotate]\n\n"
            "  ledclr [R:G:B]\n"
            "    Set shoulder buttons led color [0-255:0-255:0-255]\n\n"

            "Notes:\n\n"
            "  Controller V1 features:\n"
            "     Supports up to 4 key/time slots for back buttons macro.\n"
            "     If more keys/times are provied, they will be ignored.\n"
            "     The 4th time slot is special, it sets the whole macro start time.\n\n"

            "  Controller V2 features:\n"
            "     Supports up to 32 key/time/hold slots for back buttons.\n"
            /*"     Back buttons have 3 modes: single, 4-buttons, 32-slots macro.\n"
            "     Mode is automatically updated based on the number of keys being set: 1, 2-4, 5+.\n"*/
            "     The number of active key slots is automatically updated on write.\n\n"

            "  Deadzone settings:\n"
            "     This is composed of two values, center and boundary.\n"
            "     Center refers to the deadzone itself, 0 is the default value from GPD, roughtly ~15%.\n"
            "     A value of -10 removes the deadzone.\n"
            "     Boundary refers to the circularity, 0 is the default value from GPD, roughtly ~13% average error.\n"
            "     A value of -10 should lessen the average error on circularity tests.\n\n";
    }

    void CMDParser::showKeys() const {
        std::cout << APP_NAME << " " << APP_VER_MAJOR << "." << APP_VER_MINOR << "\n\n"
            "Keyboard[&Mouse] mode key values:\n";

        for (const auto &[code, key]: HIDUsageIDMap)
            std::cout << "  " << key << "\n";
    }

    void CMDParser::showXKeys() const {
        std::cout << APP_NAME << " " << APP_VER_MAJOR << "." << APP_VER_MINOR << "\n\n"
            "Xinput mode key values:\n";

        for (const auto &[code, key]: XinputUsageIDMap)
            std::cout << "  " << key << "\n";
    }

    bool CMDParser::isArg(const std::string_view arg) const {
        return arg == argV[0];
    }

    bool CMDParser::parseSetOptions() {
        if (argC < 1) {
            showHelp();
            return false;
        }

        while (argC > 0) {
            if (isArg("du") || isArg("dd") || isArg("dl") || isArg("dr") ||
                isArg("a") || isArg("b") || isArg("x") || isArg("y") ||
                isArg("lu") || isArg("ld") || isArg("ll") || isArg("lr") ||
                isArg("st") || isArg("sl") || isArg("mu") || isArg("l1") ||
                isArg("l2") || isArg("l3") || isArg("r1") || isArg("r2") ||
                isArg("r3") || isArg("xdu") || isArg("xdd") || isArg("xdl") ||
                isArg("xdr") || isArg("xa") || isArg("xb") || isArg("xx") ||
                isArg("xy") || isArg("xst") || isArg("xsl") || isArg("xmu") ||
                isArg("xlu") || isArg("xld") || isArg("xll") || isArg("xlr") ||
                isArg("xru") || isArg("xrd") || isArg("xrl") || isArg("xrr") ||
                isArg("xl1") || isArg("xl2") || isArg("xl3") || isArg("xr1") ||
                isArg("xr2") || isArg("xr3"))
            {
                std::string key = argV[1];

                std::erase_if(key, [](const char c)->bool { return std::isspace(c); });
                std::transform(key.begin(), key.end(), key.begin(), ::toupper);
                args.emplace(argV[0], key);

            } else if (isArg("l4") || isArg("r4")) {
                std::vector<std::string> keys;
                char *s = strtok(argV[1], ",");

                while (s != nullptr) {
                    std::string key = s;

                    std::transform(key.begin(), key.end(), key.begin(), ::toupper);
                    keys.emplace_back(key);
                    s = strtok(nullptr, ",");
                }

                args.emplace(argV[0], keys);

            } else if (isArg("l4d") || isArg("l4h") || isArg("r4d") || isArg("r4h")) {
                std::vector<int> times;
                char *s = strtok(argV[1], ",");

                while (s != nullptr) {
                    times.emplace_back(std::stoi(s));

                    s = strtok(nullptr, ",");
                }

                args.emplace(argV[0], times);

            } else if (isArg("lc") || isArg("lb") || isArg("rc") || isArg("rb") ||
                       isArg("led") || isArg("rmb") || isArg("l4n") || isArg("r4n"))
            {
                args.emplace(argV[0], std::stoi(argV[1]));

            } else if (isArg("ledclr")) {
                int r, g, b;

                if (std::sscanf(argV[1], "%d:%d:%d", &r, &g, &b) != 3) {
                    std::cerr << "invalid ledclr value\n";
                    return false;
                }

                args.emplace(argV[1], std::make_tuple(r, g ,b));
            }

            argC -= 2;
            argV += 2;
        }

        return true;
    }

    bool CMDParser::parse() {
        if (argC < 1 || isArg("help")) {
            showHelp();
            return false;

        } else if (isArg("keys")) {
            showKeys();
            return false;

        } else if (isArg("xkeys")) {
            showXKeys();
            return false;

        } else if (isArg("print")) {
            args.emplace(argV[0], 0);
            return true;

        } else if (isArg("export") || isArg("import")) {
            args.emplace(argV[0], argV[1]);
            return true;

        } else if (isArg("set")) {
            args.emplace(argV[0], 0);
            --argC;
            ++argV;
            return parseSetOptions();
        }

        showHelp();
        return false;
    }
}

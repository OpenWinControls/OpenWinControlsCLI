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

#include "CMDParser.h"
#include "../version.h"
#include "../extern/libOpenWinControls/src/include/HIDUsageIDMap.h"

namespace OWC {
    CMDParser::CMDParser(const int argc, char *argv[]) {
        argC = argc - 1;
        argV = argv + 1;
    }

    void CMDParser::showHelp() const {
        std::cout << APP_NAME << " " << APP_VER_MAJOR << "." << APP_VER_MINOR << "\n\n"
            "Usage: " << APP_NAME << " command [args]\n\n"

            "Commands:\n\n"
            "  help\n"
            "    show help\n\n"
            "  keys\n"
            "    print supported keys\n\n"
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
            "  du\n"
            "    Assign dpad up a key\n\n"
            "  dd\n"
            "    Assign dpad down a key\n\n"
            "  dl\n"
            "    Assign dpad left a key\n\n"
            "  dr\n"
            "    Assign dpad right a key\n\n"
            "  a\n"
            "    Assign A button a key\n\n"
            "  b\n"
            "    Assign B button a key\n\n"
            "  x\n"
            "    Assign X button a key\n\n"
            "  y\n"
            "    Assign Y button a key\n\n"
            "  lu\n"
            "    Assign left analog up a key\n\n"
            "  ld\n"
            "    Assign left analog down a key\n\n"
            "  ll\n"
            "    Assign left analog left a key\n\n"
            "  lr\n"
            "    Assign left analog right a key\n\n"
            "  st\n"
            "    Assign start button a key\n\n"
            "  sl\n"
            "    Assign select button a key\n\n"
            "  mu\n"
            "    Assign menu button a key\n\n"
            "  l1\n"
            "    Assign L1 button a key\n\n"
            "  l2\n"
            "    Assign L2 button a key\n\n"
            "  l3\n"
            "    Assign L3 button a key\n\n"
            "  r1\n"
            "    Assign R1 button a key\n\n"
            "  r2\n"
            "    Assign R2 button a key\n\n"
            "  r3\n"
            "    Assign R3 button a key\n\n"
            "  bl1\n"
            "    Assign left back button slot 1 a key\n\n"
            "  bl1d\n"
            "    Set left back button slot 1 key delay in milliseconds\n\n"
            "  bl2\n"
            "    Assign left back button slot 2 a key\n\n"
            "  bl2d\n"
            "    Set left back button slot 2 key delay in milliseconds\n\n"
            "  bl3\n"
            "    Assign left back button slot 3 a key\n\n"
            "  bl3d\n"
            "    Set left back button slot 3 key delay in milliseconds\n\n"
            "  bl4\n"
            "    Assign left back button slot 4 a key\n\n"
            "  bl4d\n"
            "    Set left back button slot 4 key delay in milliseconds\n\n"
            "  br1\n"
            "    Assign right back button slot 1 a key\n\n"
            "  br1d\n"
            "    Set right back button slot 1 key delay in milliseconds\n\n"
            "  br2\n"
            "    Assign right back button slot 2 a key\n\n"
            "  br2d\n"
            "    Set right back button slot 2 key delay in milliseconds\n\n"
            "  br3\n"
            "    Assign right back button slot 3 a key\n\n"
            "  br3d\n"
            "    Set right back button slot 3 key delay in milliseconds\n\n"
            "  br4\n"
            "    Assign right back button slot 4 a key\n\n"
            "  br4d\n"
            "    Set right back button slot 4 key delay in milliseconds\n\n"
            "  rmb\n"
            "    Set vibration intensity [off, low, high]\n\n"
            "  lc\n"
            "    Adjust left analog deadzone [-10, +10]\n\n"
            "  lb\n"
            "    Adjust left analog boundary [-10, +10]\n\n"
            "  rc\n"
            "    Adjust right analog deadzone [-10, +10]\n\n"
            "  rb\n"
            "    Adjust right analog boundary [-10, +10]\n\n"
            "  led\n"
            "    Set shoulder buttons led mode [off, solid, breathe, rotate]\n\n"
            "  ledclr\n"
            "    Set shoulder buttons led color [R:G:B] [0-255]\n\n"

            "Notes:\n\n"

            "  Deadzone settings:\n"
            "     This is composed of two values, center and boundary.\n"
            "     Center refers to the deadzone itself, 0 is the default value from GPD, roughtly ~15%.\n"
            "     A value of -10 removes the deadzone.\n"
            "     Boundary refers to the circularity, 0 is the default value from GPD, roughtly ~13% average error.\n"
            "     A value of -10 should lessen the average error on circularity tests.\n\n";
    }

    void CMDParser::showKeys() const {
        std::cout << APP_NAME << " " << APP_VER_MAJOR << "." << APP_VER_MINOR << "\n\n"
            "Here an almost complete list of possible values to assign:\n";

        for (const auto &[code, key]: HIDUsageIDMap)
            std::cout << "  " << key << "\n";
    }

    bool CMDParser::isArg(const std::string_view arg) const {
        return arg == argV[0];
    }

    bool CMDParser::parseSetOptions() {
        using namespace std::string_view_literals;

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
                isArg("r3") || isArg("bl1") || isArg("bl2") || isArg("bl3") ||
                isArg("bl4") || isArg("br1") || isArg("br2") || isArg("br3") ||
                isArg("br4"))
            {
                std::string key = argV[1];

                std::erase_if(key, [](const char c)->bool { return std::isspace(c); });
                std::transform(key.begin(), key.end(), key.begin(), ::toupper);
                args.emplace(argV[0], key);

            } else if (isArg("bl1d") || isArg("bl2d") || isArg("bl3d") ||
                isArg("bl4d") || isArg("br1d") || isArg("br2d") || isArg("br3d") ||
                isArg("br4d") || isArg("lc") || isArg("lb") || isArg("rc") || isArg("rb"))
            {
                args.emplace(argV[0], std::stoi(argV[1]));

            } else if (isArg("rmb")) {
                if (argV[1] != "off"sv && argV[1] != "low"sv && argV[1] != "high"sv) {
                    std::cerr << "invalid rmb value\n";
                    return false;
                }

                args.emplace(argV[0], argV[1]);

            } else if (isArg("led")) {
                if (argV[1] != "off"sv && argV[1] != "solid"sv && argV[1] != "breathe"sv && argV[1] != "rotate"sv) {
                    std::cerr << "invalid led value\n";
                    return false;
                }

                args.emplace(argV[0], argV[1]);

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

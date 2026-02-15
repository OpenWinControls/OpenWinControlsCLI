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
            "  l4 [key1,key2,key3..]\n"
            "    Comma separated list of keys\n"
            "    Assign L4 back button\n\n"
            "  l4d [time1,time2..]\n"
            "    Comma separated list of times\n"
            "    Set L4 back button keys start time in milliseconds\n\n"
            "  r4 [key1,key2,key3..]\n"
            "    Comma separated list of keys\n"
            "    Assign R4 back button\n\n"
            "  r4d [time1,time2..]\n"
            "    Comma separated list of times\n"
            "    Set R4 back button keys start time in milliseconds\n\n"
            "  rmb [mode]\n"
            "    Set vibration intensity [off, low, high]\n\n"
            "  lc [value]\n"
            "    Adjust left analog deadzone [-10, +10]\n\n"
            "  lb [value]\n"
            "    Adjust left analog boundary [-10, +10]\n\n"
            "  rc [value]\n"
            "    Adjust right analog deadzone [-10, +10]\n\n"
            "  rb [value]\n"
            "    Adjust right analog boundary [-10, +10]\n\n"
            "  led [mode]\n"
            "    Set shoulder buttons led mode [off, solid, breathe, rotate]\n\n"
            "  ledclr [R:G:B]\n"
            "    Set shoulder buttons led color [0-255:0-255:0-255]\n\n"

            "Notes:\n\n"
            "  Controller V1 features:\n"
            "     Support up to 4 key/time slots for back buttons macro.\n"
            "     If more keys/times are provied, they will be ignored.\n"
            "     The 4th time slot is special, it sets the whole macro start time.\n\n"

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
                isArg("r3"))
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

            } else if (isArg("l4d") || isArg("r4d")) {
                std::vector<int> times;
                char *s = strtok(argV[1], ",");

                while (s != nullptr) {
                    times.emplace_back(std::stoi(s));

                    s = strtok(nullptr, ",");
                }

                args.emplace(argV[0], times);

            } else if (isArg("lc") || isArg("lb") || isArg("rc") || isArg("rb")) {
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

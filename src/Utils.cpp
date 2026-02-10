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
#include <fstream>
#include <format>

#include "Utils.h"
#include "extern/libOpenWinControls/src/include/ControllerFeature.h"
#include "extern/libOpenWinControls/src/controller/ControllerV1.h"
#include "extern/yaml-cpp/include/yaml-cpp/yaml.h"

namespace OWCL {
    static constexpr std::array<std::pair<std::string_view, OWC::Button>, 21> kbmKeys = {{
        {"A", OWC::Button::KBD_A},
        {"B", OWC::Button::KBD_B},
        {"X", OWC::Button::KBD_X},
        {"Y", OWC::Button::KBD_Y},
        {"DPAD_UP", OWC::Button::KBD_DPAD_UP},
        {"DPAD_DOWN", OWC::Button::KBD_DPAD_DOWN},
        {"DPAD_LEFT", OWC::Button::KBD_DPAD_LEFT},
        {"DPAD_RIGHT", OWC::Button::KBD_DPAD_RIGHT},
        {"L_ANALOG_UP", OWC::Button::KBD_LANALOG_UP},
        {"L_ANALOG_DOWN", OWC::Button::KBD_LANALOG_DOWN},
        {"L_ANALOG_LEFT", OWC::Button::KBD_LANALOG_LEFT},
        {"L_ANALOG_RIGHT", OWC::Button::KBD_LANALOG_RIGHT},
        {"L1", OWC::Button::KBD_L1},
        {"L2", OWC::Button::KBD_L2},
        {"L3", OWC::Button::KBD_L3},
        {"R1", OWC::Button::KBD_R1},
        {"R2", OWC::Button::KBD_R2},
        {"R3", OWC::Button::KBD_R3},
        {"START", OWC::Button::KBD_START},
        {"SELECT", OWC::Button::KBD_SELECT},
        {"MENU", OWC::Button::KBD_MENU}
    }};

    static void printControllerVersion(const std::shared_ptr<OWC::Controller> &gpd) {
        if (gpd->getControllerType() == 1) {
            const std::shared_ptr<OWC::ControllerV1> gpdV1 = std::dynamic_pointer_cast<OWC::ControllerV1>(gpd);
            const auto &[xmin, xmaj] = gpdV1->getXVersion();
            const auto &[kmin, kmaj] = gpdV1->getKVersion();

            std::cout << "=== Controller Firmware Version ===\n\n" <<
                "Xinput Mode:\t\t" << xmin << "." << xmaj << "\n" <<
                "Keyboard&Mouse Mode:\t" << kmin << "." << kmaj << "\n";
        }
    }

    static void printKeyboardMouseMapping(const std::shared_ptr<OWC::Controller> &gpd) {
        std::cout << "\n=== Keyboard&Mouse Mapping ===\n\n" <<
            "DPAD Up:\t\t" << gpd->getButton(OWC::Button::KBD_DPAD_UP) << "\n"
            "DPAD Down:\t\t" << gpd->getButton(OWC::Button::KBD_DPAD_DOWN) << "\n"
            "DPAD Left:\t\t" << gpd->getButton(OWC::Button::KBD_DPAD_LEFT) << "\n"
            "DPAD Right:\t\t" << gpd->getButton(OWC::Button::KBD_DPAD_RIGHT) << "\n"
            "A:\t\t\t" << gpd->getButton(OWC::Button::KBD_A) << "\n"
            "B:\t\t\t" << gpd->getButton(OWC::Button::KBD_B) << "\n"
            "X:\t\t\t" << gpd->getButton(OWC::Button::KBD_X) << "\n"
            "Y:\t\t\t" << gpd->getButton(OWC::Button::KBD_Y) << "\n"
            "Start:\t\t\t" << gpd->getButton(OWC::Button::KBD_START) << "\n"
            "Select:\t\t\t" << gpd->getButton(OWC::Button::KBD_SELECT) << "\n"
            "Menu:\t\t\t" << gpd->getButton(OWC::Button::KBD_MENU) << "\n"
            "Left Analog Up:\t\t" << gpd->getButton(OWC::Button::KBD_LANALOG_UP) << "\n"
            "Left Analog Down:\t" << gpd->getButton(OWC::Button::KBD_LANALOG_DOWN) << "\n"
            "Left Analog Left:\t" << gpd->getButton(OWC::Button::KBD_LANALOG_LEFT) << "\n"
            "Left Analog Right:\t" << gpd->getButton(OWC::Button::KBD_LANALOG_RIGHT) << "\n"
            "L1:\t\t\t" << gpd->getButton(OWC::Button::KBD_L1) << "\n"
            "L2:\t\t\t" << gpd->getButton(OWC::Button::KBD_L2) << "\n"
            "L3:\t\t\t" << gpd->getButton(OWC::Button::KBD_L3) << "\n"
            "R1:\t\t\t" << gpd->getButton(OWC::Button::KBD_R1) << "\n"
            "R2:\t\t\t" << gpd->getButton(OWC::Button::KBD_R2) << "\n"
            "R3:\t\t\t" << gpd->getButton(OWC::Button::KBD_R3) << "\n";
    }

    static void printBackButtonsV1(const std::shared_ptr<OWC::Controller> &gpd) {
        std::cout << "\n=== L4 Back Button Macro ===\n\n"

            "Key 1:\t\t\t" << gpd->getBackButton(1, 1) << "\n"
            "Key 1 Start Time:\t" << gpd->getBackButtonStartTime(1, 1) << "\n"
            "Key 2:\t\t\t" << gpd->getBackButton(1, 2) << "\n"
            "Key 2 Start Time:\t" << gpd->getBackButtonStartTime(1, 2) << "\n"
            "Key 3:\t\t\t" << gpd->getBackButton(1, 3) << "\n"
            "Key 3 Start Time:\t" << gpd->getBackButtonStartTime(1, 3) << "\n"
            "Key 4:\t\t\t" << gpd->getBackButton(1, 4) << "\n"
            "Macro Start Time:\t" << gpd->getBackButtonStartTime(1, 4) << "\n"

            "\n=== R4 Back Button Macro ===\n\n"

            "Key 1:\t\t\t" << gpd->getBackButton(2, 1) << "\n"
            "Key 1 Start Time:\t" << gpd->getBackButtonStartTime(2, 1) << "\n"
            "Key 2:\t\t\t" << gpd->getBackButton(2, 2) << "\n"
            "Key 2 Start Time:\t" << gpd->getBackButtonStartTime(2, 2) << "\n"
            "Key 3:\t\t\t" << gpd->getBackButton(2, 3) << "\n"
            "Key 3 Start Time:\t" << gpd->getBackButtonStartTime(2, 3) << "\n"
            "Key 4:\t\t\t" << gpd->getBackButton(2, 4) << "\n"
            "Macro Start Time:\t" << gpd->getBackButtonStartTime(2, 4) << "\n";
    }

    void printCurrentSettings(const std::shared_ptr<OWC::Controller> &gpd) {
        printControllerVersion(gpd);
        printKeyboardMouseMapping(gpd);

        if (gpd->getControllerType() == 1)
            printBackButtonsV1(gpd);

        if (gpd->hasFeature(OWC::ControllerFeature::RumbleV1)) {
            std::cout << "\n=== Rumble ===\n\n"
                "Vibration intensity:\t" << gpd->getRumbleMode() << "\n";
        }

        if (gpd->hasFeature(OWC::ControllerFeature::DeadZoneControlV1)) {
            std::cout << "\n=== Calibration/Deadzone ===\n\n"
                "Left Analog deadzone:\t" << gpd->getAnalogCenter(true) << "\n"
                "Left Analog boundary:\t" << gpd->getAnalogBoundary(true) << "\n"
                "Right Analog deadzone:\t" << gpd->getAnalogCenter(false) << "\n"
                "Right Analog boundary:\t" << gpd->getAnalogBoundary(false) << "\n";
        }

        if (gpd->hasFeature(OWC::ControllerFeature::ShoulderLedsV1)) {
            const std::string mode = gpd->getLedMode();

            std::cout << "\n=== Shoulder LEDs ===\n\n"
                "Mode:\t\t\t" << mode << "\n";

            if (mode != "off" && mode != "rotate") {
                const std::tuple<int, int, int> color = gpd->getLedColor();

                std::cout << "Color:\t\t\t"
                    "R(" << std::get<0>(color) << "), "
                    "G(" << std::get<1>(color) << "), "
                    "B(" << std::get<2>(color) << ")\n";
            }
        }
    }

    static void exportBackButtonsV1Yaml(const std::shared_ptr<OWC::Controller> &gpd, std::ofstream &ofs) {
        ofs << "L4_K1: " << gpd->getBackButton(1, 1) << "\n"
            "L4_K1_START_TIME: " << gpd->getBackButtonStartTime(1, 1) << "\n"
            "L4_K2: " << gpd->getBackButton(1, 2) << "\n"
            "L4_K2_START_TIME: " << gpd->getBackButtonStartTime(1, 2) << "\n"
            "L4_K3: " << gpd->getBackButton(1, 3) << "\n"
            "L4_K3_START_TIME: " << gpd->getBackButtonStartTime(1, 3) << "\n"
            "L4_K4: " << gpd->getBackButton(1, 4) << "\n"
            "L4_MACRO_START_TIME: " << gpd->getBackButtonStartTime(1, 4) << "\n"

            "R4_K1: " << gpd->getBackButton(2, 1) << "\n"
            "R4_K1_START_TIME: " << gpd->getBackButtonStartTime(2, 1) << "\n"
            "R4_K2: " << gpd->getBackButton(2, 2) << "\n"
            "R4_K2_START_TIME: " << gpd->getBackButtonStartTime(2, 2) << "\n"
            "R4_K3: " << gpd->getBackButton(2, 3) << "\n"
            "R4_K3_START_TIME: " << gpd->getBackButtonStartTime(2, 3) << "\n"
            "R4_K4: " << gpd->getBackButton(2, 4) << "\n"
            "R4_MACRO_START_TIME: " << gpd->getBackButtonStartTime(2, 4) << "\n";
    }

    int exportToYaml(const std::shared_ptr<OWC::Controller> &gpd, const std::string &fileName) {
        std::ofstream yaml (fileName);

        if (!yaml.is_open()) {
            std::cerr << "failed to open " << fileName <<" for write\n";
            return 1;
        }

        yaml << "MAPPING_TYPE: " << gpd->getControllerType() << "\n";

        // keyboard&mouse mapping
        for (const auto &[key, btn]: kbmKeys)
            yaml << std::format("{}: ", key) << gpd->getButton(btn) << "\n";

        if (gpd->getControllerType() == 1)
            exportBackButtonsV1Yaml(gpd, yaml);

        yaml.close();
        std::cout << "exported config to " << fileName << "\n";
        return 0;
    }

    static void importBackButtonsV1Yaml(const std::shared_ptr<OWC::Controller> &gpd, const YAML::Node &yaml) {
        for (int i=1; i<=4; ++i) {
            const std::string btnlK = std::format("L4_K{}", i);
            const std::string btnrK = std::format("R4_K{}", i);

            if (yaml[btnlK] && !gpd->setBackButton(1, i, yaml[btnlK].as<std::string>()))
                std::cerr << "failed to set " << btnlK << "\n";

            if (yaml[btnrK] && !gpd->setBackButton(2, i, yaml[btnrK].as<std::string>()))
                std::cerr << "failed to set " << btnrK << "\n";
        }

        if (yaml["L4_K1_START_TIME"])
            gpd->setBackButtonStartTime(1, 1, yaml["L4_K1_START_TIME"].as<int>());

        if (yaml["L4_K2_START_TIME"])
            gpd->setBackButtonStartTime(1, 2, yaml["L4_K2_START_TIME"].as<int>());

        if (yaml["L4_K3_START_TIME"])
            gpd->setBackButtonStartTime(1, 3, yaml["L4_K3_START_TIME"].as<int>());

        if (yaml["L4_MACRO_START_TIME"])
            gpd->setBackButtonStartTime(1, 4, yaml["L4_MACRO_START_TIME"].as<int>());

        if (yaml["R4_K1_START_TIME"])
            gpd->setBackButtonStartTime(2, 1, yaml["R4_K1_START_TIME"].as<int>());

        if (yaml["R4_K2_START_TIME"])
            gpd->setBackButtonStartTime(2, 2, yaml["R4_K2_START_TIME"].as<int>());

        if (yaml["R4_K3_START_TIME"])
            gpd->setBackButtonStartTime(2, 3, yaml["R4_K3_START_TIME"].as<int>());

        if (yaml["R4_MACRO_START_TIME"])
            gpd->setBackButtonStartTime(2, 4, yaml["R4_MACRO_START_TIME"].as<int>());
    }

    int importFromYaml(const std::shared_ptr<OWC::Controller> &gpd, const std::string &fileName) {
        const YAML::Node yaml = YAML::LoadFile(fileName);

        if (!yaml.IsMap()) {
            std::cerr << "invalid yaml file\n";
            return 1;
        }

        if (!yaml["MAPPING_TYPE"]) {
            std::cerr << "mapping type missing, cannot apply mapping\n";
            return 1;

        } else if (yaml["MAPPING_TYPE"].as<int>() != gpd->getControllerType()) {
            std::cerr << "wrong mapping type for this controller, cannot apply\n";
            return 1;
        }

        // keyboard&mouse mapping
        for (const auto &[key, btn]: kbmKeys) {
            if (yaml[key] && !gpd->setButton(btn, yaml[key].as<std::string>()))
                std::cerr << "failed to set " << key << "\n";
        }

        if (gpd->getControllerType() == 1)
            importBackButtonsV1Yaml(gpd, yaml);

        if (!gpd->writeConfig()) {
            std::cerr << "failed to write controller\n";
            return 1;
        }

        std::cout << "applied config from " << fileName << "\n";
        return 0;
    }

    static void writeConfigBackButtonsV1(const std::shared_ptr<OWC::Controller> &gpd, const OWC::CMDParser &cmd) {
        if (cmd.hasArg("l4")) {
            const std::vector<std::string> keys = std::get<std::vector<std::string>>(cmd.getValue("l4"));

            for (int i=0,l=keys.size(); i<l && i<4; ++i) {
                if (!gpd->setBackButton(1, i+1, keys[i]))
                    std::cerr << "failed to set L4 slot " << i << "\n";
            }
        }

        if (cmd.hasArg("r4")) {
            const std::vector<std::string> keys = std::get<std::vector<std::string>>(cmd.getValue("r4"));

            for (int i=0,l=keys.size(); i<l && i<4; ++i) {
                if (!gpd->setBackButton(2, i+1, keys[i]))
                    std::cerr << "failed to set R4 slot " << i << "\n";
            }
        }

        if (cmd.hasArg("l4d")) {
            const std::vector<int> times = std::get<std::vector<int>>(cmd.getValue("l4d"));

            for (int i=0,l=times.size(); i<l && i<4; ++i)
                gpd->setBackButtonStartTime(1, i+1, times[i]);
        }

        if (cmd.hasArg("r4d")) {
            const std::vector<int> times = std::get<std::vector<int>>(cmd.getValue("r4d"));

            for (int i=0,l=times.size(); i<l && i<4; ++i)
                gpd->setBackButtonStartTime(2, i+1, times[i]);
        }
    }

    int writeConfig(const std::shared_ptr<OWC::Controller> &gpd, const OWC::CMDParser &cmd) {
        if (cmd.hasArg("du") && !gpd->setButton(OWC::Button::KBD_DPAD_UP, std::get<std::string>(cmd.getValue("du"))))
            std::cerr << "failed to set dpad up\n";

        if (cmd.hasArg("dd") && !gpd->setButton(OWC::Button::KBD_DPAD_DOWN, std::get<std::string>(cmd.getValue("dd"))))
            std::cerr << "failed to set dpad down\n";

        if (cmd.hasArg("dl") && !gpd->setButton(OWC::Button::KBD_DPAD_LEFT, std::get<std::string>(cmd.getValue("dl"))))
            std::cerr << "failed to set dpad left\n";

        if (cmd.hasArg("dr") && !gpd->setButton(OWC::Button::KBD_DPAD_RIGHT, std::get<std::string>(cmd.getValue("dr"))))
            std::cerr << "failed to set dpad right\n";

        if (cmd.hasArg("a") && !gpd->setButton(OWC::Button::KBD_A, std::get<std::string>(cmd.getValue("a"))))
            std::cerr << "failed to set A\n";

        if (cmd.hasArg("b") && !gpd->setButton(OWC::Button::KBD_B, std::get<std::string>(cmd.getValue("b"))))
            std::cerr << "failed to set B\n";

        if (cmd.hasArg("x") && !gpd->setButton(OWC::Button::KBD_X, std::get<std::string>(cmd.getValue("x"))))
            std::cerr << "failed to set X\n";

        if (cmd.hasArg("y") && !gpd->setButton(OWC::Button::KBD_Y, std::get<std::string>(cmd.getValue("y"))))
            std::cerr << "failed to set Y\n";

        if (cmd.hasArg("lu") && !gpd->setButton(OWC::Button::KBD_LANALOG_UP, std::get<std::string>(cmd.getValue("lu"))))
            std::cerr << "failed to set left analog up\n";

        if (cmd.hasArg("ld") && !gpd->setButton(OWC::Button::KBD_LANALOG_DOWN, std::get<std::string>(cmd.getValue("ld"))))
            std::cerr << "failed to set left analog down\n";

        if (cmd.hasArg("ll") && !gpd->setButton(OWC::Button::KBD_LANALOG_LEFT, std::get<std::string>(cmd.getValue("ll"))))
            std::cerr << "failed to set left analog left\n";

        if (cmd.hasArg("lr") && !gpd->setButton(OWC::Button::KBD_LANALOG_RIGHT, std::get<std::string>(cmd.getValue("lr"))))
            std::cerr << "failed to set left analog right\n";

        if (cmd.hasArg("st") && !gpd->setButton(OWC::Button::KBD_START, std::get<std::string>(cmd.getValue("st"))))
            std::cerr << "failed to set start\n";

        if (cmd.hasArg("sl") && !gpd->setButton(OWC::Button::KBD_SELECT, std::get<std::string>(cmd.getValue("sl"))))
            std::cerr << "failed to set select\n";

        if (cmd.hasArg("mu") && !gpd->setButton(OWC::Button::KBD_MENU, std::get<std::string>(cmd.getValue("mu"))))
            std::cerr << "failed to set menu\n";

        if (cmd.hasArg("l1") && !gpd->setButton(OWC::Button::KBD_L1, std::get<std::string>(cmd.getValue("l1"))))
            std::cerr << "failed to set L1\n";

        if (cmd.hasArg("l2") && !gpd->setButton(OWC::Button::KBD_L2, std::get<std::string>(cmd.getValue("l2"))))
            std::cerr << "failed to set L2\n";

        if (cmd.hasArg("l3") && !gpd->setButton(OWC::Button::KBD_L3, std::get<std::string>(cmd.getValue("l3"))))
            std::cerr << "failed to set L3\n";

        if (cmd.hasArg("r1") && !gpd->setButton(OWC::Button::KBD_R1, std::get<std::string>(cmd.getValue("r1"))))
            std::cerr << "failed to set R1\n";

        if (cmd.hasArg("r2") && !gpd->setButton(OWC::Button::KBD_R2, std::get<std::string>(cmd.getValue("r2"))))
            std::cerr << "failed to set R2\n";

        if (cmd.hasArg("r3") && !gpd->setButton(OWC::Button::KBD_R3, std::get<std::string>(cmd.getValue("r3"))))
            std::cerr << "failed to set R3\n";

        if (gpd->getControllerType() == 1)
            writeConfigBackButtonsV1(gpd, cmd);

        if (gpd->hasFeature(OWC::ControllerFeature::RumbleV1) && cmd.hasArg("rmb"))
            gpd->setRumble(std::get<std::string>(cmd.getValue("rmb")));

        if (gpd->hasFeature(OWC::ControllerFeature::DeadZoneControlV1)) {
            if (cmd.hasArg("lc"))
                gpd->setAnalogCenter(std::get<int>(cmd.getValue("lc")), true);

            if (cmd.hasArg("lb"))
                gpd->setAnalogBoundary(std::get<int>(cmd.getValue("lb")), true);

            if (cmd.hasArg("rc"))
                gpd->setAnalogCenter(std::get<int>(cmd.getValue("rc")), false);

            if (cmd.hasArg("rb"))
                gpd->setAnalogBoundary(std::get<int>(cmd.getValue("rb")), false);
        }

        if (gpd->hasFeature(OWC::ControllerFeature::ShoulderLedsV1)) {
            if (cmd.hasArg("led"))
                gpd->setLedMode(std::get<std::string>(cmd.getValue("led")));

            if (cmd.hasArg("ledclr")) {
                const std::tuple<int, int, int> color = std::get<std::tuple<int, int, int>>(cmd.getValue("ledclr"));

                gpd->setLedColor(std::get<0>(color), std::get<1>(color), std::get<2>(color));
            }
        }

        if (!gpd->writeConfig()) {
            std::cerr << "failed to write controller\n";
            return 1;
        }

        return 0;
    }
}

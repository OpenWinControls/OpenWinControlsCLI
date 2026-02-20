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
#include "extern/libOpenWinControls/src/Utils.h"
#include "extern/libOpenWinControls/src/include/ControllerFeature.h"
#include "extern/libOpenWinControls/src/controller/ControllerV1.h"
#include "extern/libOpenWinControls/src/controller/ControllerV2.h"
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

    static constexpr std::array<std::pair<std::string_view, OWC::Button>, 25> xinptKeys = {{
        {"X_A", OWC::Button::X_A},
        {"X_B", OWC::Button::X_B},
        {"X_X", OWC::Button::X_X},
        {"X_Y", OWC::Button::X_Y},
        {"X_DPAD_UP", OWC::Button::X_DPAD_UP},
        {"X_DPAD_DOWN", OWC::Button::X_DPAD_DOWN},
        {"X_DPAD_LEFT", OWC::Button::X_DPAD_LEFT},
        {"X_DPAD_RIGHT", OWC::Button::X_DPAD_RIGHT},
        {"X_L_ANALOG_UP", OWC::Button::X_LANALOG_UP},
        {"X_L_ANALOG_DOWN", OWC::Button::X_LANALOG_DOWN},
        {"X_L_ANALOG_LEFT", OWC::Button::X_LANALOG_LEFT},
        {"X_L_ANALOG_RIGHT", OWC::Button::X_LANALOG_RIGHT},
        {"X_R_ANALOG_UP", OWC::Button::X_RANALOG_UP},
        {"X_R_ANALOG_DOWN", OWC::Button::X_RANALOG_DOWN},
        {"X_R_ANALOG_LEFT", OWC::Button::X_RANALOG_LEFT},
        {"X_R_ANALOG_RIGHT", OWC::Button::X_RANALOG_RIGHT},
        {"X_L1", OWC::Button::X_L1},
        {"X_L2", OWC::Button::X_L2},
        {"X_L3", OWC::Button::X_L3},
        {"X_R1", OWC::Button::X_R1},
        {"X_R2", OWC::Button::X_R2},
        {"X_R3", OWC::Button::X_R3},
        {"X_START", OWC::Button::X_START},
        {"X_SELECT", OWC::Button::X_SELECT},
        {"X_MENU", OWC::Button::X_MENU}
    }};

    static constexpr std::array<std::tuple<std::string_view, OWC::Button, std::string_view>, 21> keyArgs = {{
        {"du", OWC::Button::KBD_DPAD_UP, "dpad up"},
        {"dd", OWC::Button::KBD_DPAD_DOWN, "dpad down"},
        {"dl", OWC::Button::KBD_DPAD_LEFT, "dpad left"},
        {"dr", OWC::Button::KBD_DPAD_RIGHT, "dpad right"},
        {"a",  OWC::Button::KBD_A, "A"},
        {"b",  OWC::Button::KBD_B, "B"},
        {"x",  OWC::Button::KBD_X, "X"},
        {"y",  OWC::Button::KBD_Y, "Y"},
        {"lu", OWC::Button::KBD_LANALOG_UP, "left analog up"},
        {"ld", OWC::Button::KBD_LANALOG_DOWN, "left analog down"},
        {"ll", OWC::Button::KBD_LANALOG_LEFT, "left analog left"},
        {"lr", OWC::Button::KBD_LANALOG_RIGHT, "left analog right"},
        {"st", OWC::Button::KBD_START, "start"},
        {"sl", OWC::Button::KBD_SELECT, "select"},
        {"mu", OWC::Button::KBD_MENU, "menu"},
        {"l1", OWC::Button::KBD_L1, "L1"},
        {"l2", OWC::Button::KBD_L2, "L2"},
        {"l3", OWC::Button::KBD_L3, "L3"},
        {"r1", OWC::Button::KBD_R1, "R1"},
        {"r2", OWC::Button::KBD_R2, "R2"},
        {"r3", OWC::Button::KBD_R3, "R3"}
    }};

    static constexpr std::array<std::tuple<std::string_view, OWC::Button, std::string_view>, 25> xkeyArgs = {{
        {"xdu", OWC::Button::X_DPAD_UP, "xinput dpad up"},
        {"xdd", OWC::Button::X_DPAD_DOWN, "xinput dpad down"},
        {"xdl", OWC::Button::X_DPAD_LEFT, "xinput dpad left"},
        {"xdr", OWC::Button::X_DPAD_RIGHT, "xinput dpad right"},
        {"xa",  OWC::Button::X_A, "xinput A"},
        {"xb",  OWC::Button::X_B, "xinput B"},
        {"xx",  OWC::Button::X_X, "xinput X"},
        {"xy",  OWC::Button::X_Y, "xinput Y"},
        {"xlu", OWC::Button::X_LANALOG_UP, "xinput left analog up"},
        {"xld", OWC::Button::X_LANALOG_DOWN, "xinput left analog down"},
        {"xll", OWC::Button::X_LANALOG_LEFT, "xinput left analog left"},
        {"xlr", OWC::Button::X_LANALOG_RIGHT, "xinput left analog right"},
        {"xru", OWC::Button::X_RANALOG_UP, "xinput right analog up"},
        {"xrd", OWC::Button::X_RANALOG_DOWN, "xinput right analog down"},
        {"xrl", OWC::Button::X_RANALOG_LEFT, "xinput right analog left"},
        {"xrr", OWC::Button::X_RANALOG_RIGHT, "xinput right analog right"},
        {"xst", OWC::Button::X_START, "xinput start"},
        {"xsl", OWC::Button::X_SELECT, "xinput select"},
        {"xmu", OWC::Button::X_MENU, "xinput menu"},
        {"xl1", OWC::Button::X_L1, "xinput L1"},
        {"xl2", OWC::Button::X_L2, "xinput L2"},
        {"xl3", OWC::Button::X_L3, "xinput L3"},
        {"xr1", OWC::Button::X_R1, "xinput R1"},
        {"xr2", OWC::Button::X_R2, "xinput R2"},
        {"xr3", OWC::Button::X_R3, "xinput R3"}
    }};

    static void printControllerInfoV1(const std::shared_ptr<OWC::ControllerV1> &gpd) {
        const auto [xmaj, xmin] = gpd->getXVersion();
        const auto [kmaj, kmin] = gpd->getKVersion();

        std::cout << "=== Controller V1 Info ===\n\n"
            "Xinput Version:\t\t" << xmaj << "." << xmin << "\n" <<
            "Keyboard&Mouse Version:\t" << kmaj << "." << kmin << "\n";
    }

    static void printControllerInfoV2(const std::shared_ptr<OWC::ControllerV2> &gpd) {
        const auto [major, minor] = gpd->getVersion();

        std::cout << "=== Controller V2 Info ===\n\n"
            "Version:\t\t" << major << "." << minor << "\n"
            "Emulation Mode:\t\t" << OWC::emulationModeToString(gpd->getEmulationMode()) << "\n";
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

    static void printXinputMapping(const std::shared_ptr<OWC::Controller> &gpd) {
        if (!gpd->hasFeature(OWC::ControllerFeature::XinputMappingV1))
            return;

        std::cout << "\n=== Xinput Mapping ===\n\n" <<
            "DPAD Up:\t\t" << gpd->getButton(OWC::Button::X_DPAD_UP) << "\n"
            "DPAD Down:\t\t" << gpd->getButton(OWC::Button::X_DPAD_DOWN) << "\n"
            "DPAD Left:\t\t" << gpd->getButton(OWC::Button::X_DPAD_LEFT) << "\n"
            "DPAD Right:\t\t" << gpd->getButton(OWC::Button::X_DPAD_RIGHT) << "\n"
            "A:\t\t\t" << gpd->getButton(OWC::Button::X_A) << "\n"
            "B:\t\t\t" << gpd->getButton(OWC::Button::X_B) << "\n"
            "X:\t\t\t" << gpd->getButton(OWC::Button::X_X) << "\n"
            "Y:\t\t\t" << gpd->getButton(OWC::Button::X_Y) << "\n"
            "Start:\t\t\t" << gpd->getButton(OWC::Button::X_START) << "\n"
            "Select:\t\t\t" << gpd->getButton(OWC::Button::X_SELECT) << "\n"
            "Menu:\t\t\t" << gpd->getButton(OWC::Button::X_MENU) << "\n"
            "Left Analog Up:\t\t" << gpd->getButton(OWC::Button::X_LANALOG_UP) << "\n"
            "Left Analog Down:\t" << gpd->getButton(OWC::Button::X_LANALOG_DOWN) << "\n"
            "Left Analog Left:\t" << gpd->getButton(OWC::Button::X_LANALOG_LEFT) << "\n"
            "Left Analog Right:\t" << gpd->getButton(OWC::Button::X_LANALOG_RIGHT) << "\n"
            "Right Analog Up:\t" << gpd->getButton(OWC::Button::X_RANALOG_UP) << "\n"
            "Right Analog Down:\t" << gpd->getButton(OWC::Button::X_RANALOG_DOWN) << "\n"
            "Right Analog Left:\t" << gpd->getButton(OWC::Button::X_RANALOG_LEFT) << "\n"
            "Right Analog Right:\t" << gpd->getButton(OWC::Button::X_RANALOG_RIGHT) << "\n"
            "L1:\t\t\t" << gpd->getButton(OWC::Button::X_L1) << "\n"
            "L2:\t\t\t" << gpd->getButton(OWC::Button::X_L2) << "\n"
            "L3:\t\t\t" << gpd->getButton(OWC::Button::X_L3) << "\n"
            "R1:\t\t\t" << gpd->getButton(OWC::Button::X_R1) << "\n"
            "R2:\t\t\t" << gpd->getButton(OWC::Button::X_R2) << "\n"
            "R3:\t\t\t" << gpd->getButton(OWC::Button::X_R3) << "\n";
    }

    static void printBackButtonsV1(const std::shared_ptr<OWC::Controller> &gpd) {
        int num = 1;

        for (const std::string_view btn: {"L4", "R4"}) {
            std::cout << "\n=== " << btn << " Back Button Macro ===\n\n"

                "Key 1:\t\t\t" << gpd->getBackButton(num, 1) << "\n"
                "Key 1 Start Time:\t" << gpd->getBackButtonStartTime(num, 1) << "\n"
                "Key 2:\t\t\t" << gpd->getBackButton(num, 2) << "\n"
                "Key 2 Start Time:\t" << gpd->getBackButtonStartTime(num, 2) << "\n"
                "Key 3:\t\t\t" << gpd->getBackButton(num, 3) << "\n"
                "Key 3 Start Time:\t" << gpd->getBackButtonStartTime(num, 3) << "\n"
                "Key 4:\t\t\t" << gpd->getBackButton(num, 4) << "\n"
                "Macro Start Time:\t" << gpd->getBackButtonStartTime(num, 4) << "\n";

            ++num;
        }
    }

    static void printBackButtonsV2(const std::shared_ptr<OWC::ControllerV2> &gpd) {
        int num = 1;

        for (const std::string_view btn: {"L4", "R4"}) {
            std::cout << "\n=== " << btn << " Back Button ===\n\n"

                "Button Mode:\t" << OWC::backButtonModeToString(gpd->getBackButtonMode(num)) << "\n"
                //"Macro type:\t" << OWC::backButtonMacroTypeToString(gpd->getBackButtonMacroType(num)) << "\n"
                "Active slots:\t" << gpd->getBackButtonActiveSlots(num) << "\n\n";

            for (int i=1; i<=32; ++i) {
                std::cout << "Key " << i << ":\t\t\t" << gpd->getBackButton(num, i) << "\n"
                    "Key " << i << " Start Time:\t" << gpd->getBackButtonStartTime(num, i) << "\n"
                    "Key " << i << " Hold Time:\t" << gpd->getBackButtonHoldTime(num, i) << "\n";
            }

            ++num;
        }
    }

    static void printRumbleV1(const std::shared_ptr<OWC::Controller> &gpd) {
        if (!gpd->hasFeature(OWC::ControllerFeature::RumbleV1))
            return;

        std::cout << "\n=== Rumble ===\n\n"
            "Vibration intensity:\t" << OWC::rumbleModeToString(gpd->getRumbleMode()) << "\n";
    }

    static void printDeadzoneControlV1(const std::shared_ptr<OWC::Controller> &gpd) {
        if (!gpd->hasFeature(OWC::ControllerFeature::DeadZoneControlV1))
            return;

        std::cout << "\n=== Calibration/Deadzone ===\n\n"
            "Left Analog deadzone:\t" << gpd->getAnalogCenter(true) << "\n"
            "Left Analog boundary:\t" << gpd->getAnalogBoundary(true) << "\n"
            "Right Analog deadzone:\t" << gpd->getAnalogCenter(false) << "\n"
            "Right Analog boundary:\t" << gpd->getAnalogBoundary(false) << "\n";
    }

    static void printShoulderLedsV1(const std::shared_ptr<OWC::Controller> &gpd) {
        if (!gpd->hasFeature(OWC::ControllerFeature::ShoulderLedsV1))
            return;

        const OWC::LedMode mode = gpd->getLedMode();

        std::cout << "\n=== Shoulder LEDs ===\n\n"
            "Mode:\t\t\t" << OWC::ledModeToString(mode) << "\n";

        if (mode != OWC::LedMode::Off && mode != OWC::LedMode::Rotate) {
            const std::tuple<int, int, int> color = gpd->getLedColor();

            std::cout << "Color:\t\t\t"
                "R(" << std::get<0>(color) << "), "
                "G(" << std::get<1>(color) << "), "
                "B(" << std::get<2>(color) << ")\n";
        }
    }

    void printCurrentSettings(const std::shared_ptr<OWC::Controller> &gpd) {
        const int controllerType = gpd->getControllerType();

        if (controllerType == 1) {
            const std::shared_ptr<OWC::ControllerV1> gpdV1 = std::dynamic_pointer_cast<OWC::ControllerV1>(gpd);

            printControllerInfoV1(gpdV1);
            printKeyboardMouseMapping(gpd);
            printBackButtonsV1(gpd);

        } else if (controllerType == 2) {
            const std::shared_ptr<OWC::ControllerV2> gpdV2 = std::dynamic_pointer_cast<OWC::ControllerV2>(gpd);

            printControllerInfoV2(gpdV2);
            printKeyboardMouseMapping(gpd);
            printXinputMapping(gpd);
            printBackButtonsV2(gpdV2);
        }

        printRumbleV1(gpd);
        printDeadzoneControlV1(gpd);
        printShoulderLedsV1(gpd);
    }

    static void exportBackButtonsV1Yaml(const std::shared_ptr<OWC::Controller> &gpd, std::ofstream &ofs) {
        int num = 1;

        for (const std::string_view btn: {"L4", "R4"}) {
            for (int i=1; i<=4; ++i) {
                ofs << btn << "_K" << i << ": " << gpd->getBackButton(num, i) << "\n";

                if (i < 4)
                    ofs << btn << "_k" << i << "_START_TIME: " << gpd->getBackButtonStartTime(num, i) << "\n";
            }

            ++num;
        }

        ofs << "L4_MACRO_START_TIME: " << gpd->getBackButtonStartTime(1, 4) << "\n"
            "R4_MACRO_START_TIME: " << gpd->getBackButtonStartTime(2, 4) << "\n";
    }

    static void exportBackButtonsV2Yaml(const std::shared_ptr<OWC::ControllerV2> &gpd, std::ofstream &ofs) {
        int num = 1;

        for (const std::string_view btn: {"L4", "R4"}) {
            for (int i=1; i<=32; ++i) {
                ofs << btn << "_K" << i <<": " << gpd->getBackButton(num, i) << "\n" <<
                    btn << "_K" << i << "_START_TIME: " << gpd->getBackButtonStartTime(num, i) << "\n" <<
                    btn << "_K" << i << "_HOLD_TIME: " << gpd->getBackButtonHoldTime(num, i) << "\n";
            }

            /*ofs << btn << "_MODE: " << static_cast<int>(gpd->getBackButtonMode(num)) << "\n" <<
                btn << "_MACRO_TYPE: " << static_cast<int>(gpd->getBackButtonMacroType(num)) << "\n" <<
                btn << "_ACTIVE_SLOTS: " << gpd->getBackButtonActiveSlots(num) << "\n";*/

            ++num;
        }
    }

    int exportToYaml(const std::shared_ptr<OWC::Controller> &gpd, const std::string &fileName) {
        const int controllerType = gpd->getControllerType();
        std::ofstream yaml (fileName);

        if (!yaml.is_open()) {
            std::cerr << "failed to open " << fileName <<" for write\n";
            return 1;
        }

        yaml << "MAPPING_TYPE: " << controllerType << "\n";

        // keyboard&mouse mapping
        for (const auto &[key, btn]: kbmKeys)
            yaml << std::format("{}: ", key) << gpd->getButton(btn) << "\n";

        if (gpd->hasFeature(OWC::ControllerFeature::XinputMappingV1)) {
            for (const auto &[key, btn]: xinptKeys)
                yaml << std::format("{}: ", key) << gpd->getButton(btn) << "\n";
        }

        if (controllerType == 1) {
            exportBackButtonsV1Yaml(gpd, yaml);

        } else if (controllerType == 2) {
            const std::shared_ptr<OWC::ControllerV2> gpdV2 = std::dynamic_pointer_cast<OWC::ControllerV2>(gpd);

            exportBackButtonsV2Yaml(gpdV2, yaml);
        }

        yaml.close();
        std::cout << "exported config to " << fileName << "\n";
        return 0;
    }

    static void importBackButtonsV1Yaml(const std::shared_ptr<OWC::Controller> &gpd, const YAML::Node &yaml) {
        int num = 1;

        for (const std::string_view btn: {"L4", "R4"}) {
            for (int i=1; i<=4; ++i) {
                const std::string key = std::format("{}_K{}", btn, i);
                std::string keyCode = yaml[key].as<std::string>();

                std::transform(keyCode.begin(), keyCode.end(), keyCode.begin(), ::toupper);

                if (yaml[key] && !gpd->setBackButton(num, i, keyCode))
                    std::cerr << "failed to set " << key << "\n";

                if (i < 4) {
                    const std::string time = std::format("{}_K{}_START_TIME", btn, i);

                    if (yaml[time])
                        gpd->setBackButtonStartTime(num, i, yaml[time].as<int>());
                }
            }

            ++num;
        }

        if (yaml["L4_MACRO_START_TIME"])
            gpd->setBackButtonStartTime(1, 4, yaml["L4_MACRO_START_TIME"].as<int>());

        if (yaml["R4_MACRO_START_TIME"])
            gpd->setBackButtonStartTime(2, 4, yaml["R4_MACRO_START_TIME"].as<int>());
    }

    static void importBackButtonsV2Yaml(const std::shared_ptr<OWC::ControllerV2> &gpd, const YAML::Node &yaml) {
        int num = 1;

        for (const std::string_view btn: {"L4", "R4"}) {
            /*const std::string mode = std::format("{}_MODE", btn);
            const std::string type = std::format("{}_MACRO_TYPE", btn);
            const std::string activeC = std::format("{}_ACTIVE_SLOTS", btn);*/

            for (int i=1; i<=32; ++i) {
                const std::string key = std::format("{}_K{}", btn, i);
                const std::string time = std::format("{}_K{}_START_TIME", btn, i);
                const std::string hold = std::format("{}_K{}_HOLD_TIME", btn, i);
                std::string kc = yaml[key].as<std::string>();

                std::transform(kc.begin(), kc.end(), kc.begin(), ::toupper);

                if (yaml[key] && !gpd->setBackButton(num, i, kc))
                    std::cerr << "failed to set " << key << "\n";

                if (yaml[time])
                    gpd->setBackButtonStartTime(num, i, yaml[time].as<int>());

                if (yaml[hold])
                    gpd->setBackButtonHoldTime(num, i, yaml[hold].as<int>());
            }

            /*if (yaml[mode])
                gpd->setBackButtonMode(num, static_cast<OWC::BackButtonMode>(std::clamp(yaml[mode].as<int>(), 0, 2)));

            if (yaml[type])
                gpd->setBackButtonMacroType(num, static_cast<OWC::BackButtonMacroType>(std::clamp(yaml[type].as<int>(), 0, 1)));

            if (yaml[activeC])
                gpd->setBackButtonActiveSlots(num, yaml[activeC].as<int>());*/

            ++num;
        }
    }

    int importFromYaml(const std::shared_ptr<OWC::Controller> &gpd, const std::string &fileName) {
        const int controllerType = gpd->getControllerType();
        const YAML::Node yaml = YAML::LoadFile(fileName);

        if (!yaml.IsMap()) {
            std::cerr << "invalid yaml file\n";
            return 1;
        }

        if (!yaml["MAPPING_TYPE"]) {
            std::cerr << "mapping type missing, cannot apply mapping\n";
            return 1;

        } else if (yaml["MAPPING_TYPE"].as<int>() != controllerType) {
            std::cerr << "wrong mapping type for this controller, cannot apply\n";
            return 1;
        }

        // keyboard&mouse mapping
        for (const auto &[key, btn]: kbmKeys) {
            if (!yaml[key])
                continue;

            std::string kc = yaml[key].as<std::string>();

            std::transform(kc.begin(), kc.end(), kc.begin(), ::toupper);

            if (!gpd->setButton(btn, kc))
                std::cerr << "failed to set " << key << "\n";
        }

        if (gpd->hasFeature(OWC::ControllerFeature::XinputMappingV1)) {
            for (const auto &[key, btn]: xinptKeys) {
                if (!yaml[key])
                    continue;

                std::string kc = yaml[key].as<std::string>();

                std::transform(kc.begin(), kc.end(), kc.begin(), ::toupper);

                if (!gpd->setButton(btn, kc))
                    std::cerr << "failed to set " << key << "\n";
            }
        }

        if (controllerType == 1) {
            importBackButtonsV1Yaml(gpd, yaml);

        } else if (controllerType == 2) {
            const std::shared_ptr<OWC::ControllerV2> gpdV2 = std::dynamic_pointer_cast<OWC::ControllerV2>(gpd);

            importBackButtonsV2Yaml(gpdV2, yaml);
        }

        if (!gpd->writeConfig()) {
            std::cerr << "failed to write controller\n";
            return 1;
        }

        std::cout << "applied config from " << fileName << "\n";
        return 0;
    }

    static void writeConfigBackButtonsV1(const std::shared_ptr<OWC::Controller> &gpd, const OWC::CMDParser &cmd) {
        int num = 1;

        for (const std::string_view btn: {"l4", "r4"}) {
            if (!cmd.hasArg(btn.data()))
                continue;

            const std::vector<std::string> keys = std::get<std::vector<std::string>>(cmd.getValue(btn.data()));

            for (int i=0,l=keys.size(); i<l && i<4; ++i) {
                if (!gpd->setBackButton(num, i+1, keys[i]))
                    std::cerr << "failed to set " << btn << " slot " << (i + 1) << "\n";
            }

            ++num;
        }

        num = 1;

        for (const std::string_view btn: {"l4d", "r4d"}) {
            if (!cmd.hasArg(btn.data()))
                continue;

            const std::vector<int> times = std::get<std::vector<int>>(cmd.getValue(btn.data()));

            for (int i=0,l=times.size(); i<l && i<4; ++i)
                gpd->setBackButtonStartTime(num, i+1, times[i]);

            ++num;
        }
    }

    static void writeConfigBackButtonsV2(const std::shared_ptr<OWC::ControllerV2> &gpd, const OWC::CMDParser &cmd) {
        int num = 1;

        for (const std::string_view btn: {"l4", "r4"}) {
            if (!cmd.hasArg(btn.data()))
                continue;

            const std::vector<std::string> keys = std::get<std::vector<std::string>>(cmd.getValue(btn.data()));
            int slotsC = 0;

            for (int i=0,l=keys.size(); i<l && i<32; ++i) {
                if (keys[i] != "UNSET")
                    ++slotsC;

                if (!gpd->setBackButton(num, i+1, keys[i]))
                    std::cerr << "failed to set " << btn << " slot " << (i + 1) << "\n";
            }

            /*
            if (keys.size() == 1)
                gpd->setBackButtonMode(num, OWC::BackButtonMode::Single);
            else if (keys.size() <= 4)
                gpd->setBackButtonMode(num, OWC::BackButtonMode::Four);
            else
                gpd->setBackButtonMode(num, OWC::BackButtonMode::Macro);*/

            gpd->setBackButtonActiveSlots(num, std::clamp(slotsC, 4, 32));
            ++num;
        }

        num = 1;

        for (const std::string_view btn: {"l4d", "r4d"}) {
            if (!cmd.hasArg(btn.data()))
                continue;

            const std::vector<int> times = std::get<std::vector<int>>(cmd.getValue(btn.data()));

            for (int i=0,l=times.size(); i<l && i<32; ++i)
                gpd->setBackButtonStartTime(num, i+1, times[i]);

            ++num;
        }

        num = 1;

        for (const std::string_view btn: {"l4h", "r4h"}) {
            if (!cmd.hasArg(btn.data()))
                continue;

            const std::vector<int> times = std::get<std::vector<int>>(cmd.getValue(btn.data()));

            for (int i=0,l=times.size(); i<l && i<32; ++i)
                gpd->setBackButtonHoldTime(num, i+1, times[i]);

            ++num;
        }
    }

    int writeConfig(const std::shared_ptr<OWC::Controller> &gpd, const OWC::CMDParser &cmd) {
        const int controllerType = gpd->getControllerType();

        for (const auto [karg, btn, desc]: keyArgs) {
            if (cmd.hasArg(karg.data()) && !gpd->setButton(btn, std::get<std::string>(cmd.getValue(karg.data()))))
                std::cerr << "failed to set " << desc << "\n";
        }

        if (gpd->hasFeature(OWC::ControllerFeature::XinputMappingV1)) {
            for (const auto [karg, btn, desc]: xkeyArgs) {
                if (cmd.hasArg(karg.data()) && !gpd->setButton(btn, std::get<std::string>(cmd.getValue(karg.data()))))
                    std::cerr << "failed to set " << desc << "\n";
            }
        }

        if (controllerType == 1) {
            writeConfigBackButtonsV1(gpd, cmd);

        } else if (controllerType == 2) {
            const std::shared_ptr<OWC::ControllerV2> gpdV2 = std::dynamic_pointer_cast<OWC::ControllerV2>(gpd);

            writeConfigBackButtonsV2(gpdV2, cmd);
        }

        if (gpd->hasFeature(OWC::ControllerFeature::RumbleV1) && cmd.hasArg("rmb"))
            gpd->setRumble(static_cast<OWC::RumbleMode>(std::clamp(std::get<int>(cmd.getValue("rmb")), 0, 2)));

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
                gpd->setLedMode(static_cast<OWC::LedMode>(std::clamp(std::get<int>(cmd.getValue("led")), 0, 3)));

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

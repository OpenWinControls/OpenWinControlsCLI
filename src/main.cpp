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
#ifdef _WIN32
#include "include/win.h"
#endif
#include <iostream>
#include <fstream>
#include <memory>
#include <format>
#include <cctype>

#include "classes/CMDParser.h"
#include "classes/FileLogger.h"
#include "extern/libOpenWinControls/src/include/ControllerFeature.h"
#include "extern/libOpenWinControls/src/controller/ControllerV1.h"
#include "extern/yaml-cpp/include/yaml-cpp/yaml.h"

static constexpr char win3[] = "G1618-03";
static constexpr char win4[] = "G1618-04";
static constexpr char mini23[] = "G1617-01";
static constexpr char mini24[] = "G1617-02";
static constexpr char mini25[] = "G1617-02-L";
static constexpr char max2_22[] = "G1619-04";
static constexpr char max2_25[] = "G1619-05";
static constexpr char win5[] = "G1618-05";

[[nodiscard]]
static std::string getProduct() {
#ifdef __linux__
    std::ifstream prodDmi;
    std::string prod;

    prodDmi.open("/sys/class/dmi/id/board_name");
    if (!prodDmi.is_open())
        return "";

    std::getline(prodDmi, prod);
    prodDmi.close();

    return prod;
#elif defined(_WIN32)
    DWORD bufSz = 0;
    std::unique_ptr<TCHAR[]> buf;
    LSTATUS ret;
    HKEY rkey;

    ret = RegOpenKeyExA(HKEY_LOCAL_MACHINE, R"(HARDWARE\DESCRIPTION\System\BIOS)", 0, KEY_READ, &rkey);
    if (ret != ERROR_SUCCESS) {
        std::cerr << "failed to open bios subkey, code: " << ret;
        return "";
    }

    ret = RegGetValueA(rkey, nullptr, "BaseBoardProduct", RRF_RT_REG_SZ, nullptr, nullptr, &bufSz);
    if (ret != ERROR_SUCCESS) {
        std::cerr << "failed to read size for reg value, code " << ret;
        RegCloseKey(rkey);
        return "";
    }

    bufSz += sizeof(TCHAR);
    buf = std::make_unique<TCHAR[]>(bufSz);

    ret = RegGetValueA(rkey, nullptr, "BaseBoardProduct", RRF_RT_REG_SZ, nullptr, buf.get(), &bufSz);
    if (ret != ERROR_SUCCESS) {
        std::cerr << "failed to read reg value, code " << ret;
        RegCloseKey(rkey);
        return "";
    }

    RegCloseKey(rkey);
    return std::string(buf.get());
#else
    return "";
#endif
}

[[nodiscard]]
static std::shared_ptr<OWC::Controller> getDevice(const std::string &product) {
    std::shared_ptr<OWC::Controller> device;

    if (product == win4)
        device = std::make_shared<OWC::ControllerV1>(OWC::ControllerFeature::DeadZoneControlV1 | OWC::ControllerFeature::ShoulderLedsV1 | OWC::ControllerFeature::RumbleV1);
    else if (product == mini23 || product == mini24 || product == max2_22 || product == max2_25)
        device = std::make_shared<OWC::ControllerV1>(OWC::ControllerFeature::DeadZoneControlV1 | OWC::ControllerFeature::RumbleV1);
    //else if (product == win3)
    //    device = std::make_shared<OWC::ControllerV1>();
//    else if (product == win5 || product == mini25)
//        device = std::make_shared<OWC::ControllerV2>();
    else
        std::cerr << "unknown device: " << product << "\n";

    return device;
}

[[nodiscard]]
static bool isCompatible(const std::string &product, const std::shared_ptr<OWC::Controller> &gpd) {
    /*if (product == win3) {
        return true;

    } else*/ if (product == win4) {
        const auto [major, minor] = std::dynamic_pointer_cast<OWC::ControllerV1>(gpd)->getKVersion();
        const bool compCheck = major >= 4 && minor >= 7;

        if (!compCheck)
            std::cout << "version " << major << "." << minor << " is not supported\n";

        return compCheck;

    } else if (product == mini23 || product == mini24) {
        const auto [major, minor] = std::dynamic_pointer_cast<OWC::ControllerV1>(gpd)->getKVersion();
        const bool compCheck = major >= 5 && minor >= 3;

        if (!compCheck)
            std::cout << "version " << major << "." << minor << " is not supported\n";

        return compCheck;

    } else if (product == max2_22 || product == max2_25) {
        const auto [major, minor] = std::dynamic_pointer_cast<OWC::ControllerV1>(gpd)->getKVersion();
        const bool compCheck = major >= 1 && minor >= 23;

        if (!compCheck)
            std::cout << "version " << major << "." << minor << " is not supported\n";

        return compCheck;
    }

    return false;
}

[[nodiscard]]
static int printCurrentSettings(const std::shared_ptr<OWC::Controller> &gpd) {
    if (gpd->getControllerType() == 1) {
        const std::shared_ptr<OWC::ControllerV1> gpdV1 = std::dynamic_pointer_cast<OWC::ControllerV1>(gpd);
        const std::pair<int, int> x = gpdV1->getXVersion();
        const std::pair<int, int> k = gpdV1->getKVersion();

        std::cout << "=== Gamepad Firmware Version ===\n\n" <<
            "Xbox Controller:\t" << x.first << "." << x.second << "\n" <<
            "Controller Keyboard:\t" << k.first << "." << k.second << "\n";
    }

    std::cout << "\n=== Mapping ===\n\n" <<
        "DPAD Up:\t\t" << gpd->getDpadUp() << "\n"
        "DPAD Down:\t\t" << gpd->getDpadDown() << "\n"
        "DPAD Left:\t\t" << gpd->getDpadLeft() << "\n"
        "DPAD Right:\t\t" << gpd->getDpadRight() << "\n"
        "A:\t\t\t" << gpd->getA() << "\n"
        "B:\t\t\t" << gpd->getB() << "\n"
        "X:\t\t\t" << gpd->getX() << "\n"
        "Y:\t\t\t" << gpd->getY() << "\n"
        "Start:\t\t\t" << gpd->getStart() << "\n"
        "Select:\t\t\t" << gpd->getSelect() << "\n"
        "Menu:\t\t\t" << gpd->getMenu() << "\n"
        "Left Analog Up:\t\t" << gpd->getLAnalogUp() << "\n"
        "Left Analog Down:\t" << gpd->getLAnalogDown() << "\n"
        "Left Analog Left:\t" << gpd->getLAnalogLeft() << "\n"
        "Left Analog Right:\t" << gpd->getLAnalogRight() << "\n"
        "L1:\t\t\t" << gpd->getL1() << "\n"
        "L2:\t\t\t" << gpd->getL2() << "\n"
        "L3:\t\t\t" << gpd->getL3() << "\n"
        "R1:\t\t\t" << gpd->getR1() << "\n"
        "R2:\t\t\t" << gpd->getR2() << "\n"
        "R3:\t\t\t" << gpd->getR3() << "\n"

        "\n=== Left Back Button Macro ===\n\n"
        "Key 1:\t\t\t" << gpd->getLBack1() << "\n"
        "Key 1 Delay:\t\t" << gpd->getLBack1Delay() << "\n"
        "Key 2:\t\t\t" << gpd->getLBack2() << "\n"
        "Key 2 Delay:\t\t" << gpd->getLBack2Delay() << "\n"
        "Key 3:\t\t\t" << gpd->getLBack3() << "\n"
        "Key 3 Delay:\t\t" << gpd->getLBack3Delay() << "\n"
        "Key 4:\t\t\t" << gpd->getLBack4() << "\n"
        "Key 4 Delay:\t\t" << gpd->getLBack4Delay() << "\n"

        "\n=== Right Back Button Macro ===\n\n"
        "Key 1:\t\t\t" << gpd->getRBack1() << "\n"
        "Key 1 Delay:\t\t" << gpd->getRBack1Delay() << "\n"
        "Key 2:\t\t\t" << gpd->getRBack2() << "\n"
        "Key 2 Delay:\t\t" << gpd->getRBack2Delay() << "\n"
        "Key 3:\t\t\t" << gpd->getRBack3() << "\n"
        "Key 3 Delay:\t\t" << gpd->getRBack3Delay() << "\n"
        "Key 4:\t\t\t" << gpd->getRBack4() << "\n"
        "Key 4 Delay:\t\t" << gpd->getRBack4Delay() << "\n";

    if (gpd->hasFeature(OWC::ControllerFeature::RumbleV1)) {
        std::cout << "\n=== Rumble ===\n\n"
            "Vibration intensity:\t" << gpd->getRumbleMode() << "\n";
    }

    if (gpd->hasFeature(OWC::ControllerFeature::DeadZoneControlV1)) {
        std::cout << "\n=== Calibration/Deadzone ===\n\n"
            "Left Analog deadzone:\t" << gpd->getLAnalogCenter() << "\n"
            "Left Analog boundary:\t" << gpd->getLAnalogBoundary() << "\n"
            "Right Analog deadzone:\t" << gpd->getRAnalogCenter() << "\n"
            "Right Analog boundary:\t" << gpd->getRAnalogBoundary() << "\n";
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

    return 0;
}

[[nodiscard]]
static int exportToYaml(const std::shared_ptr<OWC::Controller> &gpd, const std::string &fileName) {
    std::ofstream yaml (fileName);

    if (!yaml.is_open()) {
        std::cerr << "failed to open " << fileName <<" for write\n";
        return 1;
    }

    yaml << "MAPPING_TYPE: " << gpd->getControllerType() << "\n"
        "A: " << gpd->getA() << "\n"
        "B: " << gpd->getB() << "\n"
        "X: " << gpd->getX() << "\n"
        "Y: " << gpd->getY() << "\n"
        "DPAD_UP: " << gpd->getDpadUp() << "\n"
        "DPAD_DOWN: " << gpd->getDpadDown() << "\n"
        "DPAD_LEFT: " << gpd->getDpadLeft() << "\n"
        "DPAD_RIGHT: " << gpd->getDpadRight() << "\n"
        "L_ANALOG_UP: " << gpd->getLAnalogUp() << "\n"
        "L_ANALOG_DOWN: " << gpd->getLAnalogDown() << "\n"
        "L_ANALOG_LEFT: " << gpd->getLAnalogLeft() << "\n"
        "L_ANALOG_RIGHT: " << gpd->getLAnalogRight() << "\n"
        "L1: " << gpd->getL1() << "\n"
        "L2: " << gpd->getL2() << "\n"
        "L3: " << gpd->getL3() << "\n"
        "R1: " << gpd->getR1() << "\n"
        "R2: " << gpd->getR2() << "\n"
        "R3: " << gpd->getR3() << "\n"
        "START: " << gpd->getStart() << "\n"
        "SELECT: " << gpd->getSelect() << "\n"
        "MENU: " << gpd->getMenu() << "\n"
        "L_BACK1: " << gpd->getLBack1() << "\n"
        "L_BACK1_DELAY: " << gpd->getLBack1Delay() << "\n"
        "L_BACK2: " << gpd->getLBack2() << "\n"
        "L_BACK2_DELAY: " << gpd->getLBack2Delay() << "\n"
        "L_BACK3: " << gpd->getLBack3() << "\n"
        "L_BACK3_DELAY: " << gpd->getLBack3Delay() << "\n"
        "L_BACK4: " << gpd->getLBack4() << "\n"
        "L_BACK4_DELAY: " << gpd->getLBack4Delay() << "\n"
        "R_BACK1: " << gpd->getRBack1() << "\n"
        "R_BACK1_DELAY: " << gpd->getRBack1Delay() << "\n"
        "R_BACK2: " << gpd->getRBack2() << "\n"
        "R_BACK2_DELAY: " << gpd->getRBack2Delay() << "\n"
        "R_BACK3: " << gpd->getRBack3() << "\n"
        "R_BACK3_DELAY: " << gpd->getRBack3Delay() << "\n"
        "R_BACK4: " << gpd->getRBack4() << "\n"
        "R_BACK4_DELAY: " << gpd->getRBack4Delay() << "\n";

    yaml.close();
    std::cout << "exported config to " << fileName << "\n";
    return 0;
}

[[nodiscard]]
static int importFromYaml(const std::shared_ptr<OWC::Controller> &gpd, const std::string &fileName) {
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

    if (yaml["A"] && !gpd->setA(yaml["A"].as<std::string>()))
        std::cerr << "failed to set A\n";

    if (yaml["B"] && !gpd->setB(yaml["B"].as<std::string>()))
        std::cerr << "failed to set B\n";

    if (yaml["X"] && !gpd->setX(yaml["X"].as<std::string>()))
        std::cerr << "failed to set X\n";

    if (yaml["Y"] && !gpd->setY(yaml["Y"].as<std::string>()))
        std::cerr << "failed to set Y\n";

    if (yaml["DPAD_UP"] && !gpd->setDpadUp(yaml["DPAD_UP"].as<std::string>()))
        std::cerr << "failed to set DPAD_UP\n";

    if (yaml["DPAD_DOWN"] && !gpd->setDpadDown(yaml["DPAD_DOWN"].as<std::string>()))
        std::cerr << "failed to set DPAD_DOWN\n";

    if (yaml["DPAD_LEFT"] && !gpd->setDpadLeft(yaml["DPAD_LEFT"].as<std::string>()))
        std::cerr << "failed to set DPAD_LEFT\n";

    if (yaml["DPAD_RIGHT"] && !gpd->setDpadRight(yaml["DPAD_RIGHT"].as<std::string>()))
        std::cerr << "failed to set DPAD_RIGHT\n";

    if (yaml["L_ANALOG_UP"] && !gpd->setLAnalogUp(yaml["L_ANALOG_UP"].as<std::string>()))
        std::cerr << "failed to set L_ANALOG_UP\n";

    if (yaml["L_ANALOG_DOWN"] && !gpd->setLAnalogDown(yaml["L_ANALOG_DOWN"].as<std::string>()))
        std::cerr << "failed to set L_ANALOG_DOWN\n";

    if (yaml["L_ANALOG_LEFT"] && !gpd->setLAnalogLeft(yaml["L_ANALOG_LEFT"].as<std::string>()))
        std::cerr << "failed to set L_ANALOG_LEFT\n";

    if (yaml["L_ANALOG_RIGHT"] && !gpd->setLAnalogRight(yaml["L_ANALOG_RIGHT"].as<std::string>()))
        std::cerr << "failed to set L_ANALOG_RIGHT\n";

    if (yaml["L1"] && !gpd->setL1(yaml["L1"].as<std::string>()))
        std::cerr << "failed to set L1\n";

    if (yaml["L2"] && !gpd->setL2(yaml["L2"].as<std::string>()))
        std::cerr << "failed to set L2\n";

    if (yaml["L3"] && !gpd->setL3(yaml["L3"].as<std::string>()))
        std::cerr << "failed to set L3\n";

    if (yaml["R1"] && !gpd->setR1(yaml["R1"].as<std::string>()))
        std::cerr << "failed to set R1\n";

    if (yaml["R2"] && !gpd->setR2(yaml["R2"].as<std::string>()))
        std::cerr << "failed to set R2\n";

    if (yaml["R3"] && !gpd->setR3(yaml["R3"].as<std::string>()))
        std::cerr << "failed to set R3\n";

    if (yaml["START"] && !gpd->setStart(yaml["START"].as<std::string>()))
        std::cerr << "failed to set START\n";

    if (yaml["SELECT"] && !gpd->setSelect(yaml["SELECT"].as<std::string>()))
        std::cerr << "failed to set SELECT\n";

    if (yaml["MENU"] && !gpd->setMenu(yaml["MENU"].as<std::string>()))
        std::cerr << "failed to set MENU\n";

    if (yaml["L_BACK1"] && !gpd->setLBack1(yaml["L_BACK1"].as<std::string>()))
        std::cerr << "failed to set L_BACK1\n";

    if (yaml["L_BACK2"] && !gpd->setLBack2(yaml["L_BACK2"].as<std::string>()))
        std::cerr << "failed to set L_BACK2\n";

    if (yaml["L_BACK3"] && !gpd->setLBack3(yaml["L_BACK3"].as<std::string>()))
        std::cerr << "failed to set L_BACK3\n";

    if (yaml["L_BACK4"] && !gpd->setLBack4(yaml["L_BACK4"].as<std::string>()))
        std::cerr << "failed to set L_BACK4\n";

    if (yaml["R_BACK1"] && !gpd->setRBack1(yaml["R_BACK1"].as<std::string>()))
        std::cerr << "failed to set R_BACK1\n";

    if (yaml["R_BACK2"] && !gpd->setRBack2(yaml["R_BACK2"].as<std::string>()))
        std::cerr << "failed to set R_BACK2\n";

    if (yaml["R_BACK3"] && !gpd->setRBack3(yaml["R_BACK3"].as<std::string>()))
        std::cerr << "failed to set R_BACK3\n";

    if (yaml["R_BACK4"] && !gpd->setRBack4(yaml["R_BACK4"].as<std::string>()))
        std::cerr << "failed to set R_BACK4\n";

    if (yaml["L_BACK1_DELAY"])
        gpd->setLBack1Delay(yaml["L_BACK1_DELAY"].as<int>());

    if (yaml["L_BACK2_DELAY"])
        gpd->setLBack2Delay(yaml["L_BACK2_DELAY"].as<int>());

    if (yaml["L_BACK3_DELAY"])
        gpd->setLBack3Delay(yaml["L_BACK3_DELAY"].as<int>());

    if (yaml["L_BACK4_DELAY"])
        gpd->setLBack4Delay(yaml["L_BACK4_DELAY"].as<int>());

    if (yaml["R_BACK1_DELAY"])
        gpd->setRBack1Delay(yaml["R_BACK1_DELAY"].as<int>());

    if (yaml["R_BACK2_DELAY"])
        gpd->setRBack2Delay(yaml["R_BACK2_DELAY"].as<int>());

    if (yaml["R_BACK3_DELAY"])
        gpd->setRBack3Delay(yaml["R_BACK3_DELAY"].as<int>());

    if (yaml["R_BACK4_DELAY"])
        gpd->setRBack4Delay(yaml["R_BACK4_DELAY"].as<int>());

    if (!gpd->writeConfig()) {
        std::cerr << "failed to write controller\n";
        return 1;
    }

    std::cout << "applied config from " << fileName << "\n";
    return 0;
}

[[nodiscard]]
static int writeConfig(const std::shared_ptr<OWC::Controller> &gpd, const OWC::CMDParser &cmd) {
    if (cmd.hasArg("du") && !gpd->setDpadUp(std::get<std::string>(cmd.getValue("du"))))
        std::cerr << "failed to set dpad up\n";

    if (cmd.hasArg("dd") && !gpd->setDpadDown(std::get<std::string>(cmd.getValue("dd"))))
        std::cerr << "failed to set dpad down\n";

    if (cmd.hasArg("dl") && !gpd->setDpadLeft(std::get<std::string>(cmd.getValue("dl"))))
        std::cerr << "failed to set dpad left\n";

    if (cmd.hasArg("dr") && !gpd->setDpadRight(std::get<std::string>(cmd.getValue("dr"))))
        std::cerr << "failed to set dpad right\n";

    if (cmd.hasArg("a") && !gpd->setA(std::get<std::string>(cmd.getValue("a"))))
        std::cerr << "failed to set A\n";

    if (cmd.hasArg("b") && !gpd->setB(std::get<std::string>(cmd.getValue("b"))))
        std::cerr << "failed to set B\n";

    if (cmd.hasArg("x") && !gpd->setX(std::get<std::string>(cmd.getValue("x"))))
        std::cerr << "failed to set X\n";

    if (cmd.hasArg("y") && !gpd->setY(std::get<std::string>(cmd.getValue("y"))))
        std::cerr << "failed to set Y\n";

    if (cmd.hasArg("lu") && !gpd->setLAnalogUp(std::get<std::string>(cmd.getValue("lu"))))
        std::cerr << "failed to set left analog up\n";

    if (cmd.hasArg("ld") && !gpd->setLAnalogDown(std::get<std::string>(cmd.getValue("ld"))))
        std::cerr << "failed to set left analog down\n";

    if (cmd.hasArg("ll") && !gpd->setLAnalogLeft(std::get<std::string>(cmd.getValue("ll"))))
        std::cerr << "failed to set left analog left\n";

    if (cmd.hasArg("lr") && !gpd->setLAnalogRight(std::get<std::string>(cmd.getValue("lr"))))
        std::cerr << "failed to set left analog right\n";

    if (cmd.hasArg("st") && !gpd->setStart(std::get<std::string>(cmd.getValue("st"))))
        std::cerr << "failed to set start\n";

    if (cmd.hasArg("sl") && !gpd->setSelect(std::get<std::string>(cmd.getValue("sl"))))
        std::cerr << "failed to set select\n";

    if (cmd.hasArg("mu") && !gpd->setMenu(std::get<std::string>(cmd.getValue("mu"))))
        std::cerr << "failed to set menu\n";

    if (cmd.hasArg("l1") && !gpd->setL1(std::get<std::string>(cmd.getValue("l1"))))
        std::cerr << "failed to set L1\n";

    if (cmd.hasArg("l2") && !gpd->setL2(std::get<std::string>(cmd.getValue("l2"))))
        std::cerr << "failed to set L2\n";

    if (cmd.hasArg("l3") && !gpd->setL3(std::get<std::string>(cmd.getValue("l3"))))
        std::cerr << "failed to set L3\n";

    if (cmd.hasArg("r1") && !gpd->setR1(std::get<std::string>(cmd.getValue("r1"))))
        std::cerr << "failed to set R1\n";

    if (cmd.hasArg("r2") && !gpd->setR2(std::get<std::string>(cmd.getValue("r2"))))
        std::cerr << "failed to set R2\n";

    if (cmd.hasArg("r3") && !gpd->setR3(std::get<std::string>(cmd.getValue("r3"))))
        std::cerr << "failed to set R3\n";

    if (cmd.hasArg("bl1") && !gpd->setLBack1(std::get<std::string>(cmd.getValue("bl1"))))
        std::cerr << "failed to set back left slot 1\n";

    if (cmd.hasArg("bl2") && !gpd->setLBack2(std::get<std::string>(cmd.getValue("bl2"))))
        std::cerr << "failed to set back left slot 2\n";

    if (cmd.hasArg("bl3") && !gpd->setLBack3(std::get<std::string>(cmd.getValue("bl3"))))
        std::cerr << "failed to set back left slot 3\n";

    if (cmd.hasArg("bl4") && !gpd->setLBack4(std::get<std::string>(cmd.getValue("bl4"))))
        std::cerr << "failed to set back left slot 4\n";

    if (cmd.hasArg("br1") && !gpd->setRBack1(std::get<std::string>(cmd.getValue("br1"))))
        std::cerr << "failed to set back right slot 1\n";

    if (cmd.hasArg("br2") && !gpd->setRBack2(std::get<std::string>(cmd.getValue("br2"))))
        std::cerr << "failed to set back right slot 2\n";

    if (cmd.hasArg("br3") && !gpd->setRBack3(std::get<std::string>(cmd.getValue("br3"))))
        std::cerr << "failed to set back right slot 3\n";

    if (cmd.hasArg("br4") && !gpd->setRBack4(std::get<std::string>(cmd.getValue("br4"))))
        std::cerr << "failed to set back right slot 4\n";

    if (cmd.hasArg("bl1d"))
        gpd->setLBack1Delay(std::get<int>(cmd.getValue("bl1d")));

    if (cmd.hasArg("bl2d"))
        gpd->setLBack2Delay(std::get<int>(cmd.getValue("bl2d")));

    if (cmd.hasArg("bl3d"))
        gpd->setLBack3Delay(std::get<int>(cmd.getValue("bl3d")));

    if (cmd.hasArg("bl4d"))
        gpd->setLBack4Delay(std::get<int>(cmd.getValue("bl4d")));

    if (cmd.hasArg("br1d"))
        gpd->setRBack1Delay(std::get<int>(cmd.getValue("br1d")));

    if (cmd.hasArg("br2d"))
        gpd->setRBack2Delay(std::get<int>(cmd.getValue("br2d")));

    if (cmd.hasArg("br3d"))
        gpd->setRBack3Delay(std::get<int>(cmd.getValue("br3d")));

    if (cmd.hasArg("br4d"))
        gpd->setRBack4Delay(std::get<int>(cmd.getValue("br4d")));

    if (gpd->hasFeature(OWC::ControllerFeature::RumbleV1) && cmd.hasArg("rmb"))
        gpd->setRumble(std::get<std::string>(cmd.getValue("rmb")));

    if (gpd->hasFeature(OWC::ControllerFeature::DeadZoneControlV1)) {
        if (cmd.hasArg("lc"))
            gpd->setLAnalogCenter(std::get<int>(cmd.getValue("lc")));

        if (cmd.hasArg("lb"))
            gpd->setLAnalogBoundary(std::get<int>(cmd.getValue("lb")));

        if (cmd.hasArg("rc"))
            gpd->setRAnalogCenter(std::get<int>(cmd.getValue("rc")));

        if (cmd.hasArg("rb"))
            gpd->setRAnalogBoundary(std::get<int>(cmd.getValue("rb")));
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

int main(int argc, char *argv[]) {
    OWC::CMDParser cmdParser(argc, argv);

    if (!cmdParser.parse())
        return 1;

    const std::string product = getProduct();
    const std::shared_ptr<OWC::Controller> gpd = getDevice(product);
    const std::shared_ptr<OWC::FileLogger> logger = OWC::FileLogger::getInstance();
    const std::function<void(const std::string &)> logCB = [&logger](const std::string &msg) { logger->write(msg); };

    if (!gpd)
        return 1;

    if (!logger->init())
        std::cerr << "failed to init log file\n";

    gpd->enableLogging(logCB);

    if (!gpd->init()) {
        std::cerr << "device initialization failed\n";
        return 1;

    } else if (!gpd->readConfig()) {
        std::cerr << "failed to read firmware config\n";
        return 1;

    } else if (!isCompatible(product, gpd)) {
        return 1;
    }

    if (cmdParser.hasArg("print"))
        return printCurrentSettings(gpd);
    else if (cmdParser.hasArg("export"))
       return exportToYaml(gpd, std::get<std::string>(cmdParser.getValue("export")));
    else if (cmdParser.hasArg("import"))
        return importFromYaml(gpd, std::get<std::string>(cmdParser.getValue("import")));
    else if (cmdParser.hasArg("set"))
        return writeConfig(gpd, cmdParser);

    return 0;
}

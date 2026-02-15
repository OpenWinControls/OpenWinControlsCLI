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

#include "classes/FileLogger.h"
#include  "Utils.h"
#include "extern/libOpenWinControls/src/include/ControllerFeature.h"
#include "extern/libOpenWinControls/src/controller/ControllerV1.h"

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
    std::pair<int, int> version = {0, 0};
    bool compCheck = false;

    /*if (product == win3) {
        return true;

    } else*/ if (product == win4) {
        version = std::dynamic_pointer_cast<OWC::ControllerV1>(gpd)->getKVersion();
        compCheck = version.first >= 4 && version.second >= 7;

    } else if (product == mini23 || product == mini24) {
        version = std::dynamic_pointer_cast<OWC::ControllerV1>(gpd)->getKVersion();
        compCheck = version.first >= 5 && version.second >= 3;

    } else if (product == max2_22 || product == max2_25) {
        version = std::dynamic_pointer_cast<OWC::ControllerV1>(gpd)->getKVersion();
        compCheck = version.first >= 1 && version.second >= 23;
    }

    if (!compCheck)
        std::cout << "version " << version.first << "." << version.second << " is not supported, please update.\n";

    return compCheck;
}

int main(int argc, char *argv[]) {
    OWC::CMDParser cmdParser(argc, argv);

    if (!cmdParser.parse())
        return 1;

    const std::string product = getProduct();
    const std::shared_ptr<OWC::Controller> gpd = getDevice(product);
    const std::shared_ptr<OWC::FileLogger> logger = OWC::FileLogger::getInstance();

    if (!gpd)
        return 1;

    if (!logger->init())
        std::cerr << "failed to init log file\n";
    else
        gpd->enableLogging([&logger](const std::wstring &msg) { logger->writeExt(msg); });

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
        OWCL::printCurrentSettings(gpd);
    else if (cmdParser.hasArg("export"))
       return OWCL::exportToYaml(gpd, std::get<std::string>(cmdParser.getValue("export")));
    else if (cmdParser.hasArg("import"))
        return OWCL::importFromYaml(gpd, std::get<std::string>(cmdParser.getValue("import")));
    else if (cmdParser.hasArg("set"))
        return OWCL::writeConfig(gpd, cmdParser);

    return 0;
}

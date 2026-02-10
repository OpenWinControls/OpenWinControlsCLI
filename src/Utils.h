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
#pragma once

#include <memory>

#include "extern/libOpenWinControls/src/controller/Controller.h"
#include "classes/CMDParser.h"

namespace OWCL {
    void printCurrentSettings(const std::shared_ptr<OWC::Controller> &gpd);
    [[nodiscard]] int exportToYaml(const std::shared_ptr<OWC::Controller> &gpd, const std::string &fileName);
    [[nodiscard]] int importFromYaml(const std::shared_ptr<OWC::Controller> &gpd, const std::string &fileName);
    [[nodiscard]] int writeConfig(const std::shared_ptr<OWC::Controller> &gpd, const OWC::CMDParser &cmd);
}

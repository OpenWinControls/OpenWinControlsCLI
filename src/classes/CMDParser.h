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

#include <map>
#include <string>
#include <variant>
#include <vector>

namespace OWC {
    typedef std::variant<std::string, int, std::tuple<int, int, int>, std::vector<std::string>, std::vector<int>> owc_arg_value;

    class CMDParser final {
    private:
        std::map<std::string, owc_arg_value> args;
        char **argV;
        int argC;

        void showHelp() const;
        void showKeys() const;
        [[nodiscard]] bool isArg(std::string_view arg) const;
        [[nodiscard]] bool parseSetOptions();

    public:
        CMDParser(int argc, char *argv[]);

        [[nodiscard]] bool parse();
        [[nodiscard]] bool hasArg(const std::string &arg) const { return args.contains(arg); }
        [[nodiscard]] owc_arg_value getValue(const std::string &arg) const { return args.at(arg); }
    };
}

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

#include <fstream>
#include <source_location>

namespace OWC {
    class FileLogger final {
    private:
        static inline FileLogger *instance = nullptr;
        std::wofstream logF;

        FileLogger() = default;

    public:
        FileLogger(FileLogger &) = delete;

        ~FileLogger();

        static FileLogger *getInstance();
        [[nodiscard]] bool init();
        void write(const std::wstring &msg, std::source_location loc = std::source_location::current());
        void writeExt(const std::wstring &msg);
    };
}

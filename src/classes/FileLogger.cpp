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
#include <chrono>

#include "FileLogger.h"
#include "../version.h"

namespace OWC {
    FileLogger::~FileLogger() {
        logF.close();
    }

    std::shared_ptr<FileLogger> FileLogger::getInstance() {
        if (!instance)
            instance.reset(new FileLogger);

        return instance;
    }

    bool FileLogger::init() {
        const std::chrono::time_point now = std::chrono::system_clock::now();
        const std::time_t time = std::chrono::system_clock::to_time_t(now);

        logF.open("openwincontrols.log");

        if (!logF.is_open())
            return false;

        logF << APP_NAME " logs - " << std::ctime(&time) << "\n\n";
        return true;
    }

    void FileLogger::write(const std::wstring &msg, const std::source_location loc) {
        if (!logF.is_open())
            return;

        logF << loc.file_name() << "\n[" << loc.function_name() << ":" << loc.line() << "]\n" << msg << "\n\n";
        logF.flush();
    }

    void FileLogger::writeExt(const std::wstring &msg) {
        if (!logF.is_open())
            return;

        logF << msg << "\n\n";
        logF.flush();
    }
}

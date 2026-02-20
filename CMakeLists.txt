cmake_minimum_required(VERSION 3.23)

project(OpenWinControlsCLI VERSION 2.2 DESCRIPTION "Multiplatform GPD WinControls replacement (command-line)" LANGUAGES CXX)

set(PROJECT_AUTHOR "kylon")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS OFF)
add_subdirectory(src/extern/libOpenWinControls)
add_subdirectory(src/extern/yaml-cpp)

set(PROJECT_SRC
    src/classes/FileLogger.h
    src/classes/FileLogger.cpp
    src/classes/CMDParser.h
    src/classes/CMDParser.cpp

    src/Utils.h
    src/Utils.cpp
    src/main.cpp
)

if (WIN32)
  configure_file(src/resources/win.rc.in ${CMAKE_CURRENT_SOURCE_DIR}/win.rc)

  list(APPEND PROJECT_SRC
      src/include/win.h
      win.rc
  )
endif ()

configure_file(src/resources/version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/version.h)

add_executable(${PROJECT_NAME} ${PROJECT_SRC})

include(CheckIPOSupported)
check_ipo_supported(RESULT has_ipo OUTPUT ipo_error)
if (has_ipo)
  message(STATUS "${PROJECT_NAME}: IPO/LTO enabled")
  set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

target_link_libraries(${PROJECT_NAME} PRIVATE lowc::owc yaml-cpp::yaml-cpp)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

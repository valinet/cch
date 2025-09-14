cmake_minimum_required(VERSION 3.10)
project(cch LANGUAGES CXX)

include(GNUInstallDirs)

# Compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wno-sign-compare -Werror -O2)

# Detect git hash
find_package(Git QUIET)
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --verify HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE BUILD_VER
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
else()
    set(BUILD_VER "unknown")
endif()

# Repo URL
set(REPO_URL "https://github.com/tjps/cch")

# Generate Version.cc at configure time
set(VERSION_CC ${CMAKE_BINARY_DIR}/Version.cc)
file(WRITE ${VERSION_CC}
"#include \"Version.h\"\n"
"const char* Version::kBuildVersion = \"${BUILD_VER}\";\n"
"const char* Version::kRepoURL = \"${REPO_URL}\";\n"
)

# Source files
set(CCH_SOURCES
    src/main.cc
    src/StringView.cc
    src/Token.cc
    src/Util.cc
    ${VERSION_CC}
)

set(TEST_SOURCES
    test/unittest_util.cc
    src/Util.cc
)

# Main executable
add_executable(cch ${CCH_SOURCES})
target_include_directories(cch PRIVATE src ${CMAKE_BINARY_DIR})

# Test executable
add_executable(unittest_util ${TEST_SOURCES})
target_include_directories(unittest_util PRIVATE src ${CMAKE_BINARY_DIR})

# Custom test runner target
add_custom_target(runtests
    COMMAND ${CMAKE_SOURCE_DIR}/test/testcases.sh
    COMMAND ${CMAKE_SOURCE_DIR}/test/unittests.sh
    DEPENDS unittest_util
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Install rules
install(TARGETS cch DESTINATION ${CMAKE_INSTALL_BINDIR})

install(CODE "
    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/man)
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/man/cch.1 ${CMAKE_BINARY_DIR}/man/cch.1)
    execute_process(COMMAND gzip -c ${CMAKE_BINARY_DIR}/man/cch.1
                    OUTPUT_FILE ${CMAKE_BINARY_DIR}/cch.1.gz)
")

install(FILES ${CMAKE_BINARY_DIR}/cch.1.gz
    DESTINATION ${CMAKE_INSTALL_MANDIR}/man1
)

# Clean target (optional, cmake --build . --target clean works too)
add_custom_target(distclean
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/*
)

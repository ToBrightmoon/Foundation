unset(CMAKE_C_COMPILER CACHE)
unset(CMAKE_CXX_COMPILER CACHE)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Windows 下使用 Visual Studio 编译器
    set(CMAKE_C_COMPILER "cl.exe")
    set(CMAKE_CXX_COMPILER "cl.exe")
    set(CMAKE_GENERATOR "Visual Studio 17 2022")  # 或者其他版本号的 Visual Studio
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Linux 下使用 GCC 编译器
    set(CMAKE_C_COMPILER "/usr/bin/gcc-13")
    set(CMAKE_CXX_COMPILER "/usr/bin/g++-13")
endif()

# 输出编译器信息以供验证
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")

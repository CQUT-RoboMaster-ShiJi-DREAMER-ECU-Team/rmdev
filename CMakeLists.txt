cmake_minimum_required(VERSION 3.28)

project(rmdev C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)

add_library(${PROJECT_NAME} STATIC)

if (NOT(${CMAKE_CXX_FLAGS} MATCHES "-fmodules-ts"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmodules-ts")
endif ()
if (NOT(${CMAKE_CXX_FLAGS} MATCHES "-fno-exceptions"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
endif ()
if (NOT(${CMAKE_CXX_FLAGS} MATCHES "-fno-rtti"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif ()

target_include_directories(${PROJECT_NAME}
    PUBLIC
        inc/
)

file(GLOB_RECURSE RMDEV_PUBLIC_MODULES "modules/*.cppm" "src/*.cppm")
file(GLOB_RECURSE RMDEV_PUBLIC_SOURCES "src/*.cpp")
target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET rmdev_public_modules_fileset TYPE CXX_MODULES FILES ${RMDEV_PUBLIC_MODULES}

    PUBLIC
        ${RMDEV_PUBLIC_SOURCES}
)

if (RMDEV_ENABLE_TESTS)
    add_subdirectory(test/platform_test)
endif ()

if (RMDEV_USE_CMSIS_DSP)
    add_definitions(-DRMDEV_USE_CMSIS_DSP=1)

    if (RMDEV_USE_STM32CUBEMX)
        enable_language(ASM)
        target_link_libraries(${PROJECT_NAME} PUBLIC stm32cubemx)
    else ()
        # todo 后续增加对其他平台的支持
    endif ()
endif ()

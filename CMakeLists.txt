cmake_minimum_required(VERSION 3.28)

project(rmdev C CXX)

set(CMAKE_C_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)

option(RMDEV_ENABLE_TESTS "Enable the test for rmdev." OFF)
option(RMDEV_USE_STM32CUBEMX "Will link stm32cubemx to rmdev." ON)
option(RMDEV_ENABLE_INS_MODULE "Enable the rmdev_ins module." OFF)

set(RMDEV_ENABLED_DRIVER_LIST "" CACHE STRING "Select which device driver to use.")

set(RMDEV_THREAD_SYSTEM "FreeRTOS" CACHE STRING "Set this variable to choose which system to use. Use \"FreeRTOS\" as default.")

set(EMDEVIF_THREAD_SYSTEM ${RMDEV_THREAD_SYSTEM} CACHE STRING "" FORCE)
set(EMDEVIF_USE_STM32CUBEMX ${RMDEV_USE_STM32CUBEMX} CACHE BOOL "" FORCE)

if (${RMDEV_THREAD_SYSTEM} STREQUAL "FreeRTOS")
    option(RMDEV_FREERTOS_USE_STM32CUBEMX "Will use stm32cubemx generated FreeRTOS sources" ON)

    set(EMDEVIF_FREERTOS_USE_STM32CUBEMX ${RMDEV_FREERTOS_USE_STM32CUBEMX} CACHE BOOL "" FORCE)
endif ()

if (RMDEV_USE_STM32CUBEMX)
    enable_language(ASM)
endif ()

if (RMDEV_ENABLE_TESTS)
    set(EMDEVIF_ENABLE_TESTS ON CACHE BOOL "" FORCE)
endif ()
add_subdirectory(emdevif_home/emdevif)

add_subdirectory(modules/rmdev_math)
add_subdirectory(modules/rmdev_control_algorithm)
add_subdirectory(modules/rmdev_kinematic_solution)
add_subdirectory(modules/rmdev_device_model)
add_subdirectory(modules/rmdev_debug_assistance)

add_library(${PROJECT_NAME} STATIC)

target_link_libraries(${PROJECT_NAME} PUBLIC
    emdevif

    rmdev_math
    rmdev_control_algorithm
    rmdev_kinematic_solution
    rmdev_device_model
    rmdev_debug_assistance
)

if (NOT (RMDEV_ENABLED_DRIVER_LIST STREQUAL ""))
    block()
        foreach (usedDriverName IN LISTS RMDEV_ENABLED_DRIVER_LIST)
            set(subDirName "rmdev_driver_")
            string(APPEND subDirName ${usedDriverName})

            set(libName ${subDirName})

            add_subdirectory(drivers/${subDirName})
            target_link_libraries(${PROJECT_NAME} PUBLIC ${libName})

            message("[rmdev]: Enabled driver lib \"${libName}\"")
        endforeach ()
    endblock()
endif ()

if (RMDEV_ENABLE_INS_MODULE)
    add_subdirectory(modules/rmdev_ins)
    target_link_libraries(${PROJECT_NAME} PUBLIC rmdev_ins)
    message("[${PROJECT_NAME}]: Enabled \"INS\" lib.")
endif ()

if (RMDEV_ENABLE_TESTS)
    enable_testing()
    add_subdirectory(test)
    message("[${PROJECT_NAME}]: Enabled tests.")
endif ()

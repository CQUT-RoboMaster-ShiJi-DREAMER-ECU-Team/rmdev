cmake_minimum_required(VERSION 3.28)

project(rmdev_thread CXX)

add_library(${PROJECT_NAME} OBJECT)

add_subdirectory(system)

target_link_libraries(${PROJECT_NAME} PUBLIC rmdev_public_headers rmdev_thread_system_support rmdev_error_handler)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET rmdev_thread_modules TYPE CXX_MODULES FILES
        message_queue.cppm
        thread_constant.cppm
)

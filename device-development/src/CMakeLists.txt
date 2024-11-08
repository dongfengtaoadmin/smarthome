cmake_minimum_required(VERSION 3.10)
aux_source_directory(linux-uart SRC_CPP_LISTS)
aux_source_directory(fingerprint SRC_CPP_LISTS)
add_library(smart_dev STATIC ${SRC_CPP_LISTS})

target_include_directories(smart_dev PUBLIC ${CMAKE_SOURCE_DIR}/src/linux-uart)

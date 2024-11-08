cmake_minimum_required(VERSION 3.10)

#要成对都有才行 fingerprint-protocol-test 模块
add_executable(fingerprint-protocol-test fingerprint/fingerprint-protocol-test.cpp)
target_include_directories(fingerprint-protocol-test PUBLIC ${CMAKE_SOURCE_DIR}/src/fingerprint)
target_link_libraries(fingerprint-protocol-test smart_dev)


add_executable(fingerprint-device-test fingerprint/fingerprint-device-test.cpp)
target_include_directories(fingerprint-device-test PUBLIC ${CMAKE_SOURCE_DIR}/src/fingerprint)
target_link_libraries(fingerprint-device-test smart_dev)


#要成对都有才行 linux-uart-test 模块
# 定义一个名为 linux-uart-test 的可执行文件，并将 linux-uart/linux-uart-test.cpp 作为其源文件
add_executable(linux-uart-test linux-uart/linux-uart-test.cpp)
# 设置 linux-uart-test 可执行文件的包含目录，使其可以访问 ${CMAKE_SOURCE_DIR}/src/linux-uart 中的头文件
target_include_directories(linux-uart-test PUBLIC ${CMAKE_SOURCE_DIR}/src/linux-uart)

# 将 smart_dev 库链接到 linux-uart-test 可执行文件上，使其可以使用 smart_dev 库中的函数和资源
target_link_libraries(linux-uart-test smart_dev)

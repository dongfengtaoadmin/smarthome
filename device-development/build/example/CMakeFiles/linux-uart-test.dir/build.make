# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/linux/imx6ull-iot-smart-car/fs/ubuntu-rootfs/smarthome/device-development

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/linux/imx6ull-iot-smart-car/fs/ubuntu-rootfs/smarthome/device-development/build

# Include any dependencies generated for this target.
include example/CMakeFiles/linux-uart-test.dir/depend.make

# Include the progress variables for this target.
include example/CMakeFiles/linux-uart-test.dir/progress.make

# Include the compile flags for this target's objects.
include example/CMakeFiles/linux-uart-test.dir/flags.make

example/CMakeFiles/linux-uart-test.dir/linux-uart/linux-uart-test.cpp.o: example/CMakeFiles/linux-uart-test.dir/flags.make
example/CMakeFiles/linux-uart-test.dir/linux-uart/linux-uart-test.cpp.o: ../example/linux-uart/linux-uart-test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/linux/imx6ull-iot-smart-car/fs/ubuntu-rootfs/smarthome/device-development/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object example/CMakeFiles/linux-uart-test.dir/linux-uart/linux-uart-test.cpp.o"
	cd /home/linux/imx6ull-iot-smart-car/fs/ubuntu-rootfs/smarthome/device-development/build/example && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/linux-uart-test.dir/linux-uart/linux-uart-test.cpp.o -c /home/linux/imx6ull-iot-smart-car/fs/ubuntu-rootfs/smarthome/device-development/example/linux-uart/linux-uart-test.cpp

example/CMakeFiles/linux-uart-test.dir/linux-uart/linux-uart-test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/linux-uart-test.dir/linux-uart/linux-uart-test.cpp.i"
	cd /home/linux/imx6ull-iot-smart-car/fs/ubuntu-rootfs/smarthome/device-development/build/example && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/linux/imx6ull-iot-smart-car/fs/ubuntu-rootfs/smarthome/device-development/example/linux-uart/linux-uart-test.cpp > CMakeFiles/linux-uart-test.dir/linux-uart/linux-uart-test.cpp.i

example/CMakeFiles/linux-uart-test.dir/linux-uart/linux-uart-test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/linux-uart-test.dir/linux-uart/linux-uart-test.cpp.s"
	cd /home/linux/imx6ull-iot-smart-car/fs/ubuntu-rootfs/smarthome/device-development/build/example && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/linux/imx6ull-iot-smart-car/fs/ubuntu-rootfs/smarthome/device-development/example/linux-uart/linux-uart-test.cpp -o CMakeFiles/linux-uart-test.dir/linux-uart/linux-uart-test.cpp.s

example/CMakeFiles/linux-uart-test.dir/linux-uart/linux-uart-test.cpp.o.requires:

.PHONY : example/CMakeFiles/linux-uart-test.dir/linux-uart/linux-uart-test.cpp.o.requires

example/CMakeFiles/linux-uart-test.dir/linux-uart/linux-uart-test.cpp.o.provides: example/CMakeFiles/linux-uart-test.dir/linux-uart/linux-uart-test.cpp.o.requires
	$(MAKE) -f example/CMakeFiles/linux-uart-test.dir/build.make example/CMakeFiles/linux-uart-test.dir/linux-uart/linux-uart-test.cpp.o.provides.build
.PHONY : example/CMakeFiles/linux-uart-test.dir/linux-uart/linux-uart-test.cpp.o.provides

example/CMakeFiles/linux-uart-test.dir/linux-uart/linux-uart-test.cpp.o.provides.build: example/CMakeFiles/linux-uart-test.dir/linux-uart/linux-uart-test.cpp.o


# Object files for target linux-uart-test
linux__uart__test_OBJECTS = \
"CMakeFiles/linux-uart-test.dir/linux-uart/linux-uart-test.cpp.o"

# External object files for target linux-uart-test
linux__uart__test_EXTERNAL_OBJECTS =

example/linux-uart-test: example/CMakeFiles/linux-uart-test.dir/linux-uart/linux-uart-test.cpp.o
example/linux-uart-test: example/CMakeFiles/linux-uart-test.dir/build.make
example/linux-uart-test: src/libsmart_dev.a
example/linux-uart-test: example/CMakeFiles/linux-uart-test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/linux/imx6ull-iot-smart-car/fs/ubuntu-rootfs/smarthome/device-development/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable linux-uart-test"
	cd /home/linux/imx6ull-iot-smart-car/fs/ubuntu-rootfs/smarthome/device-development/build/example && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/linux-uart-test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
example/CMakeFiles/linux-uart-test.dir/build: example/linux-uart-test

.PHONY : example/CMakeFiles/linux-uart-test.dir/build

example/CMakeFiles/linux-uart-test.dir/requires: example/CMakeFiles/linux-uart-test.dir/linux-uart/linux-uart-test.cpp.o.requires

.PHONY : example/CMakeFiles/linux-uart-test.dir/requires

example/CMakeFiles/linux-uart-test.dir/clean:
	cd /home/linux/imx6ull-iot-smart-car/fs/ubuntu-rootfs/smarthome/device-development/build/example && $(CMAKE_COMMAND) -P CMakeFiles/linux-uart-test.dir/cmake_clean.cmake
.PHONY : example/CMakeFiles/linux-uart-test.dir/clean

example/CMakeFiles/linux-uart-test.dir/depend:
	cd /home/linux/imx6ull-iot-smart-car/fs/ubuntu-rootfs/smarthome/device-development/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/linux/imx6ull-iot-smart-car/fs/ubuntu-rootfs/smarthome/device-development /home/linux/imx6ull-iot-smart-car/fs/ubuntu-rootfs/smarthome/device-development/example /home/linux/imx6ull-iot-smart-car/fs/ubuntu-rootfs/smarthome/device-development/build /home/linux/imx6ull-iot-smart-car/fs/ubuntu-rootfs/smarthome/device-development/build/example /home/linux/imx6ull-iot-smart-car/fs/ubuntu-rootfs/smarthome/device-development/build/example/CMakeFiles/linux-uart-test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : example/CMakeFiles/linux-uart-test.dir/depend


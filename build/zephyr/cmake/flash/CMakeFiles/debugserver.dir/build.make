# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.11

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/israel/Dropbox/Ufal/2017.2/SE/06_Projeto

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/israel/Dropbox/Ufal/2017.2/SE/06_Projeto/build

# Utility rule file for debugserver.

# Include the progress variables for this target.
include zephyr/cmake/flash/CMakeFiles/debugserver.dir/progress.make

zephyr/cmake/flash/CMakeFiles/debugserver: zephyr/zephyr.elf
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/israel/Dropbox/Ufal/2017.2/SE/06_Projeto/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Debugging bbc_microbit"
	/usr/local/bin/cmake -E env /usr/bin/python3 /home/israel/zephyr/scripts/support/zephyr_flash_debug.py pyocd debugserver --board-dir=/home/israel/zephyr/boards/arm/bbc_microbit --kernel-elf=/home/israel/Dropbox/Ufal/2017.2/SE/06_Projeto/build/zephyr/zephyr.elf --kernel-hex=/home/israel/Dropbox/Ufal/2017.2/SE/06_Projeto/build/zephyr/zephyr.hex --kernel-bin=/home/israel/Dropbox/Ufal/2017.2/SE/06_Projeto/build/zephyr/zephyr.bin --gdb=/opt/zephyr-sdk/sysroots/x86_64-pokysdk-linux/usr/bin/arm-zephyr-eabi/arm-zephyr-eabi-gdb --openocd=/opt/zephyr-sdk/sysroots/x86_64-pokysdk-linux/usr/bin/openocd --openocd-search=/opt/zephyr-sdk/sysroots/x86_64-pokysdk-linux/usr/share/openocd/scripts --dt-flash=y --target=nrf51

debugserver: zephyr/cmake/flash/CMakeFiles/debugserver
debugserver: zephyr/cmake/flash/CMakeFiles/debugserver.dir/build.make

.PHONY : debugserver

# Rule to build all files generated by this target.
zephyr/cmake/flash/CMakeFiles/debugserver.dir/build: debugserver

.PHONY : zephyr/cmake/flash/CMakeFiles/debugserver.dir/build

zephyr/cmake/flash/CMakeFiles/debugserver.dir/clean:
	cd /home/israel/Dropbox/Ufal/2017.2/SE/06_Projeto/build/zephyr/cmake/flash && $(CMAKE_COMMAND) -P CMakeFiles/debugserver.dir/cmake_clean.cmake
.PHONY : zephyr/cmake/flash/CMakeFiles/debugserver.dir/clean

zephyr/cmake/flash/CMakeFiles/debugserver.dir/depend:
	cd /home/israel/Dropbox/Ufal/2017.2/SE/06_Projeto/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/israel/Dropbox/Ufal/2017.2/SE/06_Projeto /home/israel/zephyr/cmake/flash /home/israel/Dropbox/Ufal/2017.2/SE/06_Projeto/build /home/israel/Dropbox/Ufal/2017.2/SE/06_Projeto/build/zephyr/cmake/flash /home/israel/Dropbox/Ufal/2017.2/SE/06_Projeto/build/zephyr/cmake/flash/CMakeFiles/debugserver.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : zephyr/cmake/flash/CMakeFiles/debugserver.dir/depend

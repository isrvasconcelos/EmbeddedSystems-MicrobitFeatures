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

# Utility rule file for gconfig.

# Include the progress variables for this target.
include CMakeFiles/gconfig.dir/progress.make

CMakeFiles/gconfig:
	cd /home/israel/Dropbox/Ufal/2017.2/SE/06_Projeto/build/zephyr/kconfig && /usr/local/bin/cmake -E env srctree=/home/israel/zephyr KERNELVERSION=1.11.0 KCONFIG_CONFIG=/home/israel/Dropbox/Ufal/2017.2/SE/06_Projeto/build/zephyr/.config ENV_VAR_ARCH=arm ENV_VAR_BOARD_DIR=/home/israel/zephyr/boards/arm/bbc_microbit gconf /home/israel/zephyr/Kconfig

gconfig: CMakeFiles/gconfig
gconfig: CMakeFiles/gconfig.dir/build.make

.PHONY : gconfig

# Rule to build all files generated by this target.
CMakeFiles/gconfig.dir/build: gconfig

.PHONY : CMakeFiles/gconfig.dir/build

CMakeFiles/gconfig.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gconfig.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gconfig.dir/clean

CMakeFiles/gconfig.dir/depend:
	cd /home/israel/Dropbox/Ufal/2017.2/SE/06_Projeto/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/israel/Dropbox/Ufal/2017.2/SE/06_Projeto /home/israel/Dropbox/Ufal/2017.2/SE/06_Projeto /home/israel/Dropbox/Ufal/2017.2/SE/06_Projeto/build /home/israel/Dropbox/Ufal/2017.2/SE/06_Projeto/build /home/israel/Dropbox/Ufal/2017.2/SE/06_Projeto/build/CMakeFiles/gconfig.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/gconfig.dir/depend

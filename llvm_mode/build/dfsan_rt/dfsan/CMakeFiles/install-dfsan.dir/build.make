# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /parmesan/llvm_mode

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /parmesan/llvm_mode/build

# Utility rule file for install-dfsan.

# Include the progress variables for this target.
include dfsan_rt/dfsan/CMakeFiles/install-dfsan.dir/progress.make

dfsan_rt/dfsan/CMakeFiles/install-dfsan:
	cd /parmesan/llvm_mode/build/dfsan_rt/dfsan && /usr/bin/cmake -DCMAKE_INSTALL_COMPONENT=dfsan -P /parmesan/llvm_mode/build/cmake_install.cmake

install-dfsan: dfsan_rt/dfsan/CMakeFiles/install-dfsan
install-dfsan: dfsan_rt/dfsan/CMakeFiles/install-dfsan.dir/build.make

.PHONY : install-dfsan

# Rule to build all files generated by this target.
dfsan_rt/dfsan/CMakeFiles/install-dfsan.dir/build: install-dfsan

.PHONY : dfsan_rt/dfsan/CMakeFiles/install-dfsan.dir/build

dfsan_rt/dfsan/CMakeFiles/install-dfsan.dir/clean:
	cd /parmesan/llvm_mode/build/dfsan_rt/dfsan && $(CMAKE_COMMAND) -P CMakeFiles/install-dfsan.dir/cmake_clean.cmake
.PHONY : dfsan_rt/dfsan/CMakeFiles/install-dfsan.dir/clean

dfsan_rt/dfsan/CMakeFiles/install-dfsan.dir/depend:
	cd /parmesan/llvm_mode/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /parmesan/llvm_mode /parmesan/llvm_mode/dfsan_rt/dfsan /parmesan/llvm_mode/build /parmesan/llvm_mode/build/dfsan_rt/dfsan /parmesan/llvm_mode/build/dfsan_rt/dfsan/CMakeFiles/install-dfsan.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : dfsan_rt/dfsan/CMakeFiles/install-dfsan.dir/depend


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

# Utility rule file for install-dfsan_rt-x86_64.

# Include the progress variables for this target.
include dfsan_rt/dfsan/CMakeFiles/install-dfsan_rt-x86_64.dir/progress.make

dfsan_rt/dfsan/CMakeFiles/install-dfsan_rt-x86_64: dfsan_rt/libdfsan_rt-x86_64.a
	cd /parmesan/llvm_mode/build/dfsan_rt/dfsan && /usr/bin/cmake -DCMAKE_INSTALL_COMPONENT=dfsan_rt-x86_64 -P /parmesan/llvm_mode/build/cmake_install.cmake

install-dfsan_rt-x86_64: dfsan_rt/dfsan/CMakeFiles/install-dfsan_rt-x86_64
install-dfsan_rt-x86_64: dfsan_rt/dfsan/CMakeFiles/install-dfsan_rt-x86_64.dir/build.make

.PHONY : install-dfsan_rt-x86_64

# Rule to build all files generated by this target.
dfsan_rt/dfsan/CMakeFiles/install-dfsan_rt-x86_64.dir/build: install-dfsan_rt-x86_64

.PHONY : dfsan_rt/dfsan/CMakeFiles/install-dfsan_rt-x86_64.dir/build

dfsan_rt/dfsan/CMakeFiles/install-dfsan_rt-x86_64.dir/clean:
	cd /parmesan/llvm_mode/build/dfsan_rt/dfsan && $(CMAKE_COMMAND) -P CMakeFiles/install-dfsan_rt-x86_64.dir/cmake_clean.cmake
.PHONY : dfsan_rt/dfsan/CMakeFiles/install-dfsan_rt-x86_64.dir/clean

dfsan_rt/dfsan/CMakeFiles/install-dfsan_rt-x86_64.dir/depend:
	cd /parmesan/llvm_mode/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /parmesan/llvm_mode /parmesan/llvm_mode/dfsan_rt/dfsan /parmesan/llvm_mode/build /parmesan/llvm_mode/build/dfsan_rt/dfsan /parmesan/llvm_mode/build/dfsan_rt/dfsan/CMakeFiles/install-dfsan_rt-x86_64.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : dfsan_rt/dfsan/CMakeFiles/install-dfsan_rt-x86_64.dir/depend


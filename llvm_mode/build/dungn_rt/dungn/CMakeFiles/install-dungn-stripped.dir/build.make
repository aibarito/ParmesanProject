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

# Utility rule file for install-dungn-stripped.

# Include the progress variables for this target.
include dungn_rt/dungn/CMakeFiles/install-dungn-stripped.dir/progress.make

dungn_rt/dungn/CMakeFiles/install-dungn-stripped:
	cd /parmesan/llvm_mode/build/dungn_rt/dungn && /usr/bin/cmake -DCMAKE_INSTALL_COMPONENT=dungn -DCMAKE_INSTALL_DO_STRIP=1 -P /parmesan/llvm_mode/build/cmake_install.cmake

install-dungn-stripped: dungn_rt/dungn/CMakeFiles/install-dungn-stripped
install-dungn-stripped: dungn_rt/dungn/CMakeFiles/install-dungn-stripped.dir/build.make

.PHONY : install-dungn-stripped

# Rule to build all files generated by this target.
dungn_rt/dungn/CMakeFiles/install-dungn-stripped.dir/build: install-dungn-stripped

.PHONY : dungn_rt/dungn/CMakeFiles/install-dungn-stripped.dir/build

dungn_rt/dungn/CMakeFiles/install-dungn-stripped.dir/clean:
	cd /parmesan/llvm_mode/build/dungn_rt/dungn && $(CMAKE_COMMAND) -P CMakeFiles/install-dungn-stripped.dir/cmake_clean.cmake
.PHONY : dungn_rt/dungn/CMakeFiles/install-dungn-stripped.dir/clean

dungn_rt/dungn/CMakeFiles/install-dungn-stripped.dir/depend:
	cd /parmesan/llvm_mode/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /parmesan/llvm_mode /parmesan/llvm_mode/dungn_rt/dungn /parmesan/llvm_mode/build /parmesan/llvm_mode/build/dungn_rt/dungn /parmesan/llvm_mode/build/dungn_rt/dungn/CMakeFiles/install-dungn-stripped.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : dungn_rt/dungn/CMakeFiles/install-dungn-stripped.dir/depend


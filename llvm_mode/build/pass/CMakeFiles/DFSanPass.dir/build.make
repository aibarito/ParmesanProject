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

# Include any dependencies generated for this target.
include pass/CMakeFiles/DFSanPass.dir/depend.make

# Include the progress variables for this target.
include pass/CMakeFiles/DFSanPass.dir/progress.make

# Include the compile flags for this target's objects.
include pass/CMakeFiles/DFSanPass.dir/flags.make

pass/CMakeFiles/DFSanPass.dir/DFSanPass.cc.o: pass/CMakeFiles/DFSanPass.dir/flags.make
pass/CMakeFiles/DFSanPass.dir/DFSanPass.cc.o: ../pass/DFSanPass.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/parmesan/llvm_mode/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object pass/CMakeFiles/DFSanPass.dir/DFSanPass.cc.o"
	cd /parmesan/llvm_mode/build/pass && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/DFSanPass.dir/DFSanPass.cc.o -c /parmesan/llvm_mode/pass/DFSanPass.cc

pass/CMakeFiles/DFSanPass.dir/DFSanPass.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DFSanPass.dir/DFSanPass.cc.i"
	cd /parmesan/llvm_mode/build/pass && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /parmesan/llvm_mode/pass/DFSanPass.cc > CMakeFiles/DFSanPass.dir/DFSanPass.cc.i

pass/CMakeFiles/DFSanPass.dir/DFSanPass.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DFSanPass.dir/DFSanPass.cc.s"
	cd /parmesan/llvm_mode/build/pass && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /parmesan/llvm_mode/pass/DFSanPass.cc -o CMakeFiles/DFSanPass.dir/DFSanPass.cc.s

# Object files for target DFSanPass
DFSanPass_OBJECTS = \
"CMakeFiles/DFSanPass.dir/DFSanPass.cc.o"

# External object files for target DFSanPass
DFSanPass_EXTERNAL_OBJECTS =

pass/libDFSanPass.so: pass/CMakeFiles/DFSanPass.dir/DFSanPass.cc.o
pass/libDFSanPass.so: pass/CMakeFiles/DFSanPass.dir/build.make
pass/libDFSanPass.so: pass/CMakeFiles/DFSanPass.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/parmesan/llvm_mode/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared module libDFSanPass.so"
	cd /parmesan/llvm_mode/build/pass && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DFSanPass.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
pass/CMakeFiles/DFSanPass.dir/build: pass/libDFSanPass.so

.PHONY : pass/CMakeFiles/DFSanPass.dir/build

pass/CMakeFiles/DFSanPass.dir/clean:
	cd /parmesan/llvm_mode/build/pass && $(CMAKE_COMMAND) -P CMakeFiles/DFSanPass.dir/cmake_clean.cmake
.PHONY : pass/CMakeFiles/DFSanPass.dir/clean

pass/CMakeFiles/DFSanPass.dir/depend:
	cd /parmesan/llvm_mode/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /parmesan/llvm_mode /parmesan/llvm_mode/pass /parmesan/llvm_mode/build /parmesan/llvm_mode/build/pass /parmesan/llvm_mode/build/pass/CMakeFiles/DFSanPass.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : pass/CMakeFiles/DFSanPass.dir/depend


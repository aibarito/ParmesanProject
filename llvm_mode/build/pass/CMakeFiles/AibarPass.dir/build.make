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
include pass/CMakeFiles/AibarPass.dir/depend.make

# Include the progress variables for this target.
include pass/CMakeFiles/AibarPass.dir/progress.make

# Include the compile flags for this target's objects.
include pass/CMakeFiles/AibarPass.dir/flags.make

pass/CMakeFiles/AibarPass.dir/AibarPass.cc.o: pass/CMakeFiles/AibarPass.dir/flags.make
pass/CMakeFiles/AibarPass.dir/AibarPass.cc.o: ../pass/AibarPass.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/parmesan/llvm_mode/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object pass/CMakeFiles/AibarPass.dir/AibarPass.cc.o"
	cd /parmesan/llvm_mode/build/pass && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/AibarPass.dir/AibarPass.cc.o -c /parmesan/llvm_mode/pass/AibarPass.cc

pass/CMakeFiles/AibarPass.dir/AibarPass.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AibarPass.dir/AibarPass.cc.i"
	cd /parmesan/llvm_mode/build/pass && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /parmesan/llvm_mode/pass/AibarPass.cc > CMakeFiles/AibarPass.dir/AibarPass.cc.i

pass/CMakeFiles/AibarPass.dir/AibarPass.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AibarPass.dir/AibarPass.cc.s"
	cd /parmesan/llvm_mode/build/pass && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /parmesan/llvm_mode/pass/AibarPass.cc -o CMakeFiles/AibarPass.dir/AibarPass.cc.s

# Object files for target AibarPass
AibarPass_OBJECTS = \
"CMakeFiles/AibarPass.dir/AibarPass.cc.o"

# External object files for target AibarPass
AibarPass_EXTERNAL_OBJECTS =

pass/libAibarPass.so: pass/CMakeFiles/AibarPass.dir/AibarPass.cc.o
pass/libAibarPass.so: pass/CMakeFiles/AibarPass.dir/build.make
pass/libAibarPass.so: pass/CMakeFiles/AibarPass.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/parmesan/llvm_mode/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared module libAibarPass.so"
	cd /parmesan/llvm_mode/build/pass && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AibarPass.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
pass/CMakeFiles/AibarPass.dir/build: pass/libAibarPass.so

.PHONY : pass/CMakeFiles/AibarPass.dir/build

pass/CMakeFiles/AibarPass.dir/clean:
	cd /parmesan/llvm_mode/build/pass && $(CMAKE_COMMAND) -P CMakeFiles/AibarPass.dir/cmake_clean.cmake
.PHONY : pass/CMakeFiles/AibarPass.dir/clean

pass/CMakeFiles/AibarPass.dir/depend:
	cd /parmesan/llvm_mode/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /parmesan/llvm_mode /parmesan/llvm_mode/pass /parmesan/llvm_mode/build /parmesan/llvm_mode/build/pass /parmesan/llvm_mode/build/pass/CMakeFiles/AibarPass.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : pass/CMakeFiles/AibarPass.dir/depend

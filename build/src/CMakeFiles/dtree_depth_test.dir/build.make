# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jamunk/Dev/CS461/CS461-HW2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jamunk/Dev/CS461/CS461-HW2/build

# Include any dependencies generated for this target.
include src/CMakeFiles/dtree_depth_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/dtree_depth_test.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/dtree_depth_test.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/dtree_depth_test.dir/flags.make

src/CMakeFiles/dtree_depth_test.dir/dtree_depth_test.c.o: src/CMakeFiles/dtree_depth_test.dir/flags.make
src/CMakeFiles/dtree_depth_test.dir/dtree_depth_test.c.o: /home/jamunk/Dev/CS461/CS461-HW2/src/dtree_depth_test.c
src/CMakeFiles/dtree_depth_test.dir/dtree_depth_test.c.o: src/CMakeFiles/dtree_depth_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jamunk/Dev/CS461/CS461-HW2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/CMakeFiles/dtree_depth_test.dir/dtree_depth_test.c.o"
	cd /home/jamunk/Dev/CS461/CS461-HW2/build/src && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/CMakeFiles/dtree_depth_test.dir/dtree_depth_test.c.o -MF CMakeFiles/dtree_depth_test.dir/dtree_depth_test.c.o.d -o CMakeFiles/dtree_depth_test.dir/dtree_depth_test.c.o -c /home/jamunk/Dev/CS461/CS461-HW2/src/dtree_depth_test.c

src/CMakeFiles/dtree_depth_test.dir/dtree_depth_test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dtree_depth_test.dir/dtree_depth_test.c.i"
	cd /home/jamunk/Dev/CS461/CS461-HW2/build/src && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jamunk/Dev/CS461/CS461-HW2/src/dtree_depth_test.c > CMakeFiles/dtree_depth_test.dir/dtree_depth_test.c.i

src/CMakeFiles/dtree_depth_test.dir/dtree_depth_test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dtree_depth_test.dir/dtree_depth_test.c.s"
	cd /home/jamunk/Dev/CS461/CS461-HW2/build/src && /usr/lib64/ccache/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jamunk/Dev/CS461/CS461-HW2/src/dtree_depth_test.c -o CMakeFiles/dtree_depth_test.dir/dtree_depth_test.c.s

# Object files for target dtree_depth_test
dtree_depth_test_OBJECTS = \
"CMakeFiles/dtree_depth_test.dir/dtree_depth_test.c.o"

# External object files for target dtree_depth_test
dtree_depth_test_EXTERNAL_OBJECTS =

dtree_depth_test: src/CMakeFiles/dtree_depth_test.dir/dtree_depth_test.c.o
dtree_depth_test: src/CMakeFiles/dtree_depth_test.dir/build.make
dtree_depth_test: src/core/libcore.a
dtree_depth_test: src/models/libmodels.a
dtree_depth_test: /usr/lib64/libgsl.so
dtree_depth_test: src/core/libcore.a
dtree_depth_test: /usr/lib64/libgslcblas.so
dtree_depth_test: src/CMakeFiles/dtree_depth_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jamunk/Dev/CS461/CS461-HW2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ../dtree_depth_test"
	cd /home/jamunk/Dev/CS461/CS461-HW2/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dtree_depth_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/dtree_depth_test.dir/build: dtree_depth_test
.PHONY : src/CMakeFiles/dtree_depth_test.dir/build

src/CMakeFiles/dtree_depth_test.dir/clean:
	cd /home/jamunk/Dev/CS461/CS461-HW2/build/src && $(CMAKE_COMMAND) -P CMakeFiles/dtree_depth_test.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/dtree_depth_test.dir/clean

src/CMakeFiles/dtree_depth_test.dir/depend:
	cd /home/jamunk/Dev/CS461/CS461-HW2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jamunk/Dev/CS461/CS461-HW2 /home/jamunk/Dev/CS461/CS461-HW2/src /home/jamunk/Dev/CS461/CS461-HW2/build /home/jamunk/Dev/CS461/CS461-HW2/build/src /home/jamunk/Dev/CS461/CS461-HW2/build/src/CMakeFiles/dtree_depth_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/dtree_depth_test.dir/depend


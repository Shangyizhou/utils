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
CMAKE_SOURCE_DIR = /home/shang/code/C++/log4_test/zero

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/shang/code/C++/log4_test/zero/build

# Include any dependencies generated for this target.
include CMakeFiles/4-RollingFileAppender.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/4-RollingFileAppender.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/4-RollingFileAppender.dir/flags.make

CMakeFiles/4-RollingFileAppender.dir/4-RollingFileAppender.cpp.o: CMakeFiles/4-RollingFileAppender.dir/flags.make
CMakeFiles/4-RollingFileAppender.dir/4-RollingFileAppender.cpp.o: ../4-RollingFileAppender.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/shang/code/C++/log4_test/zero/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/4-RollingFileAppender.dir/4-RollingFileAppender.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/4-RollingFileAppender.dir/4-RollingFileAppender.cpp.o -c /home/shang/code/C++/log4_test/zero/4-RollingFileAppender.cpp

CMakeFiles/4-RollingFileAppender.dir/4-RollingFileAppender.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/4-RollingFileAppender.dir/4-RollingFileAppender.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/shang/code/C++/log4_test/zero/4-RollingFileAppender.cpp > CMakeFiles/4-RollingFileAppender.dir/4-RollingFileAppender.cpp.i

CMakeFiles/4-RollingFileAppender.dir/4-RollingFileAppender.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/4-RollingFileAppender.dir/4-RollingFileAppender.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/shang/code/C++/log4_test/zero/4-RollingFileAppender.cpp -o CMakeFiles/4-RollingFileAppender.dir/4-RollingFileAppender.cpp.s

CMakeFiles/4-RollingFileAppender.dir/4-RollingFileAppender.cpp.o.requires:

.PHONY : CMakeFiles/4-RollingFileAppender.dir/4-RollingFileAppender.cpp.o.requires

CMakeFiles/4-RollingFileAppender.dir/4-RollingFileAppender.cpp.o.provides: CMakeFiles/4-RollingFileAppender.dir/4-RollingFileAppender.cpp.o.requires
	$(MAKE) -f CMakeFiles/4-RollingFileAppender.dir/build.make CMakeFiles/4-RollingFileAppender.dir/4-RollingFileAppender.cpp.o.provides.build
.PHONY : CMakeFiles/4-RollingFileAppender.dir/4-RollingFileAppender.cpp.o.provides

CMakeFiles/4-RollingFileAppender.dir/4-RollingFileAppender.cpp.o.provides.build: CMakeFiles/4-RollingFileAppender.dir/4-RollingFileAppender.cpp.o


# Object files for target 4-RollingFileAppender
4__RollingFileAppender_OBJECTS = \
"CMakeFiles/4-RollingFileAppender.dir/4-RollingFileAppender.cpp.o"

# External object files for target 4-RollingFileAppender
4__RollingFileAppender_EXTERNAL_OBJECTS =

4-RollingFileAppender: CMakeFiles/4-RollingFileAppender.dir/4-RollingFileAppender.cpp.o
4-RollingFileAppender: CMakeFiles/4-RollingFileAppender.dir/build.make
4-RollingFileAppender: CMakeFiles/4-RollingFileAppender.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/shang/code/C++/log4_test/zero/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable 4-RollingFileAppender"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/4-RollingFileAppender.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/4-RollingFileAppender.dir/build: 4-RollingFileAppender

.PHONY : CMakeFiles/4-RollingFileAppender.dir/build

CMakeFiles/4-RollingFileAppender.dir/requires: CMakeFiles/4-RollingFileAppender.dir/4-RollingFileAppender.cpp.o.requires

.PHONY : CMakeFiles/4-RollingFileAppender.dir/requires

CMakeFiles/4-RollingFileAppender.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/4-RollingFileAppender.dir/cmake_clean.cmake
.PHONY : CMakeFiles/4-RollingFileAppender.dir/clean

CMakeFiles/4-RollingFileAppender.dir/depend:
	cd /home/shang/code/C++/log4_test/zero/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shang/code/C++/log4_test/zero /home/shang/code/C++/log4_test/zero /home/shang/code/C++/log4_test/zero/build /home/shang/code/C++/log4_test/zero/build /home/shang/code/C++/log4_test/zero/build/CMakeFiles/4-RollingFileAppender.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/4-RollingFileAppender.dir/depend


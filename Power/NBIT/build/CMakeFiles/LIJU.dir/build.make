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
CMAKE_SOURCE_DIR = /home/dds/ARSM/Power/NBIT

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dds/ARSM/Power/NBIT/build

# Include any dependencies generated for this target.
include CMakeFiles/LIJU.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/LIJU.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/LIJU.dir/flags.make

CMakeFiles/LIJU.dir/main.cpp.o: CMakeFiles/LIJU.dir/flags.make
CMakeFiles/LIJU.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dds/ARSM/Power/NBIT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/LIJU.dir/main.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LIJU.dir/main.cpp.o -c /home/dds/ARSM/Power/NBIT/main.cpp

CMakeFiles/LIJU.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LIJU.dir/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dds/ARSM/Power/NBIT/main.cpp > CMakeFiles/LIJU.dir/main.cpp.i

CMakeFiles/LIJU.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LIJU.dir/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dds/ARSM/Power/NBIT/main.cpp -o CMakeFiles/LIJU.dir/main.cpp.s

CMakeFiles/LIJU.dir/LIJU.cpp.o: CMakeFiles/LIJU.dir/flags.make
CMakeFiles/LIJU.dir/LIJU.cpp.o: ../LIJU.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dds/ARSM/Power/NBIT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/LIJU.dir/LIJU.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LIJU.dir/LIJU.cpp.o -c /home/dds/ARSM/Power/NBIT/LIJU.cpp

CMakeFiles/LIJU.dir/LIJU.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LIJU.dir/LIJU.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dds/ARSM/Power/NBIT/LIJU.cpp > CMakeFiles/LIJU.dir/LIJU.cpp.i

CMakeFiles/LIJU.dir/LIJU.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LIJU.dir/LIJU.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dds/ARSM/Power/NBIT/LIJU.cpp -o CMakeFiles/LIJU.dir/LIJU.cpp.s

# Object files for target LIJU
LIJU_OBJECTS = \
"CMakeFiles/LIJU.dir/main.cpp.o" \
"CMakeFiles/LIJU.dir/LIJU.cpp.o"

# External object files for target LIJU
LIJU_EXTERNAL_OBJECTS =

LIJU: CMakeFiles/LIJU.dir/main.cpp.o
LIJU: CMakeFiles/LIJU.dir/LIJU.cpp.o
LIJU: CMakeFiles/LIJU.dir/build.make
LIJU: CMakeFiles/LIJU.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dds/ARSM/Power/NBIT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable LIJU"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LIJU.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/LIJU.dir/build: LIJU

.PHONY : CMakeFiles/LIJU.dir/build

CMakeFiles/LIJU.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/LIJU.dir/cmake_clean.cmake
.PHONY : CMakeFiles/LIJU.dir/clean

CMakeFiles/LIJU.dir/depend:
	cd /home/dds/ARSM/Power/NBIT/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dds/ARSM/Power/NBIT /home/dds/ARSM/Power/NBIT /home/dds/ARSM/Power/NBIT/build /home/dds/ARSM/Power/NBIT/build /home/dds/ARSM/Power/NBIT/build/CMakeFiles/LIJU.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/LIJU.dir/depend


# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/classes/json

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/build

# Include any dependencies generated for this target.
include tests/abi/diag/CMakeFiles/abi_compat_diag.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/abi/diag/CMakeFiles/abi_compat_diag.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/abi/diag/CMakeFiles/abi_compat_diag.dir/progress.make

# Include the compile flags for this target's objects.
include tests/abi/diag/CMakeFiles/abi_compat_diag.dir/flags.make

tests/abi/diag/CMakeFiles/abi_compat_diag.dir/diag.cpp.o: tests/abi/diag/CMakeFiles/abi_compat_diag.dir/flags.make
tests/abi/diag/CMakeFiles/abi_compat_diag.dir/diag.cpp.o: /mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/classes/json/tests/abi/diag/diag.cpp
tests/abi/diag/CMakeFiles/abi_compat_diag.dir/diag.cpp.o: tests/abi/diag/CMakeFiles/abi_compat_diag.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/abi/diag/CMakeFiles/abi_compat_diag.dir/diag.cpp.o"
	cd /mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/build/tests/abi/diag && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/abi/diag/CMakeFiles/abi_compat_diag.dir/diag.cpp.o -MF CMakeFiles/abi_compat_diag.dir/diag.cpp.o.d -o CMakeFiles/abi_compat_diag.dir/diag.cpp.o -c /mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/classes/json/tests/abi/diag/diag.cpp

tests/abi/diag/CMakeFiles/abi_compat_diag.dir/diag.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/abi_compat_diag.dir/diag.cpp.i"
	cd /mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/build/tests/abi/diag && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/classes/json/tests/abi/diag/diag.cpp > CMakeFiles/abi_compat_diag.dir/diag.cpp.i

tests/abi/diag/CMakeFiles/abi_compat_diag.dir/diag.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/abi_compat_diag.dir/diag.cpp.s"
	cd /mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/build/tests/abi/diag && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/classes/json/tests/abi/diag/diag.cpp -o CMakeFiles/abi_compat_diag.dir/diag.cpp.s

# Object files for target abi_compat_diag
abi_compat_diag_OBJECTS = \
"CMakeFiles/abi_compat_diag.dir/diag.cpp.o"

# External object files for target abi_compat_diag
abi_compat_diag_EXTERNAL_OBJECTS =

tests/abi/diag/abi_compat_diag: tests/abi/diag/CMakeFiles/abi_compat_diag.dir/diag.cpp.o
tests/abi/diag/abi_compat_diag: tests/abi/diag/CMakeFiles/abi_compat_diag.dir/build.make
tests/abi/diag/abi_compat_diag: tests/abi/libabi_compat_main.a
tests/abi/diag/abi_compat_diag: tests/abi/diag/libabi_compat_diag_on.a
tests/abi/diag/abi_compat_diag: tests/abi/diag/libabi_compat_diag_off.a
tests/abi/diag/abi_compat_diag: tests/abi/diag/CMakeFiles/abi_compat_diag.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable abi_compat_diag"
	cd /mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/build/tests/abi/diag && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/abi_compat_diag.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/abi/diag/CMakeFiles/abi_compat_diag.dir/build: tests/abi/diag/abi_compat_diag
.PHONY : tests/abi/diag/CMakeFiles/abi_compat_diag.dir/build

tests/abi/diag/CMakeFiles/abi_compat_diag.dir/clean:
	cd /mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/build/tests/abi/diag && $(CMAKE_COMMAND) -P CMakeFiles/abi_compat_diag.dir/cmake_clean.cmake
.PHONY : tests/abi/diag/CMakeFiles/abi_compat_diag.dir/clean

tests/abi/diag/CMakeFiles/abi_compat_diag.dir/depend:
	cd /mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/classes/json /mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/classes/json/tests/abi/diag /mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/build /mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/build/tests/abi/diag /mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/build/tests/abi/diag/CMakeFiles/abi_compat_diag.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/abi/diag/CMakeFiles/abi_compat_diag.dir/depend


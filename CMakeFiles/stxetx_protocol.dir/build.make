# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_COMMAND = "A:/Program Files/CMake/bin/cmake.exe"

# The command to remove a file.
RM = "A:/Program Files/CMake/bin/cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = E:/STXETX_Protocol

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:/STXETX_Protocol

# Include any dependencies generated for this target.
include CMakeFiles/stxetx_protocol.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/stxetx_protocol.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/stxetx_protocol.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/stxetx_protocol.dir/flags.make

CMakeFiles/stxetx_protocol.dir/src/main.c.obj: CMakeFiles/stxetx_protocol.dir/flags.make
CMakeFiles/stxetx_protocol.dir/src/main.c.obj: CMakeFiles/stxetx_protocol.dir/includes_C.rsp
CMakeFiles/stxetx_protocol.dir/src/main.c.obj: src/main.c
CMakeFiles/stxetx_protocol.dir/src/main.c.obj: CMakeFiles/stxetx_protocol.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:/STXETX_Protocol/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/stxetx_protocol.dir/src/main.c.obj"
	C:/Mingw64/bin/gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/stxetx_protocol.dir/src/main.c.obj -MF CMakeFiles/stxetx_protocol.dir/src/main.c.obj.d -o CMakeFiles/stxetx_protocol.dir/src/main.c.obj -c E:/STXETX_Protocol/src/main.c

CMakeFiles/stxetx_protocol.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/stxetx_protocol.dir/src/main.c.i"
	C:/Mingw64/bin/gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E E:/STXETX_Protocol/src/main.c > CMakeFiles/stxetx_protocol.dir/src/main.c.i

CMakeFiles/stxetx_protocol.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/stxetx_protocol.dir/src/main.c.s"
	C:/Mingw64/bin/gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S E:/STXETX_Protocol/src/main.c -o CMakeFiles/stxetx_protocol.dir/src/main.c.s

CMakeFiles/stxetx_protocol.dir/src/stxetx_protocol.c.obj: CMakeFiles/stxetx_protocol.dir/flags.make
CMakeFiles/stxetx_protocol.dir/src/stxetx_protocol.c.obj: CMakeFiles/stxetx_protocol.dir/includes_C.rsp
CMakeFiles/stxetx_protocol.dir/src/stxetx_protocol.c.obj: src/stxetx_protocol.c
CMakeFiles/stxetx_protocol.dir/src/stxetx_protocol.c.obj: CMakeFiles/stxetx_protocol.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:/STXETX_Protocol/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/stxetx_protocol.dir/src/stxetx_protocol.c.obj"
	C:/Mingw64/bin/gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/stxetx_protocol.dir/src/stxetx_protocol.c.obj -MF CMakeFiles/stxetx_protocol.dir/src/stxetx_protocol.c.obj.d -o CMakeFiles/stxetx_protocol.dir/src/stxetx_protocol.c.obj -c E:/STXETX_Protocol/src/stxetx_protocol.c

CMakeFiles/stxetx_protocol.dir/src/stxetx_protocol.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/stxetx_protocol.dir/src/stxetx_protocol.c.i"
	C:/Mingw64/bin/gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E E:/STXETX_Protocol/src/stxetx_protocol.c > CMakeFiles/stxetx_protocol.dir/src/stxetx_protocol.c.i

CMakeFiles/stxetx_protocol.dir/src/stxetx_protocol.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/stxetx_protocol.dir/src/stxetx_protocol.c.s"
	C:/Mingw64/bin/gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S E:/STXETX_Protocol/src/stxetx_protocol.c -o CMakeFiles/stxetx_protocol.dir/src/stxetx_protocol.c.s

# Object files for target stxetx_protocol
stxetx_protocol_OBJECTS = \
"CMakeFiles/stxetx_protocol.dir/src/main.c.obj" \
"CMakeFiles/stxetx_protocol.dir/src/stxetx_protocol.c.obj"

# External object files for target stxetx_protocol
stxetx_protocol_EXTERNAL_OBJECTS =

bin/stxetx_protocol.exe: CMakeFiles/stxetx_protocol.dir/src/main.c.obj
bin/stxetx_protocol.exe: CMakeFiles/stxetx_protocol.dir/src/stxetx_protocol.c.obj
bin/stxetx_protocol.exe: CMakeFiles/stxetx_protocol.dir/build.make
bin/stxetx_protocol.exe: CMakeFiles/stxetx_protocol.dir/linkLibs.rsp
bin/stxetx_protocol.exe: CMakeFiles/stxetx_protocol.dir/objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=E:/STXETX_Protocol/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable bin/stxetx_protocol.exe"
	"A:/Program Files/CMake/bin/cmake.exe" -E rm -f CMakeFiles/stxetx_protocol.dir/objects.a
	C:/Mingw64/bin/ar.exe qc CMakeFiles/stxetx_protocol.dir/objects.a @CMakeFiles/stxetx_protocol.dir/objects1.rsp
	C:/Mingw64/bin/gcc.exe  -std=c99 -Wall -Wpedantic -ggdb -Wl,--whole-archive CMakeFiles/stxetx_protocol.dir/objects.a -Wl,--no-whole-archive -o bin/stxetx_protocol.exe -Wl,--out-implib,libstxetx_protocol.dll.a -Wl,--major-image-version,0,--minor-image-version,0 @CMakeFiles/stxetx_protocol.dir/linkLibs.rsp

# Rule to build all files generated by this target.
CMakeFiles/stxetx_protocol.dir/build: bin/stxetx_protocol.exe
.PHONY : CMakeFiles/stxetx_protocol.dir/build

CMakeFiles/stxetx_protocol.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/stxetx_protocol.dir/cmake_clean.cmake
.PHONY : CMakeFiles/stxetx_protocol.dir/clean

CMakeFiles/stxetx_protocol.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" E:/STXETX_Protocol E:/STXETX_Protocol E:/STXETX_Protocol E:/STXETX_Protocol E:/STXETX_Protocol/CMakeFiles/stxetx_protocol.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/stxetx_protocol.dir/depend


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
CMAKE_SOURCE_DIR = /home/j/ws/arena

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/j/ws/arena

# Include any dependencies generated for this target.
include CMakeFiles/Arena.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Arena.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Arena.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Arena.dir/flags.make

CMakeFiles/Arena.dir/render/button.c.o: CMakeFiles/Arena.dir/flags.make
CMakeFiles/Arena.dir/render/button.c.o: render/button.c
CMakeFiles/Arena.dir/render/button.c.o: CMakeFiles/Arena.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/j/ws/arena/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Arena.dir/render/button.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Arena.dir/render/button.c.o -MF CMakeFiles/Arena.dir/render/button.c.o.d -o CMakeFiles/Arena.dir/render/button.c.o -c /home/j/ws/arena/render/button.c

CMakeFiles/Arena.dir/render/button.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Arena.dir/render/button.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/j/ws/arena/render/button.c > CMakeFiles/Arena.dir/render/button.c.i

CMakeFiles/Arena.dir/render/button.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Arena.dir/render/button.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/j/ws/arena/render/button.c -o CMakeFiles/Arena.dir/render/button.c.s

CMakeFiles/Arena.dir/render/render_text.c.o: CMakeFiles/Arena.dir/flags.make
CMakeFiles/Arena.dir/render/render_text.c.o: render/render_text.c
CMakeFiles/Arena.dir/render/render_text.c.o: CMakeFiles/Arena.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/j/ws/arena/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Arena.dir/render/render_text.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Arena.dir/render/render_text.c.o -MF CMakeFiles/Arena.dir/render/render_text.c.o.d -o CMakeFiles/Arena.dir/render/render_text.c.o -c /home/j/ws/arena/render/render_text.c

CMakeFiles/Arena.dir/render/render_text.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Arena.dir/render/render_text.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/j/ws/arena/render/render_text.c > CMakeFiles/Arena.dir/render/render_text.c.i

CMakeFiles/Arena.dir/render/render_text.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Arena.dir/render/render_text.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/j/ws/arena/render/render_text.c -o CMakeFiles/Arena.dir/render/render_text.c.s

CMakeFiles/Arena.dir/mkb/mkb.c.o: CMakeFiles/Arena.dir/flags.make
CMakeFiles/Arena.dir/mkb/mkb.c.o: mkb/mkb.c
CMakeFiles/Arena.dir/mkb/mkb.c.o: CMakeFiles/Arena.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/j/ws/arena/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/Arena.dir/mkb/mkb.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Arena.dir/mkb/mkb.c.o -MF CMakeFiles/Arena.dir/mkb/mkb.c.o.d -o CMakeFiles/Arena.dir/mkb/mkb.c.o -c /home/j/ws/arena/mkb/mkb.c

CMakeFiles/Arena.dir/mkb/mkb.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Arena.dir/mkb/mkb.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/j/ws/arena/mkb/mkb.c > CMakeFiles/Arena.dir/mkb/mkb.c.i

CMakeFiles/Arena.dir/mkb/mkb.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Arena.dir/mkb/mkb.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/j/ws/arena/mkb/mkb.c -o CMakeFiles/Arena.dir/mkb/mkb.c.s

CMakeFiles/Arena.dir/gst/info.c.o: CMakeFiles/Arena.dir/flags.make
CMakeFiles/Arena.dir/gst/info.c.o: gst/info.c
CMakeFiles/Arena.dir/gst/info.c.o: CMakeFiles/Arena.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/j/ws/arena/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/Arena.dir/gst/info.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Arena.dir/gst/info.c.o -MF CMakeFiles/Arena.dir/gst/info.c.o.d -o CMakeFiles/Arena.dir/gst/info.c.o -c /home/j/ws/arena/gst/info.c

CMakeFiles/Arena.dir/gst/info.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Arena.dir/gst/info.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/j/ws/arena/gst/info.c > CMakeFiles/Arena.dir/gst/info.c.i

CMakeFiles/Arena.dir/gst/info.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Arena.dir/gst/info.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/j/ws/arena/gst/info.c -o CMakeFiles/Arena.dir/gst/info.c.s

CMakeFiles/Arena.dir/gst/fxs.c.o: CMakeFiles/Arena.dir/flags.make
CMakeFiles/Arena.dir/gst/fxs.c.o: gst/fxs.c
CMakeFiles/Arena.dir/gst/fxs.c.o: CMakeFiles/Arena.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/j/ws/arena/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/Arena.dir/gst/fxs.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Arena.dir/gst/fxs.c.o -MF CMakeFiles/Arena.dir/gst/fxs.c.o.d -o CMakeFiles/Arena.dir/gst/fxs.c.o -c /home/j/ws/arena/gst/fxs.c

CMakeFiles/Arena.dir/gst/fxs.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Arena.dir/gst/fxs.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/j/ws/arena/gst/fxs.c > CMakeFiles/Arena.dir/gst/fxs.c.i

CMakeFiles/Arena.dir/gst/fxs.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Arena.dir/gst/fxs.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/j/ws/arena/gst/fxs.c -o CMakeFiles/Arena.dir/gst/fxs.c.s

CMakeFiles/Arena.dir/gst/gst.c.o: CMakeFiles/Arena.dir/flags.make
CMakeFiles/Arena.dir/gst/gst.c.o: gst/gst.c
CMakeFiles/Arena.dir/gst/gst.c.o: CMakeFiles/Arena.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/j/ws/arena/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/Arena.dir/gst/gst.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Arena.dir/gst/gst.c.o -MF CMakeFiles/Arena.dir/gst/gst.c.o.d -o CMakeFiles/Arena.dir/gst/gst.c.o -c /home/j/ws/arena/gst/gst.c

CMakeFiles/Arena.dir/gst/gst.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Arena.dir/gst/gst.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/j/ws/arena/gst/gst.c > CMakeFiles/Arena.dir/gst/gst.c.i

CMakeFiles/Arena.dir/gst/gst.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Arena.dir/gst/gst.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/j/ws/arena/gst/gst.c -o CMakeFiles/Arena.dir/gst/gst.c.s

CMakeFiles/Arena.dir/gst/map.c.o: CMakeFiles/Arena.dir/flags.make
CMakeFiles/Arena.dir/gst/map.c.o: gst/map.c
CMakeFiles/Arena.dir/gst/map.c.o: CMakeFiles/Arena.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/j/ws/arena/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/Arena.dir/gst/map.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Arena.dir/gst/map.c.o -MF CMakeFiles/Arena.dir/gst/map.c.o.d -o CMakeFiles/Arena.dir/gst/map.c.o -c /home/j/ws/arena/gst/map.c

CMakeFiles/Arena.dir/gst/map.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Arena.dir/gst/map.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/j/ws/arena/gst/map.c > CMakeFiles/Arena.dir/gst/map.c.i

CMakeFiles/Arena.dir/gst/map.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Arena.dir/gst/map.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/j/ws/arena/gst/map.c -o CMakeFiles/Arena.dir/gst/map.c.s

CMakeFiles/Arena.dir/gst/units.c.o: CMakeFiles/Arena.dir/flags.make
CMakeFiles/Arena.dir/gst/units.c.o: gst/units.c
CMakeFiles/Arena.dir/gst/units.c.o: CMakeFiles/Arena.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/j/ws/arena/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/Arena.dir/gst/units.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Arena.dir/gst/units.c.o -MF CMakeFiles/Arena.dir/gst/units.c.o.d -o CMakeFiles/Arena.dir/gst/units.c.o -c /home/j/ws/arena/gst/units.c

CMakeFiles/Arena.dir/gst/units.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Arena.dir/gst/units.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/j/ws/arena/gst/units.c > CMakeFiles/Arena.dir/gst/units.c.i

CMakeFiles/Arena.dir/gst/units.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Arena.dir/gst/units.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/j/ws/arena/gst/units.c -o CMakeFiles/Arena.dir/gst/units.c.s

CMakeFiles/Arena.dir/gst/generate.c.o: CMakeFiles/Arena.dir/flags.make
CMakeFiles/Arena.dir/gst/generate.c.o: gst/generate.c
CMakeFiles/Arena.dir/gst/generate.c.o: CMakeFiles/Arena.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/j/ws/arena/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/Arena.dir/gst/generate.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Arena.dir/gst/generate.c.o -MF CMakeFiles/Arena.dir/gst/generate.c.o.d -o CMakeFiles/Arena.dir/gst/generate.c.o -c /home/j/ws/arena/gst/generate.c

CMakeFiles/Arena.dir/gst/generate.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Arena.dir/gst/generate.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/j/ws/arena/gst/generate.c > CMakeFiles/Arena.dir/gst/generate.c.i

CMakeFiles/Arena.dir/gst/generate.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Arena.dir/gst/generate.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/j/ws/arena/gst/generate.c -o CMakeFiles/Arena.dir/gst/generate.c.s

CMakeFiles/Arena.dir/hud/hud.c.o: CMakeFiles/Arena.dir/flags.make
CMakeFiles/Arena.dir/hud/hud.c.o: hud/hud.c
CMakeFiles/Arena.dir/hud/hud.c.o: CMakeFiles/Arena.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/j/ws/arena/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object CMakeFiles/Arena.dir/hud/hud.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Arena.dir/hud/hud.c.o -MF CMakeFiles/Arena.dir/hud/hud.c.o.d -o CMakeFiles/Arena.dir/hud/hud.c.o -c /home/j/ws/arena/hud/hud.c

CMakeFiles/Arena.dir/hud/hud.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Arena.dir/hud/hud.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/j/ws/arena/hud/hud.c > CMakeFiles/Arena.dir/hud/hud.c.i

CMakeFiles/Arena.dir/hud/hud.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Arena.dir/hud/hud.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/j/ws/arena/hud/hud.c -o CMakeFiles/Arena.dir/hud/hud.c.s

CMakeFiles/Arena.dir/hud/hud_views.c.o: CMakeFiles/Arena.dir/flags.make
CMakeFiles/Arena.dir/hud/hud_views.c.o: hud/hud_views.c
CMakeFiles/Arena.dir/hud/hud_views.c.o: CMakeFiles/Arena.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/j/ws/arena/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building C object CMakeFiles/Arena.dir/hud/hud_views.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Arena.dir/hud/hud_views.c.o -MF CMakeFiles/Arena.dir/hud/hud_views.c.o.d -o CMakeFiles/Arena.dir/hud/hud_views.c.o -c /home/j/ws/arena/hud/hud_views.c

CMakeFiles/Arena.dir/hud/hud_views.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Arena.dir/hud/hud_views.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/j/ws/arena/hud/hud_views.c > CMakeFiles/Arena.dir/hud/hud_views.c.i

CMakeFiles/Arena.dir/hud/hud_views.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Arena.dir/hud/hud_views.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/j/ws/arena/hud/hud_views.c -o CMakeFiles/Arena.dir/hud/hud_views.c.s

CMakeFiles/Arena.dir/json/jsonparse.c.o: CMakeFiles/Arena.dir/flags.make
CMakeFiles/Arena.dir/json/jsonparse.c.o: json/jsonparse.c
CMakeFiles/Arena.dir/json/jsonparse.c.o: CMakeFiles/Arena.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/j/ws/arena/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building C object CMakeFiles/Arena.dir/json/jsonparse.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Arena.dir/json/jsonparse.c.o -MF CMakeFiles/Arena.dir/json/jsonparse.c.o.d -o CMakeFiles/Arena.dir/json/jsonparse.c.o -c /home/j/ws/arena/json/jsonparse.c

CMakeFiles/Arena.dir/json/jsonparse.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Arena.dir/json/jsonparse.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/j/ws/arena/json/jsonparse.c > CMakeFiles/Arena.dir/json/jsonparse.c.i

CMakeFiles/Arena.dir/json/jsonparse.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Arena.dir/json/jsonparse.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/j/ws/arena/json/jsonparse.c -o CMakeFiles/Arena.dir/json/jsonparse.c.s

CMakeFiles/Arena.dir/net/net.c.o: CMakeFiles/Arena.dir/flags.make
CMakeFiles/Arena.dir/net/net.c.o: net/net.c
CMakeFiles/Arena.dir/net/net.c.o: CMakeFiles/Arena.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/j/ws/arena/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building C object CMakeFiles/Arena.dir/net/net.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Arena.dir/net/net.c.o -MF CMakeFiles/Arena.dir/net/net.c.o.d -o CMakeFiles/Arena.dir/net/net.c.o -c /home/j/ws/arena/net/net.c

CMakeFiles/Arena.dir/net/net.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Arena.dir/net/net.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/j/ws/arena/net/net.c > CMakeFiles/Arena.dir/net/net.c.i

CMakeFiles/Arena.dir/net/net.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Arena.dir/net/net.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/j/ws/arena/net/net.c -o CMakeFiles/Arena.dir/net/net.c.s

CMakeFiles/Arena.dir/umath/vec.c.o: CMakeFiles/Arena.dir/flags.make
CMakeFiles/Arena.dir/umath/vec.c.o: umath/vec.c
CMakeFiles/Arena.dir/umath/vec.c.o: CMakeFiles/Arena.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/j/ws/arena/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building C object CMakeFiles/Arena.dir/umath/vec.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Arena.dir/umath/vec.c.o -MF CMakeFiles/Arena.dir/umath/vec.c.o.d -o CMakeFiles/Arena.dir/umath/vec.c.o -c /home/j/ws/arena/umath/vec.c

CMakeFiles/Arena.dir/umath/vec.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Arena.dir/umath/vec.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/j/ws/arena/umath/vec.c > CMakeFiles/Arena.dir/umath/vec.c.i

CMakeFiles/Arena.dir/umath/vec.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Arena.dir/umath/vec.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/j/ws/arena/umath/vec.c -o CMakeFiles/Arena.dir/umath/vec.c.s

CMakeFiles/Arena.dir/umath/intersect.c.o: CMakeFiles/Arena.dir/flags.make
CMakeFiles/Arena.dir/umath/intersect.c.o: umath/intersect.c
CMakeFiles/Arena.dir/umath/intersect.c.o: CMakeFiles/Arena.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/j/ws/arena/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building C object CMakeFiles/Arena.dir/umath/intersect.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Arena.dir/umath/intersect.c.o -MF CMakeFiles/Arena.dir/umath/intersect.c.o.d -o CMakeFiles/Arena.dir/umath/intersect.c.o -c /home/j/ws/arena/umath/intersect.c

CMakeFiles/Arena.dir/umath/intersect.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Arena.dir/umath/intersect.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/j/ws/arena/umath/intersect.c > CMakeFiles/Arena.dir/umath/intersect.c.i

CMakeFiles/Arena.dir/umath/intersect.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Arena.dir/umath/intersect.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/j/ws/arena/umath/intersect.c -o CMakeFiles/Arena.dir/umath/intersect.c.s

CMakeFiles/Arena.dir/test/test.c.o: CMakeFiles/Arena.dir/flags.make
CMakeFiles/Arena.dir/test/test.c.o: test/test.c
CMakeFiles/Arena.dir/test/test.c.o: CMakeFiles/Arena.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/j/ws/arena/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building C object CMakeFiles/Arena.dir/test/test.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Arena.dir/test/test.c.o -MF CMakeFiles/Arena.dir/test/test.c.o.d -o CMakeFiles/Arena.dir/test/test.c.o -c /home/j/ws/arena/test/test.c

CMakeFiles/Arena.dir/test/test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Arena.dir/test/test.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/j/ws/arena/test/test.c > CMakeFiles/Arena.dir/test/test.c.i

CMakeFiles/Arena.dir/test/test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Arena.dir/test/test.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/j/ws/arena/test/test.c -o CMakeFiles/Arena.dir/test/test.c.s

CMakeFiles/Arena.dir/test/test_gst.c.o: CMakeFiles/Arena.dir/flags.make
CMakeFiles/Arena.dir/test/test_gst.c.o: test/test_gst.c
CMakeFiles/Arena.dir/test/test_gst.c.o: CMakeFiles/Arena.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/j/ws/arena/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Building C object CMakeFiles/Arena.dir/test/test_gst.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Arena.dir/test/test_gst.c.o -MF CMakeFiles/Arena.dir/test/test_gst.c.o.d -o CMakeFiles/Arena.dir/test/test_gst.c.o -c /home/j/ws/arena/test/test_gst.c

CMakeFiles/Arena.dir/test/test_gst.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Arena.dir/test/test_gst.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/j/ws/arena/test/test_gst.c > CMakeFiles/Arena.dir/test/test_gst.c.i

CMakeFiles/Arena.dir/test/test_gst.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Arena.dir/test/test_gst.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/j/ws/arena/test/test_gst.c -o CMakeFiles/Arena.dir/test/test_gst.c.s

CMakeFiles/Arena.dir/main.c.o: CMakeFiles/Arena.dir/flags.make
CMakeFiles/Arena.dir/main.c.o: main.c
CMakeFiles/Arena.dir/main.c.o: CMakeFiles/Arena.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/j/ws/arena/CMakeFiles --progress-num=$(CMAKE_PROGRESS_18) "Building C object CMakeFiles/Arena.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Arena.dir/main.c.o -MF CMakeFiles/Arena.dir/main.c.o.d -o CMakeFiles/Arena.dir/main.c.o -c /home/j/ws/arena/main.c

CMakeFiles/Arena.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Arena.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/j/ws/arena/main.c > CMakeFiles/Arena.dir/main.c.i

CMakeFiles/Arena.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Arena.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/j/ws/arena/main.c -o CMakeFiles/Arena.dir/main.c.s

# Object files for target Arena
Arena_OBJECTS = \
"CMakeFiles/Arena.dir/render/button.c.o" \
"CMakeFiles/Arena.dir/render/render_text.c.o" \
"CMakeFiles/Arena.dir/mkb/mkb.c.o" \
"CMakeFiles/Arena.dir/gst/info.c.o" \
"CMakeFiles/Arena.dir/gst/fxs.c.o" \
"CMakeFiles/Arena.dir/gst/gst.c.o" \
"CMakeFiles/Arena.dir/gst/map.c.o" \
"CMakeFiles/Arena.dir/gst/units.c.o" \
"CMakeFiles/Arena.dir/gst/generate.c.o" \
"CMakeFiles/Arena.dir/hud/hud.c.o" \
"CMakeFiles/Arena.dir/hud/hud_views.c.o" \
"CMakeFiles/Arena.dir/json/jsonparse.c.o" \
"CMakeFiles/Arena.dir/net/net.c.o" \
"CMakeFiles/Arena.dir/umath/vec.c.o" \
"CMakeFiles/Arena.dir/umath/intersect.c.o" \
"CMakeFiles/Arena.dir/test/test.c.o" \
"CMakeFiles/Arena.dir/test/test_gst.c.o" \
"CMakeFiles/Arena.dir/main.c.o"

# External object files for target Arena
Arena_EXTERNAL_OBJECTS =

build/Arena: CMakeFiles/Arena.dir/render/button.c.o
build/Arena: CMakeFiles/Arena.dir/render/render_text.c.o
build/Arena: CMakeFiles/Arena.dir/mkb/mkb.c.o
build/Arena: CMakeFiles/Arena.dir/gst/info.c.o
build/Arena: CMakeFiles/Arena.dir/gst/fxs.c.o
build/Arena: CMakeFiles/Arena.dir/gst/gst.c.o
build/Arena: CMakeFiles/Arena.dir/gst/map.c.o
build/Arena: CMakeFiles/Arena.dir/gst/units.c.o
build/Arena: CMakeFiles/Arena.dir/gst/generate.c.o
build/Arena: CMakeFiles/Arena.dir/hud/hud.c.o
build/Arena: CMakeFiles/Arena.dir/hud/hud_views.c.o
build/Arena: CMakeFiles/Arena.dir/json/jsonparse.c.o
build/Arena: CMakeFiles/Arena.dir/net/net.c.o
build/Arena: CMakeFiles/Arena.dir/umath/vec.c.o
build/Arena: CMakeFiles/Arena.dir/umath/intersect.c.o
build/Arena: CMakeFiles/Arena.dir/test/test.c.o
build/Arena: CMakeFiles/Arena.dir/test/test_gst.c.o
build/Arena: CMakeFiles/Arena.dir/main.c.o
build/Arena: CMakeFiles/Arena.dir/build.make
build/Arena: CMakeFiles/Arena.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/j/ws/arena/CMakeFiles --progress-num=$(CMAKE_PROGRESS_19) "Linking C executable build/Arena"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Arena.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Arena.dir/build: build/Arena
.PHONY : CMakeFiles/Arena.dir/build

CMakeFiles/Arena.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Arena.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Arena.dir/clean

CMakeFiles/Arena.dir/depend:
	cd /home/j/ws/arena && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/j/ws/arena /home/j/ws/arena /home/j/ws/arena /home/j/ws/arena /home/j/ws/arena/CMakeFiles/Arena.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Arena.dir/depend


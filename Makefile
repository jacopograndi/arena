# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.18

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\jaksk\Desktop\dev\_cgames\descript

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\jaksk\Desktop\dev\_cgames\descript

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	"C:\Program Files\CMake\bin\cmake-gui.exe" -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	"C:\Program Files\CMake\bin\cmake.exe" --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start C:\Users\jaksk\Desktop\dev\_cgames\descript\CMakeFiles C:\Users\jaksk\Desktop\dev\_cgames\descript\\CMakeFiles\progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start C:\Users\jaksk\Desktop\dev\_cgames\descript\CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles\Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named Arena

# Build rule for target.
Arena: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Makefile2 Arena
.PHONY : Arena

# fast build rule for target.
Arena/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/build
.PHONY : Arena/fast

gst/fxs.obj: gst/fxs.c.obj

.PHONY : gst/fxs.obj

# target to build an object file
gst/fxs.c.obj:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/gst/fxs.c.obj
.PHONY : gst/fxs.c.obj

gst/fxs.i: gst/fxs.c.i

.PHONY : gst/fxs.i

# target to preprocess a source file
gst/fxs.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/gst/fxs.c.i
.PHONY : gst/fxs.c.i

gst/fxs.s: gst/fxs.c.s

.PHONY : gst/fxs.s

# target to generate assembly for a file
gst/fxs.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/gst/fxs.c.s
.PHONY : gst/fxs.c.s

gst/gst.obj: gst/gst.c.obj

.PHONY : gst/gst.obj

# target to build an object file
gst/gst.c.obj:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/gst/gst.c.obj
.PHONY : gst/gst.c.obj

gst/gst.i: gst/gst.c.i

.PHONY : gst/gst.i

# target to preprocess a source file
gst/gst.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/gst/gst.c.i
.PHONY : gst/gst.c.i

gst/gst.s: gst/gst.c.s

.PHONY : gst/gst.s

# target to generate assembly for a file
gst/gst.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/gst/gst.c.s
.PHONY : gst/gst.c.s

gst/info.obj: gst/info.c.obj

.PHONY : gst/info.obj

# target to build an object file
gst/info.c.obj:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/gst/info.c.obj
.PHONY : gst/info.c.obj

gst/info.i: gst/info.c.i

.PHONY : gst/info.i

# target to preprocess a source file
gst/info.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/gst/info.c.i
.PHONY : gst/info.c.i

gst/info.s: gst/info.c.s

.PHONY : gst/info.s

# target to generate assembly for a file
gst/info.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/gst/info.c.s
.PHONY : gst/info.c.s

gst/map.obj: gst/map.c.obj

.PHONY : gst/map.obj

# target to build an object file
gst/map.c.obj:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/gst/map.c.obj
.PHONY : gst/map.c.obj

gst/map.i: gst/map.c.i

.PHONY : gst/map.i

# target to preprocess a source file
gst/map.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/gst/map.c.i
.PHONY : gst/map.c.i

gst/map.s: gst/map.c.s

.PHONY : gst/map.s

# target to generate assembly for a file
gst/map.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/gst/map.c.s
.PHONY : gst/map.c.s

gst/units.obj: gst/units.c.obj

.PHONY : gst/units.obj

# target to build an object file
gst/units.c.obj:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/gst/units.c.obj
.PHONY : gst/units.c.obj

gst/units.i: gst/units.c.i

.PHONY : gst/units.i

# target to preprocess a source file
gst/units.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/gst/units.c.i
.PHONY : gst/units.c.i

gst/units.s: gst/units.c.s

.PHONY : gst/units.s

# target to generate assembly for a file
gst/units.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/gst/units.c.s
.PHONY : gst/units.c.s

hud/hud.obj: hud/hud.c.obj

.PHONY : hud/hud.obj

# target to build an object file
hud/hud.c.obj:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/hud/hud.c.obj
.PHONY : hud/hud.c.obj

hud/hud.i: hud/hud.c.i

.PHONY : hud/hud.i

# target to preprocess a source file
hud/hud.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/hud/hud.c.i
.PHONY : hud/hud.c.i

hud/hud.s: hud/hud.c.s

.PHONY : hud/hud.s

# target to generate assembly for a file
hud/hud.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/hud/hud.c.s
.PHONY : hud/hud.c.s

hud/hud_views.obj: hud/hud_views.c.obj

.PHONY : hud/hud_views.obj

# target to build an object file
hud/hud_views.c.obj:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/hud/hud_views.c.obj
.PHONY : hud/hud_views.c.obj

hud/hud_views.i: hud/hud_views.c.i

.PHONY : hud/hud_views.i

# target to preprocess a source file
hud/hud_views.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/hud/hud_views.c.i
.PHONY : hud/hud_views.c.i

hud/hud_views.s: hud/hud_views.c.s

.PHONY : hud/hud_views.s

# target to generate assembly for a file
hud/hud_views.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/hud/hud_views.c.s
.PHONY : hud/hud_views.c.s

json/jsonparse.obj: json/jsonparse.c.obj

.PHONY : json/jsonparse.obj

# target to build an object file
json/jsonparse.c.obj:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/json/jsonparse.c.obj
.PHONY : json/jsonparse.c.obj

json/jsonparse.i: json/jsonparse.c.i

.PHONY : json/jsonparse.i

# target to preprocess a source file
json/jsonparse.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/json/jsonparse.c.i
.PHONY : json/jsonparse.c.i

json/jsonparse.s: json/jsonparse.c.s

.PHONY : json/jsonparse.s

# target to generate assembly for a file
json/jsonparse.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/json/jsonparse.c.s
.PHONY : json/jsonparse.c.s

main.obj: main.c.obj

.PHONY : main.obj

# target to build an object file
main.c.obj:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/main.c.obj
.PHONY : main.c.obj

main.i: main.c.i

.PHONY : main.i

# target to preprocess a source file
main.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/main.c.i
.PHONY : main.c.i

main.s: main.c.s

.PHONY : main.s

# target to generate assembly for a file
main.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/main.c.s
.PHONY : main.c.s

mkb/mkb.obj: mkb/mkb.c.obj

.PHONY : mkb/mkb.obj

# target to build an object file
mkb/mkb.c.obj:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/mkb/mkb.c.obj
.PHONY : mkb/mkb.c.obj

mkb/mkb.i: mkb/mkb.c.i

.PHONY : mkb/mkb.i

# target to preprocess a source file
mkb/mkb.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/mkb/mkb.c.i
.PHONY : mkb/mkb.c.i

mkb/mkb.s: mkb/mkb.c.s

.PHONY : mkb/mkb.s

# target to generate assembly for a file
mkb/mkb.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/mkb/mkb.c.s
.PHONY : mkb/mkb.c.s

net/net.obj: net/net.c.obj

.PHONY : net/net.obj

# target to build an object file
net/net.c.obj:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/net/net.c.obj
.PHONY : net/net.c.obj

net/net.i: net/net.c.i

.PHONY : net/net.i

# target to preprocess a source file
net/net.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/net/net.c.i
.PHONY : net/net.c.i

net/net.s: net/net.c.s

.PHONY : net/net.s

# target to generate assembly for a file
net/net.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/net/net.c.s
.PHONY : net/net.c.s

render/button.obj: render/button.c.obj

.PHONY : render/button.obj

# target to build an object file
render/button.c.obj:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/render/button.c.obj
.PHONY : render/button.c.obj

render/button.i: render/button.c.i

.PHONY : render/button.i

# target to preprocess a source file
render/button.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/render/button.c.i
.PHONY : render/button.c.i

render/button.s: render/button.c.s

.PHONY : render/button.s

# target to generate assembly for a file
render/button.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/render/button.c.s
.PHONY : render/button.c.s

render/render_text.obj: render/render_text.c.obj

.PHONY : render/render_text.obj

# target to build an object file
render/render_text.c.obj:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/render/render_text.c.obj
.PHONY : render/render_text.c.obj

render/render_text.i: render/render_text.c.i

.PHONY : render/render_text.i

# target to preprocess a source file
render/render_text.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/render/render_text.c.i
.PHONY : render/render_text.c.i

render/render_text.s: render/render_text.c.s

.PHONY : render/render_text.s

# target to generate assembly for a file
render/render_text.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/render/render_text.c.s
.PHONY : render/render_text.c.s

test/test.obj: test/test.c.obj

.PHONY : test/test.obj

# target to build an object file
test/test.c.obj:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/test/test.c.obj
.PHONY : test/test.c.obj

test/test.i: test/test.c.i

.PHONY : test/test.i

# target to preprocess a source file
test/test.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/test/test.c.i
.PHONY : test/test.c.i

test/test.s: test/test.c.s

.PHONY : test/test.s

# target to generate assembly for a file
test/test.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/test/test.c.s
.PHONY : test/test.c.s

test/test_gst.obj: test/test_gst.c.obj

.PHONY : test/test_gst.obj

# target to build an object file
test/test_gst.c.obj:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/test/test_gst.c.obj
.PHONY : test/test_gst.c.obj

test/test_gst.i: test/test_gst.c.i

.PHONY : test/test_gst.i

# target to preprocess a source file
test/test_gst.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/test/test_gst.c.i
.PHONY : test/test_gst.c.i

test/test_gst.s: test/test_gst.c.s

.PHONY : test/test_gst.s

# target to generate assembly for a file
test/test_gst.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/test/test_gst.c.s
.PHONY : test/test_gst.c.s

umath/intersect.obj: umath/intersect.c.obj

.PHONY : umath/intersect.obj

# target to build an object file
umath/intersect.c.obj:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/umath/intersect.c.obj
.PHONY : umath/intersect.c.obj

umath/intersect.i: umath/intersect.c.i

.PHONY : umath/intersect.i

# target to preprocess a source file
umath/intersect.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/umath/intersect.c.i
.PHONY : umath/intersect.c.i

umath/intersect.s: umath/intersect.c.s

.PHONY : umath/intersect.s

# target to generate assembly for a file
umath/intersect.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/umath/intersect.c.s
.PHONY : umath/intersect.c.s

umath/vec.obj: umath/vec.c.obj

.PHONY : umath/vec.obj

# target to build an object file
umath/vec.c.obj:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/umath/vec.c.obj
.PHONY : umath/vec.c.obj

umath/vec.i: umath/vec.c.i

.PHONY : umath/vec.i

# target to preprocess a source file
umath/vec.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/umath/vec.c.i
.PHONY : umath/vec.c.i

umath/vec.s: umath/vec.c.s

.PHONY : umath/vec.s

# target to generate assembly for a file
umath/vec.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles\Arena.dir\build.make CMakeFiles/Arena.dir/umath/vec.c.s
.PHONY : umath/vec.c.s

# Help Target
help:
	@echo The following are some of the valid targets for this Makefile:
	@echo ... all (the default if no target is provided)
	@echo ... clean
	@echo ... depend
	@echo ... edit_cache
	@echo ... rebuild_cache
	@echo ... Arena
	@echo ... gst/fxs.obj
	@echo ... gst/fxs.i
	@echo ... gst/fxs.s
	@echo ... gst/gst.obj
	@echo ... gst/gst.i
	@echo ... gst/gst.s
	@echo ... gst/info.obj
	@echo ... gst/info.i
	@echo ... gst/info.s
	@echo ... gst/map.obj
	@echo ... gst/map.i
	@echo ... gst/map.s
	@echo ... gst/units.obj
	@echo ... gst/units.i
	@echo ... gst/units.s
	@echo ... hud/hud.obj
	@echo ... hud/hud.i
	@echo ... hud/hud.s
	@echo ... hud/hud_views.obj
	@echo ... hud/hud_views.i
	@echo ... hud/hud_views.s
	@echo ... json/jsonparse.obj
	@echo ... json/jsonparse.i
	@echo ... json/jsonparse.s
	@echo ... main.obj
	@echo ... main.i
	@echo ... main.s
	@echo ... mkb/mkb.obj
	@echo ... mkb/mkb.i
	@echo ... mkb/mkb.s
	@echo ... net/net.obj
	@echo ... net/net.i
	@echo ... net/net.s
	@echo ... render/button.obj
	@echo ... render/button.i
	@echo ... render/button.s
	@echo ... render/render_text.obj
	@echo ... render/render_text.i
	@echo ... render/render_text.s
	@echo ... test/test.obj
	@echo ... test/test.i
	@echo ... test/test.s
	@echo ... test/test_gst.obj
	@echo ... test/test_gst.i
	@echo ... test/test_gst.s
	@echo ... umath/intersect.obj
	@echo ... umath/intersect.i
	@echo ... umath/intersect.s
	@echo ... umath/vec.obj
	@echo ... umath/vec.i
	@echo ... umath/vec.s
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles\Makefile.cmake 0
.PHONY : cmake_check_build_system


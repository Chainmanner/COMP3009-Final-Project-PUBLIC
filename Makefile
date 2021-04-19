# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/chainmanner/files/school/assignments/3009/final_project/COMP3009-Final-Project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chainmanner/files/school/assignments/3009/final_project/COMP3009-Final-Project

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/chainmanner/files/school/assignments/3009/final_project/COMP3009-Final-Project/CMakeFiles /home/chainmanner/files/school/assignments/3009/final_project/COMP3009-Final-Project//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/chainmanner/files/school/assignments/3009/final_project/COMP3009-Final-Project/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named COMP3009A_Final_Project

# Build rule for target.
COMP3009A_Final_Project: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 COMP3009A_Final_Project
.PHONY : COMP3009A_Final_Project

# fast build rule for target.
COMP3009A_Final_Project/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/COMP3009A_Final_Project.dir/build.make CMakeFiles/COMP3009A_Final_Project.dir/build
.PHONY : COMP3009A_Final_Project/fast

src/baseobject.o: src/baseobject.cpp.o
.PHONY : src/baseobject.o

# target to build an object file
src/baseobject.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/COMP3009A_Final_Project.dir/build.make CMakeFiles/COMP3009A_Final_Project.dir/src/baseobject.cpp.o
.PHONY : src/baseobject.cpp.o

src/baseobject.i: src/baseobject.cpp.i
.PHONY : src/baseobject.i

# target to preprocess a source file
src/baseobject.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/COMP3009A_Final_Project.dir/build.make CMakeFiles/COMP3009A_Final_Project.dir/src/baseobject.cpp.i
.PHONY : src/baseobject.cpp.i

src/baseobject.s: src/baseobject.cpp.s
.PHONY : src/baseobject.s

# target to generate assembly for a file
src/baseobject.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/COMP3009A_Final_Project.dir/build.make CMakeFiles/COMP3009A_Final_Project.dir/src/baseobject.cpp.s
.PHONY : src/baseobject.cpp.s

src/camera.o: src/camera.cpp.o
.PHONY : src/camera.o

# target to build an object file
src/camera.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/COMP3009A_Final_Project.dir/build.make CMakeFiles/COMP3009A_Final_Project.dir/src/camera.cpp.o
.PHONY : src/camera.cpp.o

src/camera.i: src/camera.cpp.i
.PHONY : src/camera.i

# target to preprocess a source file
src/camera.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/COMP3009A_Final_Project.dir/build.make CMakeFiles/COMP3009A_Final_Project.dir/src/camera.cpp.i
.PHONY : src/camera.cpp.i

src/camera.s: src/camera.cpp.s
.PHONY : src/camera.s

# target to generate assembly for a file
src/camera.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/COMP3009A_Final_Project.dir/build.make CMakeFiles/COMP3009A_Final_Project.dir/src/camera.cpp.s
.PHONY : src/camera.cpp.s

src/main.o: src/main.cpp.o
.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/COMP3009A_Final_Project.dir/build.make CMakeFiles/COMP3009A_Final_Project.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i
.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/COMP3009A_Final_Project.dir/build.make CMakeFiles/COMP3009A_Final_Project.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s
.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/COMP3009A_Final_Project.dir/build.make CMakeFiles/COMP3009A_Final_Project.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

src/polyobject.o: src/polyobject.cpp.o
.PHONY : src/polyobject.o

# target to build an object file
src/polyobject.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/COMP3009A_Final_Project.dir/build.make CMakeFiles/COMP3009A_Final_Project.dir/src/polyobject.cpp.o
.PHONY : src/polyobject.cpp.o

src/polyobject.i: src/polyobject.cpp.i
.PHONY : src/polyobject.i

# target to preprocess a source file
src/polyobject.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/COMP3009A_Final_Project.dir/build.make CMakeFiles/COMP3009A_Final_Project.dir/src/polyobject.cpp.i
.PHONY : src/polyobject.cpp.i

src/polyobject.s: src/polyobject.cpp.s
.PHONY : src/polyobject.s

# target to generate assembly for a file
src/polyobject.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/COMP3009A_Final_Project.dir/build.make CMakeFiles/COMP3009A_Final_Project.dir/src/polyobject.cpp.s
.PHONY : src/polyobject.cpp.s

src/stb_image.o: src/stb_image.cpp.o
.PHONY : src/stb_image.o

# target to build an object file
src/stb_image.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/COMP3009A_Final_Project.dir/build.make CMakeFiles/COMP3009A_Final_Project.dir/src/stb_image.cpp.o
.PHONY : src/stb_image.cpp.o

src/stb_image.i: src/stb_image.cpp.i
.PHONY : src/stb_image.i

# target to preprocess a source file
src/stb_image.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/COMP3009A_Final_Project.dir/build.make CMakeFiles/COMP3009A_Final_Project.dir/src/stb_image.cpp.i
.PHONY : src/stb_image.cpp.i

src/stb_image.s: src/stb_image.cpp.s
.PHONY : src/stb_image.s

# target to generate assembly for a file
src/stb_image.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/COMP3009A_Final_Project.dir/build.make CMakeFiles/COMP3009A_Final_Project.dir/src/stb_image.cpp.s
.PHONY : src/stb_image.cpp.s

src/utils.o: src/utils.cpp.o
.PHONY : src/utils.o

# target to build an object file
src/utils.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/COMP3009A_Final_Project.dir/build.make CMakeFiles/COMP3009A_Final_Project.dir/src/utils.cpp.o
.PHONY : src/utils.cpp.o

src/utils.i: src/utils.cpp.i
.PHONY : src/utils.i

# target to preprocess a source file
src/utils.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/COMP3009A_Final_Project.dir/build.make CMakeFiles/COMP3009A_Final_Project.dir/src/utils.cpp.i
.PHONY : src/utils.cpp.i

src/utils.s: src/utils.cpp.s
.PHONY : src/utils.s

# target to generate assembly for a file
src/utils.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/COMP3009A_Final_Project.dir/build.make CMakeFiles/COMP3009A_Final_Project.dir/src/utils.cpp.s
.PHONY : src/utils.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... COMP3009A_Final_Project"
	@echo "... src/baseobject.o"
	@echo "... src/baseobject.i"
	@echo "... src/baseobject.s"
	@echo "... src/camera.o"
	@echo "... src/camera.i"
	@echo "... src/camera.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
	@echo "... src/polyobject.o"
	@echo "... src/polyobject.i"
	@echo "... src/polyobject.s"
	@echo "... src/stb_image.o"
	@echo "... src/stb_image.i"
	@echo "... src/stb_image.s"
	@echo "... src/utils.o"
	@echo "... src/utils.i"
	@echo "... src/utils.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system


# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mmk6qa/B1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mmk6qa/B1

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target install
install: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install

# Special rule for the target install
install/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install/fast

# Special rule for the target install/local
install/local: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local

# Special rule for the target install/local
install/local/fast: install/local
.PHONY : install/local/fast

# Special rule for the target install/strip
install/strip: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip

# Special rule for the target install/strip
install/strip/fast: install/strip
.PHONY : install/strip/fast

# Special rule for the target list_install_components
list_install_components:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Available install components are: \"Unspecified\""
.PHONY : list_install_components

# Special rule for the target list_install_components
list_install_components/fast: list_install_components
.PHONY : list_install_components/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/mmk6qa/B1/CMakeFiles /home/mmk6qa/B1/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/mmk6qa/B1/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named B1

# Build rule for target.
B1: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 B1
.PHONY : B1

# fast build rule for target.
B1/fast:
	$(MAKE) -f CMakeFiles/B1.dir/build.make CMakeFiles/B1.dir/build
.PHONY : B1/fast

#=============================================================================
# Target rules for targets named exampleB1

# Build rule for target.
exampleB1: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 exampleB1
.PHONY : exampleB1

# fast build rule for target.
exampleB1/fast:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/build
.PHONY : exampleB1/fast

exampleB1.o: exampleB1.cc.o
.PHONY : exampleB1.o

# target to build an object file
exampleB1.cc.o:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/exampleB1.cc.o
.PHONY : exampleB1.cc.o

exampleB1.i: exampleB1.cc.i
.PHONY : exampleB1.i

# target to preprocess a source file
exampleB1.cc.i:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/exampleB1.cc.i
.PHONY : exampleB1.cc.i

exampleB1.s: exampleB1.cc.s
.PHONY : exampleB1.s

# target to generate assembly for a file
exampleB1.cc.s:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/exampleB1.cc.s
.PHONY : exampleB1.cc.s

src/B1ActionInitialization.o: src/B1ActionInitialization.cc.o
.PHONY : src/B1ActionInitialization.o

# target to build an object file
src/B1ActionInitialization.cc.o:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/B1ActionInitialization.cc.o
.PHONY : src/B1ActionInitialization.cc.o

src/B1ActionInitialization.i: src/B1ActionInitialization.cc.i
.PHONY : src/B1ActionInitialization.i

# target to preprocess a source file
src/B1ActionInitialization.cc.i:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/B1ActionInitialization.cc.i
.PHONY : src/B1ActionInitialization.cc.i

src/B1ActionInitialization.s: src/B1ActionInitialization.cc.s
.PHONY : src/B1ActionInitialization.s

# target to generate assembly for a file
src/B1ActionInitialization.cc.s:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/B1ActionInitialization.cc.s
.PHONY : src/B1ActionInitialization.cc.s

src/B1DetectorConstruction.o: src/B1DetectorConstruction.cc.o
.PHONY : src/B1DetectorConstruction.o

# target to build an object file
src/B1DetectorConstruction.cc.o:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/B1DetectorConstruction.cc.o
.PHONY : src/B1DetectorConstruction.cc.o

src/B1DetectorConstruction.i: src/B1DetectorConstruction.cc.i
.PHONY : src/B1DetectorConstruction.i

# target to preprocess a source file
src/B1DetectorConstruction.cc.i:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/B1DetectorConstruction.cc.i
.PHONY : src/B1DetectorConstruction.cc.i

src/B1DetectorConstruction.s: src/B1DetectorConstruction.cc.s
.PHONY : src/B1DetectorConstruction.s

# target to generate assembly for a file
src/B1DetectorConstruction.cc.s:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/B1DetectorConstruction.cc.s
.PHONY : src/B1DetectorConstruction.cc.s

src/B1EventAction.o: src/B1EventAction.cc.o
.PHONY : src/B1EventAction.o

# target to build an object file
src/B1EventAction.cc.o:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/B1EventAction.cc.o
.PHONY : src/B1EventAction.cc.o

src/B1EventAction.i: src/B1EventAction.cc.i
.PHONY : src/B1EventAction.i

# target to preprocess a source file
src/B1EventAction.cc.i:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/B1EventAction.cc.i
.PHONY : src/B1EventAction.cc.i

src/B1EventAction.s: src/B1EventAction.cc.s
.PHONY : src/B1EventAction.s

# target to generate assembly for a file
src/B1EventAction.cc.s:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/B1EventAction.cc.s
.PHONY : src/B1EventAction.cc.s

src/B1IO.o: src/B1IO.cc.o
.PHONY : src/B1IO.o

# target to build an object file
src/B1IO.cc.o:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/B1IO.cc.o
.PHONY : src/B1IO.cc.o

src/B1IO.i: src/B1IO.cc.i
.PHONY : src/B1IO.i

# target to preprocess a source file
src/B1IO.cc.i:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/B1IO.cc.i
.PHONY : src/B1IO.cc.i

src/B1IO.s: src/B1IO.cc.s
.PHONY : src/B1IO.s

# target to generate assembly for a file
src/B1IO.cc.s:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/B1IO.cc.s
.PHONY : src/B1IO.cc.s

src/B1PrimaryGeneratorAction.o: src/B1PrimaryGeneratorAction.cc.o
.PHONY : src/B1PrimaryGeneratorAction.o

# target to build an object file
src/B1PrimaryGeneratorAction.cc.o:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/B1PrimaryGeneratorAction.cc.o
.PHONY : src/B1PrimaryGeneratorAction.cc.o

src/B1PrimaryGeneratorAction.i: src/B1PrimaryGeneratorAction.cc.i
.PHONY : src/B1PrimaryGeneratorAction.i

# target to preprocess a source file
src/B1PrimaryGeneratorAction.cc.i:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/B1PrimaryGeneratorAction.cc.i
.PHONY : src/B1PrimaryGeneratorAction.cc.i

src/B1PrimaryGeneratorAction.s: src/B1PrimaryGeneratorAction.cc.s
.PHONY : src/B1PrimaryGeneratorAction.s

# target to generate assembly for a file
src/B1PrimaryGeneratorAction.cc.s:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/B1PrimaryGeneratorAction.cc.s
.PHONY : src/B1PrimaryGeneratorAction.cc.s

src/B1Run.o: src/B1Run.cc.o
.PHONY : src/B1Run.o

# target to build an object file
src/B1Run.cc.o:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/B1Run.cc.o
.PHONY : src/B1Run.cc.o

src/B1Run.i: src/B1Run.cc.i
.PHONY : src/B1Run.i

# target to preprocess a source file
src/B1Run.cc.i:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/B1Run.cc.i
.PHONY : src/B1Run.cc.i

src/B1Run.s: src/B1Run.cc.s
.PHONY : src/B1Run.s

# target to generate assembly for a file
src/B1Run.cc.s:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/B1Run.cc.s
.PHONY : src/B1Run.cc.s

src/B1RunAction.o: src/B1RunAction.cc.o
.PHONY : src/B1RunAction.o

# target to build an object file
src/B1RunAction.cc.o:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/B1RunAction.cc.o
.PHONY : src/B1RunAction.cc.o

src/B1RunAction.i: src/B1RunAction.cc.i
.PHONY : src/B1RunAction.i

# target to preprocess a source file
src/B1RunAction.cc.i:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/B1RunAction.cc.i
.PHONY : src/B1RunAction.cc.i

src/B1RunAction.s: src/B1RunAction.cc.s
.PHONY : src/B1RunAction.s

# target to generate assembly for a file
src/B1RunAction.cc.s:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/B1RunAction.cc.s
.PHONY : src/B1RunAction.cc.s

src/B1SteppingAction.o: src/B1SteppingAction.cc.o
.PHONY : src/B1SteppingAction.o

# target to build an object file
src/B1SteppingAction.cc.o:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/B1SteppingAction.cc.o
.PHONY : src/B1SteppingAction.cc.o

src/B1SteppingAction.i: src/B1SteppingAction.cc.i
.PHONY : src/B1SteppingAction.i

# target to preprocess a source file
src/B1SteppingAction.cc.i:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/B1SteppingAction.cc.i
.PHONY : src/B1SteppingAction.cc.i

src/B1SteppingAction.s: src/B1SteppingAction.cc.s
.PHONY : src/B1SteppingAction.s

# target to generate assembly for a file
src/B1SteppingAction.cc.s:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/B1SteppingAction.cc.s
.PHONY : src/B1SteppingAction.cc.s

src/SensitiveDetector.o: src/SensitiveDetector.cc.o
.PHONY : src/SensitiveDetector.o

# target to build an object file
src/SensitiveDetector.cc.o:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/SensitiveDetector.cc.o
.PHONY : src/SensitiveDetector.cc.o

src/SensitiveDetector.i: src/SensitiveDetector.cc.i
.PHONY : src/SensitiveDetector.i

# target to preprocess a source file
src/SensitiveDetector.cc.i:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/SensitiveDetector.cc.i
.PHONY : src/SensitiveDetector.cc.i

src/SensitiveDetector.s: src/SensitiveDetector.cc.s
.PHONY : src/SensitiveDetector.s

# target to generate assembly for a file
src/SensitiveDetector.cc.s:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/SensitiveDetector.cc.s
.PHONY : src/SensitiveDetector.cc.s

src/SiHit.o: src/SiHit.cc.o
.PHONY : src/SiHit.o

# target to build an object file
src/SiHit.cc.o:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/SiHit.cc.o
.PHONY : src/SiHit.cc.o

src/SiHit.i: src/SiHit.cc.i
.PHONY : src/SiHit.i

# target to preprocess a source file
src/SiHit.cc.i:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/SiHit.cc.i
.PHONY : src/SiHit.cc.i

src/SiHit.s: src/SiHit.cc.s
.PHONY : src/SiHit.s

# target to generate assembly for a file
src/SiHit.cc.s:
	$(MAKE) -f CMakeFiles/exampleB1.dir/build.make CMakeFiles/exampleB1.dir/src/SiHit.cc.s
.PHONY : src/SiHit.cc.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... B1"
	@echo "... edit_cache"
	@echo "... exampleB1"
	@echo "... install"
	@echo "... install/local"
	@echo "... install/strip"
	@echo "... list_install_components"
	@echo "... rebuild_cache"
	@echo "... exampleB1.o"
	@echo "... exampleB1.i"
	@echo "... exampleB1.s"
	@echo "... src/B1ActionInitialization.o"
	@echo "... src/B1ActionInitialization.i"
	@echo "... src/B1ActionInitialization.s"
	@echo "... src/B1DetectorConstruction.o"
	@echo "... src/B1DetectorConstruction.i"
	@echo "... src/B1DetectorConstruction.s"
	@echo "... src/B1EventAction.o"
	@echo "... src/B1EventAction.i"
	@echo "... src/B1EventAction.s"
	@echo "... src/B1IO.o"
	@echo "... src/B1IO.i"
	@echo "... src/B1IO.s"
	@echo "... src/B1PrimaryGeneratorAction.o"
	@echo "... src/B1PrimaryGeneratorAction.i"
	@echo "... src/B1PrimaryGeneratorAction.s"
	@echo "... src/B1Run.o"
	@echo "... src/B1Run.i"
	@echo "... src/B1Run.s"
	@echo "... src/B1RunAction.o"
	@echo "... src/B1RunAction.i"
	@echo "... src/B1RunAction.s"
	@echo "... src/B1SteppingAction.o"
	@echo "... src/B1SteppingAction.i"
	@echo "... src/B1SteppingAction.s"
	@echo "... src/SensitiveDetector.o"
	@echo "... src/SensitiveDetector.i"
	@echo "... src/SensitiveDetector.s"
	@echo "... src/SiHit.o"
	@echo "... src/SiHit.i"
	@echo "... src/SiHit.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/yibeibankaishui/Desktop/ImageProcess_opencv

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/yibeibankaishui/Desktop/ImageProcess_opencv/build

# Include any dependencies generated for this target.
include CMakeFiles/main_hw2.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/main_hw2.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/main_hw2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main_hw2.dir/flags.make

CMakeFiles/main_hw2.dir/src/main_hw2.cpp.o: CMakeFiles/main_hw2.dir/flags.make
CMakeFiles/main_hw2.dir/src/main_hw2.cpp.o: ../src/main_hw2.cpp
CMakeFiles/main_hw2.dir/src/main_hw2.cpp.o: CMakeFiles/main_hw2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yibeibankaishui/Desktop/ImageProcess_opencv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/main_hw2.dir/src/main_hw2.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/main_hw2.dir/src/main_hw2.cpp.o -MF CMakeFiles/main_hw2.dir/src/main_hw2.cpp.o.d -o CMakeFiles/main_hw2.dir/src/main_hw2.cpp.o -c /Users/yibeibankaishui/Desktop/ImageProcess_opencv/src/main_hw2.cpp

CMakeFiles/main_hw2.dir/src/main_hw2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main_hw2.dir/src/main_hw2.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yibeibankaishui/Desktop/ImageProcess_opencv/src/main_hw2.cpp > CMakeFiles/main_hw2.dir/src/main_hw2.cpp.i

CMakeFiles/main_hw2.dir/src/main_hw2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main_hw2.dir/src/main_hw2.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yibeibankaishui/Desktop/ImageProcess_opencv/src/main_hw2.cpp -o CMakeFiles/main_hw2.dir/src/main_hw2.cpp.s

# Object files for target main_hw2
main_hw2_OBJECTS = \
"CMakeFiles/main_hw2.dir/src/main_hw2.cpp.o"

# External object files for target main_hw2
main_hw2_EXTERNAL_OBJECTS =

../bin/main_hw2: CMakeFiles/main_hw2.dir/src/main_hw2.cpp.o
../bin/main_hw2: CMakeFiles/main_hw2.dir/build.make
../bin/main_hw2: ../lib/libUndistort.a
../bin/main_hw2: /usr/local/lib/libopencv_dnn.3.4.16.dylib
../bin/main_hw2: /usr/local/lib/libopencv_highgui.3.4.16.dylib
../bin/main_hw2: /usr/local/lib/libopencv_ml.3.4.16.dylib
../bin/main_hw2: /usr/local/lib/libopencv_objdetect.3.4.16.dylib
../bin/main_hw2: /usr/local/lib/libopencv_shape.3.4.16.dylib
../bin/main_hw2: /usr/local/lib/libopencv_stitching.3.4.16.dylib
../bin/main_hw2: /usr/local/lib/libopencv_superres.3.4.16.dylib
../bin/main_hw2: /usr/local/lib/libopencv_videostab.3.4.16.dylib
../bin/main_hw2: /usr/local/lib/libopencv_calib3d.3.4.16.dylib
../bin/main_hw2: /usr/local/lib/libopencv_features2d.3.4.16.dylib
../bin/main_hw2: /usr/local/lib/libopencv_flann.3.4.16.dylib
../bin/main_hw2: /usr/local/lib/libopencv_photo.3.4.16.dylib
../bin/main_hw2: /usr/local/lib/libopencv_video.3.4.16.dylib
../bin/main_hw2: /usr/local/lib/libopencv_videoio.3.4.16.dylib
../bin/main_hw2: /usr/local/lib/libopencv_imgcodecs.3.4.16.dylib
../bin/main_hw2: /usr/local/lib/libopencv_imgproc.3.4.16.dylib
../bin/main_hw2: /usr/local/lib/libopencv_core.3.4.16.dylib
../bin/main_hw2: /opt/local/lib/libceres.a
../bin/main_hw2: /opt/local/lib/libglog.0.5.0.dylib
../bin/main_hw2: /opt/local/lib/libgflags.2.2.2.dylib
../bin/main_hw2: /opt/local/lib/libspqr.dylib
../bin/main_hw2: /opt/local/lib/libcholmod.dylib
../bin/main_hw2: /opt/local/lib/libccolamd.dylib
../bin/main_hw2: /opt/local/lib/libcamd.dylib
../bin/main_hw2: /opt/local/lib/libcolamd.dylib
../bin/main_hw2: /opt/local/lib/libamd.dylib
../bin/main_hw2: /opt/local/lib/libsuitesparseconfig.dylib
../bin/main_hw2: /opt/local/lib/libmetis.dylib
../bin/main_hw2: /opt/local/lib/libcxsparse.dylib
../bin/main_hw2: /opt/local/lib/libsuitesparseconfig.dylib
../bin/main_hw2: /opt/local/lib/libmetis.dylib
../bin/main_hw2: /opt/local/lib/libcxsparse.dylib
../bin/main_hw2: CMakeFiles/main_hw2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/yibeibankaishui/Desktop/ImageProcess_opencv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/main_hw2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main_hw2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/main_hw2.dir/build: ../bin/main_hw2
.PHONY : CMakeFiles/main_hw2.dir/build

CMakeFiles/main_hw2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/main_hw2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/main_hw2.dir/clean

CMakeFiles/main_hw2.dir/depend:
	cd /Users/yibeibankaishui/Desktop/ImageProcess_opencv/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/yibeibankaishui/Desktop/ImageProcess_opencv /Users/yibeibankaishui/Desktop/ImageProcess_opencv /Users/yibeibankaishui/Desktop/ImageProcess_opencv/build /Users/yibeibankaishui/Desktop/ImageProcess_opencv/build /Users/yibeibankaishui/Desktop/ImageProcess_opencv/build/CMakeFiles/main_hw2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/main_hw2.dir/depend


# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/nathan/Desktop/490h/chess-sight

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nathan/Desktop/490h/chess-sight

# Include any dependencies generated for this target.
include CMakeFiles/yolo_object_detection.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/yolo_object_detection.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/yolo_object_detection.dir/flags.make

CMakeFiles/yolo_object_detection.dir/yolo_object_detection.cpp.o: CMakeFiles/yolo_object_detection.dir/flags.make
CMakeFiles/yolo_object_detection.dir/yolo_object_detection.cpp.o: yolo_object_detection.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nathan/Desktop/490h/chess-sight/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/yolo_object_detection.dir/yolo_object_detection.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/yolo_object_detection.dir/yolo_object_detection.cpp.o -c /home/nathan/Desktop/490h/chess-sight/yolo_object_detection.cpp

CMakeFiles/yolo_object_detection.dir/yolo_object_detection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/yolo_object_detection.dir/yolo_object_detection.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nathan/Desktop/490h/chess-sight/yolo_object_detection.cpp > CMakeFiles/yolo_object_detection.dir/yolo_object_detection.cpp.i

CMakeFiles/yolo_object_detection.dir/yolo_object_detection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/yolo_object_detection.dir/yolo_object_detection.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nathan/Desktop/490h/chess-sight/yolo_object_detection.cpp -o CMakeFiles/yolo_object_detection.dir/yolo_object_detection.cpp.s

CMakeFiles/yolo_object_detection.dir/yolo_object_detection.cpp.o.requires:

.PHONY : CMakeFiles/yolo_object_detection.dir/yolo_object_detection.cpp.o.requires

CMakeFiles/yolo_object_detection.dir/yolo_object_detection.cpp.o.provides: CMakeFiles/yolo_object_detection.dir/yolo_object_detection.cpp.o.requires
	$(MAKE) -f CMakeFiles/yolo_object_detection.dir/build.make CMakeFiles/yolo_object_detection.dir/yolo_object_detection.cpp.o.provides.build
.PHONY : CMakeFiles/yolo_object_detection.dir/yolo_object_detection.cpp.o.provides

CMakeFiles/yolo_object_detection.dir/yolo_object_detection.cpp.o.provides.build: CMakeFiles/yolo_object_detection.dir/yolo_object_detection.cpp.o


# Object files for target yolo_object_detection
yolo_object_detection_OBJECTS = \
"CMakeFiles/yolo_object_detection.dir/yolo_object_detection.cpp.o"

# External object files for target yolo_object_detection
yolo_object_detection_EXTERNAL_OBJECTS =

yolo_object_detection: CMakeFiles/yolo_object_detection.dir/yolo_object_detection.cpp.o
yolo_object_detection: CMakeFiles/yolo_object_detection.dir/build.make
yolo_object_detection: /usr/local/lib/libopencv_objdetect.so.3.4.1
yolo_object_detection: /usr/local/lib/libopencv_ml.so.3.4.1
yolo_object_detection: /usr/local/lib/libopencv_videostab.so.3.4.1
yolo_object_detection: /usr/local/lib/libopencv_dnn.so.3.4.1
yolo_object_detection: /usr/local/lib/libopencv_shape.so.3.4.1
yolo_object_detection: /usr/local/lib/libopencv_stitching.so.3.4.1
yolo_object_detection: /usr/local/lib/libopencv_superres.so.3.4.1
yolo_object_detection: /usr/local/lib/libopencv_viz.so.3.4.1
yolo_object_detection: /usr/local/lib/libopencv_photo.so.3.4.1
yolo_object_detection: /usr/local/lib/libopencv_calib3d.so.3.4.1
yolo_object_detection: /usr/local/lib/libopencv_features2d.so.3.4.1
yolo_object_detection: /usr/local/lib/libopencv_highgui.so.3.4.1
yolo_object_detection: /usr/local/lib/libopencv_flann.so.3.4.1
yolo_object_detection: /usr/local/lib/libopencv_videoio.so.3.4.1
yolo_object_detection: /usr/local/lib/libopencv_imgcodecs.so.3.4.1
yolo_object_detection: /usr/local/lib/libopencv_video.so.3.4.1
yolo_object_detection: /usr/local/lib/libopencv_imgproc.so.3.4.1
yolo_object_detection: /usr/local/lib/libopencv_core.so.3.4.1
yolo_object_detection: CMakeFiles/yolo_object_detection.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nathan/Desktop/490h/chess-sight/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable yolo_object_detection"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/yolo_object_detection.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/yolo_object_detection.dir/build: yolo_object_detection

.PHONY : CMakeFiles/yolo_object_detection.dir/build

CMakeFiles/yolo_object_detection.dir/requires: CMakeFiles/yolo_object_detection.dir/yolo_object_detection.cpp.o.requires

.PHONY : CMakeFiles/yolo_object_detection.dir/requires

CMakeFiles/yolo_object_detection.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/yolo_object_detection.dir/cmake_clean.cmake
.PHONY : CMakeFiles/yolo_object_detection.dir/clean

CMakeFiles/yolo_object_detection.dir/depend:
	cd /home/nathan/Desktop/490h/chess-sight && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nathan/Desktop/490h/chess-sight /home/nathan/Desktop/490h/chess-sight /home/nathan/Desktop/490h/chess-sight /home/nathan/Desktop/490h/chess-sight /home/nathan/Desktop/490h/chess-sight/CMakeFiles/yolo_object_detection.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/yolo_object_detection.dir/depend

# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/fly/projects/chatServer/chatServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/fly/projects/chatServer/chatServer/build

# Include any dependencies generated for this target.
include src/server/CMakeFiles/chatServer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/server/CMakeFiles/chatServer.dir/compiler_depend.make

# Include the progress variables for this target.
include src/server/CMakeFiles/chatServer.dir/progress.make

# Include the compile flags for this target's objects.
include src/server/CMakeFiles/chatServer.dir/flags.make

src/server/CMakeFiles/chatServer.dir/chatserver.cpp.o: src/server/CMakeFiles/chatServer.dir/flags.make
src/server/CMakeFiles/chatServer.dir/chatserver.cpp.o: /home/fly/projects/chatServer/chatServer/src/server/chatserver.cpp
src/server/CMakeFiles/chatServer.dir/chatserver.cpp.o: src/server/CMakeFiles/chatServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/fly/projects/chatServer/chatServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/server/CMakeFiles/chatServer.dir/chatserver.cpp.o"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/server/CMakeFiles/chatServer.dir/chatserver.cpp.o -MF CMakeFiles/chatServer.dir/chatserver.cpp.o.d -o CMakeFiles/chatServer.dir/chatserver.cpp.o -c /home/fly/projects/chatServer/chatServer/src/server/chatserver.cpp

src/server/CMakeFiles/chatServer.dir/chatserver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/chatServer.dir/chatserver.cpp.i"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fly/projects/chatServer/chatServer/src/server/chatserver.cpp > CMakeFiles/chatServer.dir/chatserver.cpp.i

src/server/CMakeFiles/chatServer.dir/chatserver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/chatServer.dir/chatserver.cpp.s"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fly/projects/chatServer/chatServer/src/server/chatserver.cpp -o CMakeFiles/chatServer.dir/chatserver.cpp.s

src/server/CMakeFiles/chatServer.dir/chatservice.cpp.o: src/server/CMakeFiles/chatServer.dir/flags.make
src/server/CMakeFiles/chatServer.dir/chatservice.cpp.o: /home/fly/projects/chatServer/chatServer/src/server/chatservice.cpp
src/server/CMakeFiles/chatServer.dir/chatservice.cpp.o: src/server/CMakeFiles/chatServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/fly/projects/chatServer/chatServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/server/CMakeFiles/chatServer.dir/chatservice.cpp.o"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/server/CMakeFiles/chatServer.dir/chatservice.cpp.o -MF CMakeFiles/chatServer.dir/chatservice.cpp.o.d -o CMakeFiles/chatServer.dir/chatservice.cpp.o -c /home/fly/projects/chatServer/chatServer/src/server/chatservice.cpp

src/server/CMakeFiles/chatServer.dir/chatservice.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/chatServer.dir/chatservice.cpp.i"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fly/projects/chatServer/chatServer/src/server/chatservice.cpp > CMakeFiles/chatServer.dir/chatservice.cpp.i

src/server/CMakeFiles/chatServer.dir/chatservice.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/chatServer.dir/chatservice.cpp.s"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fly/projects/chatServer/chatServer/src/server/chatservice.cpp -o CMakeFiles/chatServer.dir/chatservice.cpp.s

src/server/CMakeFiles/chatServer.dir/main.cpp.o: src/server/CMakeFiles/chatServer.dir/flags.make
src/server/CMakeFiles/chatServer.dir/main.cpp.o: /home/fly/projects/chatServer/chatServer/src/server/main.cpp
src/server/CMakeFiles/chatServer.dir/main.cpp.o: src/server/CMakeFiles/chatServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/fly/projects/chatServer/chatServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/server/CMakeFiles/chatServer.dir/main.cpp.o"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/server/CMakeFiles/chatServer.dir/main.cpp.o -MF CMakeFiles/chatServer.dir/main.cpp.o.d -o CMakeFiles/chatServer.dir/main.cpp.o -c /home/fly/projects/chatServer/chatServer/src/server/main.cpp

src/server/CMakeFiles/chatServer.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/chatServer.dir/main.cpp.i"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fly/projects/chatServer/chatServer/src/server/main.cpp > CMakeFiles/chatServer.dir/main.cpp.i

src/server/CMakeFiles/chatServer.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/chatServer.dir/main.cpp.s"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fly/projects/chatServer/chatServer/src/server/main.cpp -o CMakeFiles/chatServer.dir/main.cpp.s

src/server/CMakeFiles/chatServer.dir/db/connectionPool.cpp.o: src/server/CMakeFiles/chatServer.dir/flags.make
src/server/CMakeFiles/chatServer.dir/db/connectionPool.cpp.o: /home/fly/projects/chatServer/chatServer/src/server/db/connectionPool.cpp
src/server/CMakeFiles/chatServer.dir/db/connectionPool.cpp.o: src/server/CMakeFiles/chatServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/fly/projects/chatServer/chatServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/server/CMakeFiles/chatServer.dir/db/connectionPool.cpp.o"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/server/CMakeFiles/chatServer.dir/db/connectionPool.cpp.o -MF CMakeFiles/chatServer.dir/db/connectionPool.cpp.o.d -o CMakeFiles/chatServer.dir/db/connectionPool.cpp.o -c /home/fly/projects/chatServer/chatServer/src/server/db/connectionPool.cpp

src/server/CMakeFiles/chatServer.dir/db/connectionPool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/chatServer.dir/db/connectionPool.cpp.i"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fly/projects/chatServer/chatServer/src/server/db/connectionPool.cpp > CMakeFiles/chatServer.dir/db/connectionPool.cpp.i

src/server/CMakeFiles/chatServer.dir/db/connectionPool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/chatServer.dir/db/connectionPool.cpp.s"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fly/projects/chatServer/chatServer/src/server/db/connectionPool.cpp -o CMakeFiles/chatServer.dir/db/connectionPool.cpp.s

src/server/CMakeFiles/chatServer.dir/db/db.cpp.o: src/server/CMakeFiles/chatServer.dir/flags.make
src/server/CMakeFiles/chatServer.dir/db/db.cpp.o: /home/fly/projects/chatServer/chatServer/src/server/db/db.cpp
src/server/CMakeFiles/chatServer.dir/db/db.cpp.o: src/server/CMakeFiles/chatServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/fly/projects/chatServer/chatServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/server/CMakeFiles/chatServer.dir/db/db.cpp.o"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/server/CMakeFiles/chatServer.dir/db/db.cpp.o -MF CMakeFiles/chatServer.dir/db/db.cpp.o.d -o CMakeFiles/chatServer.dir/db/db.cpp.o -c /home/fly/projects/chatServer/chatServer/src/server/db/db.cpp

src/server/CMakeFiles/chatServer.dir/db/db.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/chatServer.dir/db/db.cpp.i"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fly/projects/chatServer/chatServer/src/server/db/db.cpp > CMakeFiles/chatServer.dir/db/db.cpp.i

src/server/CMakeFiles/chatServer.dir/db/db.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/chatServer.dir/db/db.cpp.s"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fly/projects/chatServer/chatServer/src/server/db/db.cpp -o CMakeFiles/chatServer.dir/db/db.cpp.s

src/server/CMakeFiles/chatServer.dir/model/UserModel.cpp.o: src/server/CMakeFiles/chatServer.dir/flags.make
src/server/CMakeFiles/chatServer.dir/model/UserModel.cpp.o: /home/fly/projects/chatServer/chatServer/src/server/model/UserModel.cpp
src/server/CMakeFiles/chatServer.dir/model/UserModel.cpp.o: src/server/CMakeFiles/chatServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/fly/projects/chatServer/chatServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/server/CMakeFiles/chatServer.dir/model/UserModel.cpp.o"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/server/CMakeFiles/chatServer.dir/model/UserModel.cpp.o -MF CMakeFiles/chatServer.dir/model/UserModel.cpp.o.d -o CMakeFiles/chatServer.dir/model/UserModel.cpp.o -c /home/fly/projects/chatServer/chatServer/src/server/model/UserModel.cpp

src/server/CMakeFiles/chatServer.dir/model/UserModel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/chatServer.dir/model/UserModel.cpp.i"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fly/projects/chatServer/chatServer/src/server/model/UserModel.cpp > CMakeFiles/chatServer.dir/model/UserModel.cpp.i

src/server/CMakeFiles/chatServer.dir/model/UserModel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/chatServer.dir/model/UserModel.cpp.s"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fly/projects/chatServer/chatServer/src/server/model/UserModel.cpp -o CMakeFiles/chatServer.dir/model/UserModel.cpp.s

src/server/CMakeFiles/chatServer.dir/model/friendmodel.cpp.o: src/server/CMakeFiles/chatServer.dir/flags.make
src/server/CMakeFiles/chatServer.dir/model/friendmodel.cpp.o: /home/fly/projects/chatServer/chatServer/src/server/model/friendmodel.cpp
src/server/CMakeFiles/chatServer.dir/model/friendmodel.cpp.o: src/server/CMakeFiles/chatServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/fly/projects/chatServer/chatServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object src/server/CMakeFiles/chatServer.dir/model/friendmodel.cpp.o"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/server/CMakeFiles/chatServer.dir/model/friendmodel.cpp.o -MF CMakeFiles/chatServer.dir/model/friendmodel.cpp.o.d -o CMakeFiles/chatServer.dir/model/friendmodel.cpp.o -c /home/fly/projects/chatServer/chatServer/src/server/model/friendmodel.cpp

src/server/CMakeFiles/chatServer.dir/model/friendmodel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/chatServer.dir/model/friendmodel.cpp.i"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fly/projects/chatServer/chatServer/src/server/model/friendmodel.cpp > CMakeFiles/chatServer.dir/model/friendmodel.cpp.i

src/server/CMakeFiles/chatServer.dir/model/friendmodel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/chatServer.dir/model/friendmodel.cpp.s"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fly/projects/chatServer/chatServer/src/server/model/friendmodel.cpp -o CMakeFiles/chatServer.dir/model/friendmodel.cpp.s

src/server/CMakeFiles/chatServer.dir/model/groupmodel.cpp.o: src/server/CMakeFiles/chatServer.dir/flags.make
src/server/CMakeFiles/chatServer.dir/model/groupmodel.cpp.o: /home/fly/projects/chatServer/chatServer/src/server/model/groupmodel.cpp
src/server/CMakeFiles/chatServer.dir/model/groupmodel.cpp.o: src/server/CMakeFiles/chatServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/fly/projects/chatServer/chatServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object src/server/CMakeFiles/chatServer.dir/model/groupmodel.cpp.o"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/server/CMakeFiles/chatServer.dir/model/groupmodel.cpp.o -MF CMakeFiles/chatServer.dir/model/groupmodel.cpp.o.d -o CMakeFiles/chatServer.dir/model/groupmodel.cpp.o -c /home/fly/projects/chatServer/chatServer/src/server/model/groupmodel.cpp

src/server/CMakeFiles/chatServer.dir/model/groupmodel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/chatServer.dir/model/groupmodel.cpp.i"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fly/projects/chatServer/chatServer/src/server/model/groupmodel.cpp > CMakeFiles/chatServer.dir/model/groupmodel.cpp.i

src/server/CMakeFiles/chatServer.dir/model/groupmodel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/chatServer.dir/model/groupmodel.cpp.s"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fly/projects/chatServer/chatServer/src/server/model/groupmodel.cpp -o CMakeFiles/chatServer.dir/model/groupmodel.cpp.s

src/server/CMakeFiles/chatServer.dir/model/offlinemessagemodel.cpp.o: src/server/CMakeFiles/chatServer.dir/flags.make
src/server/CMakeFiles/chatServer.dir/model/offlinemessagemodel.cpp.o: /home/fly/projects/chatServer/chatServer/src/server/model/offlinemessagemodel.cpp
src/server/CMakeFiles/chatServer.dir/model/offlinemessagemodel.cpp.o: src/server/CMakeFiles/chatServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/fly/projects/chatServer/chatServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object src/server/CMakeFiles/chatServer.dir/model/offlinemessagemodel.cpp.o"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/server/CMakeFiles/chatServer.dir/model/offlinemessagemodel.cpp.o -MF CMakeFiles/chatServer.dir/model/offlinemessagemodel.cpp.o.d -o CMakeFiles/chatServer.dir/model/offlinemessagemodel.cpp.o -c /home/fly/projects/chatServer/chatServer/src/server/model/offlinemessagemodel.cpp

src/server/CMakeFiles/chatServer.dir/model/offlinemessagemodel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/chatServer.dir/model/offlinemessagemodel.cpp.i"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fly/projects/chatServer/chatServer/src/server/model/offlinemessagemodel.cpp > CMakeFiles/chatServer.dir/model/offlinemessagemodel.cpp.i

src/server/CMakeFiles/chatServer.dir/model/offlinemessagemodel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/chatServer.dir/model/offlinemessagemodel.cpp.s"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fly/projects/chatServer/chatServer/src/server/model/offlinemessagemodel.cpp -o CMakeFiles/chatServer.dir/model/offlinemessagemodel.cpp.s

src/server/CMakeFiles/chatServer.dir/redis/redis.cpp.o: src/server/CMakeFiles/chatServer.dir/flags.make
src/server/CMakeFiles/chatServer.dir/redis/redis.cpp.o: /home/fly/projects/chatServer/chatServer/src/server/redis/redis.cpp
src/server/CMakeFiles/chatServer.dir/redis/redis.cpp.o: src/server/CMakeFiles/chatServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/fly/projects/chatServer/chatServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object src/server/CMakeFiles/chatServer.dir/redis/redis.cpp.o"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/server/CMakeFiles/chatServer.dir/redis/redis.cpp.o -MF CMakeFiles/chatServer.dir/redis/redis.cpp.o.d -o CMakeFiles/chatServer.dir/redis/redis.cpp.o -c /home/fly/projects/chatServer/chatServer/src/server/redis/redis.cpp

src/server/CMakeFiles/chatServer.dir/redis/redis.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/chatServer.dir/redis/redis.cpp.i"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fly/projects/chatServer/chatServer/src/server/redis/redis.cpp > CMakeFiles/chatServer.dir/redis/redis.cpp.i

src/server/CMakeFiles/chatServer.dir/redis/redis.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/chatServer.dir/redis/redis.cpp.s"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fly/projects/chatServer/chatServer/src/server/redis/redis.cpp -o CMakeFiles/chatServer.dir/redis/redis.cpp.s

# Object files for target chatServer
chatServer_OBJECTS = \
"CMakeFiles/chatServer.dir/chatserver.cpp.o" \
"CMakeFiles/chatServer.dir/chatservice.cpp.o" \
"CMakeFiles/chatServer.dir/main.cpp.o" \
"CMakeFiles/chatServer.dir/db/connectionPool.cpp.o" \
"CMakeFiles/chatServer.dir/db/db.cpp.o" \
"CMakeFiles/chatServer.dir/model/UserModel.cpp.o" \
"CMakeFiles/chatServer.dir/model/friendmodel.cpp.o" \
"CMakeFiles/chatServer.dir/model/groupmodel.cpp.o" \
"CMakeFiles/chatServer.dir/model/offlinemessagemodel.cpp.o" \
"CMakeFiles/chatServer.dir/redis/redis.cpp.o"

# External object files for target chatServer
chatServer_EXTERNAL_OBJECTS =

/home/fly/projects/chatServer/chatServer/bin/chatServer: src/server/CMakeFiles/chatServer.dir/chatserver.cpp.o
/home/fly/projects/chatServer/chatServer/bin/chatServer: src/server/CMakeFiles/chatServer.dir/chatservice.cpp.o
/home/fly/projects/chatServer/chatServer/bin/chatServer: src/server/CMakeFiles/chatServer.dir/main.cpp.o
/home/fly/projects/chatServer/chatServer/bin/chatServer: src/server/CMakeFiles/chatServer.dir/db/connectionPool.cpp.o
/home/fly/projects/chatServer/chatServer/bin/chatServer: src/server/CMakeFiles/chatServer.dir/db/db.cpp.o
/home/fly/projects/chatServer/chatServer/bin/chatServer: src/server/CMakeFiles/chatServer.dir/model/UserModel.cpp.o
/home/fly/projects/chatServer/chatServer/bin/chatServer: src/server/CMakeFiles/chatServer.dir/model/friendmodel.cpp.o
/home/fly/projects/chatServer/chatServer/bin/chatServer: src/server/CMakeFiles/chatServer.dir/model/groupmodel.cpp.o
/home/fly/projects/chatServer/chatServer/bin/chatServer: src/server/CMakeFiles/chatServer.dir/model/offlinemessagemodel.cpp.o
/home/fly/projects/chatServer/chatServer/bin/chatServer: src/server/CMakeFiles/chatServer.dir/redis/redis.cpp.o
/home/fly/projects/chatServer/chatServer/bin/chatServer: src/server/CMakeFiles/chatServer.dir/build.make
/home/fly/projects/chatServer/chatServer/bin/chatServer: src/server/CMakeFiles/chatServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/fly/projects/chatServer/chatServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX executable /home/fly/projects/chatServer/chatServer/bin/chatServer"
	cd /home/fly/projects/chatServer/chatServer/build/src/server && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/chatServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/server/CMakeFiles/chatServer.dir/build: /home/fly/projects/chatServer/chatServer/bin/chatServer
.PHONY : src/server/CMakeFiles/chatServer.dir/build

src/server/CMakeFiles/chatServer.dir/clean:
	cd /home/fly/projects/chatServer/chatServer/build/src/server && $(CMAKE_COMMAND) -P CMakeFiles/chatServer.dir/cmake_clean.cmake
.PHONY : src/server/CMakeFiles/chatServer.dir/clean

src/server/CMakeFiles/chatServer.dir/depend:
	cd /home/fly/projects/chatServer/chatServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/fly/projects/chatServer/chatServer /home/fly/projects/chatServer/chatServer/src/server /home/fly/projects/chatServer/chatServer/build /home/fly/projects/chatServer/chatServer/build/src/server /home/fly/projects/chatServer/chatServer/build/src/server/CMakeFiles/chatServer.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/server/CMakeFiles/chatServer.dir/depend


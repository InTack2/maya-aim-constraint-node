#-
# ==========================================================================
# Copyright (c) 2018 Autodesk, Inc.
# All rights reserved.
# 
# These coded instructions, statements, and computer programs contain
# unpublished proprietary information written by Autodesk, Inc., and are
# protected by Federal copyright law. They may not be disclosed to third
# parties or copied or duplicated in any form, in whole or in part, without
# the prior written consent of Autodesk, Inc.
# ==========================================================================
#+


cmake_minimum_required(VERSION 2.8)

# include the project setting file
include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)

# specify project name
set(PROJECT_NAME aimConstraintNode)



# set SOURCE_FILES
file(GLOB SOURCE_FILES
   ${CMAKE_SOURCE_DIR}/src/*.h
   ${CMAKE_SOURCE_DIR}/src/*.hpp
   ${CMAKE_SOURCE_DIR}/src/*.cpp
   )

# set linking libraries
set(LIBRARIES
     OpenMaya
     Foundation

)

set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Build plugin
build_plugin()
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_COMMAND "maya.exe")


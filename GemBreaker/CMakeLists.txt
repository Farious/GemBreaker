# Copyright: Do as you see fit
# Authors: Fabio Reis
# CopyPolicy: Licensed under the Simplified BSD License

CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT (GemBreaker)

# Loading the FindSDL2*.cmake files for finding the SDL2 folders
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

####################
##  Dependencies  ##
####################

# Let's try to find SDL2.0 installation
FIND_PACKAGE (SDL2 REQUIRED)

# Let's try to find SDL2_image installation
FIND_PACKAGE (SDL2_image REQUIRED)

# Let's try to find SDL2_ttf installation
FIND_PACKAGE (SDL2_ttf REQUIRED)

# If SDL2 was not found and the REQUIRED is left out
IF(NOT SDL2_FOUND)
  # Set your SDL2_FOLDER
  SET (SDL2_FOLDER ../../../SDL2-2.0.3)

  # SDL2.0 include folder
  FILE (GLOB SDL2_INCLUDE_DIR ${SDL2_FOLDER}/include)

  # SDL 2.0 Library
  FILE (GLOB SDL2_LIBRARY ${SDL2_FOLDER}/build/libSDL2*)

  # Copy dll files, change this accordingly to the SO
  # FILE (COPY ${SDL2_LIBRARY} DESTINATION .)
ENDIF(NOT SDL2_FOUND)

# If SDL2_Image was not found and the REQUIRED is left out
IF(NOT SDL2_IMAGE)
  # set your sdl2 image folder
  SET (SDL2_IMAGE_FOLDER ../../../SDL2_image-2.0.0)

  # SDL2.0_image include folder
  FILE (GLOB SDL2_IMAGE_INCLUDE_DIRS ${SDL2_IMAGE_FOLDER}/*.h)

  # SDL2.0_image include
  FILE (GLOB SDL2_IMAGE_LIBRARY ${SDL2_IMAGE_FOLDER}/build/libSDL2*)

  # Copy dll files
  #FILE (COPY ${SDL_IMAGE_DLLS} DESTINATION .)
ENDIF(NOT SDL2_IMAGE)

# If SDL2 was not found and the REQUIRED is left out
IF(NOT SDL2_TTF_FOUND)
  # set your sdl2 image folder
  SET (SDL2_TTF_FOLDER ../../../SDL2_ttf-2.0.12)

  # SDL2.0 TTF include folder
  FILE (GLOB SDL2_TTF_INCLUDE_DIRS ${SDL2_TTF_FOLDER}/*.h)

  # SDL 2.0 include
  FILE (GLOB SDL2_TTF_LIBRARY ${SDL2_TTF_FOLDER}/build/SDL2*.so)

  # Copy dll files
  #FILE (COPY ${SDL_TTF_DLLS} DESTINATION .)
ENDIF(NOT SDL2_TTF_FOUND)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIR}
                    ${SDL2_IMAGE_INCLUDE_DIRS}
                    ${SDL2_TTF_INCLUDE_DIRS})

#INCLUDE(FindPkgConfig)

#PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
#PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

# Set C++ Flags
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

SET (CMAKE_BUILD_TYPE RELEASE)
SET (CMAKE_CXX_FLAGS_DEBUG   "")
SET (CMAKE_CXX_FLAGS_RELEASE "")

# GemBreaker's include folder
INCLUDE_DIRECTORIES (include)

# GemBreaker's source code
FILE (GLOB FOLDER_SOURCE ./source/*.cpp)
FILE (GLOB FOLDER_HEADER ./include/*.hpp)

# GemBreaker's resources
FILE (COPY ./resources DESTINATION . )

SOURCE_GROUP("Source Files" FILES ${FOLDER_SOURCE})
SOURCE_GROUP("Header Files" FILES ${FOLDER_HEADER})

MESSAGE(STATUS "CMAKE SDL2 : " ${SDL2_LIBRARY})
MESSAGE(STATUS "CMAKE SDL2_image : " ${SDL2_IMAGE_LIBRARIES})
MESSAGE(STATUS "CMAKE SDL2_ttf : " ${SDL2_TTF_LIBRARY})

ADD_EXECUTABLE(GemBreaker ${FOLDER_HEADER} ${FOLDER_SOURCE})
TARGET_LINK_LIBRARIES(GemBreaker ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARY})


SET(crfpp_build "${CMAKE_CURRENT_BINARY_DIR}/crfpp")

INCLUDE(ProcessorCount)
ProcessorCount(PROCESSOR_COUNT_VAL)


EXTERNALPROJECT_ADD(
	crfpp_proj
	GIT_REPOSITORY git@github.com:yujing5b5d/crfpp.git
	PREFIX ${CMAKE_CURRENT_BINARY_DIR}
	UPDATE_COMMAND ""
	CONFIGURE_COMMAND ./configure
	SOURCE_DIR ${crfpp_build}
	# BINARY_DIR ${crfpp_build}
	BUILD_COMMAND make -j${PROCESSOR_COUNT_VAL}
	BUILD_IN_SOURCE 1
	INSTALL_COMMAND cp .libs/libcrfpp.a ${PROJECT_BINARY_DIR}/lib && cp crfpp.h ${PROJECT_BINARY_DIR}/include 
)

# http://www.cmake.org/cmake/help/v3.0/command/find_library.html
# FIND_LIBRARY ( LEVELDB_LIBRARY_INFO NAMES "libleveldb" PATH_SUFFIXES "lib" "a" "so" "dylib")
# FIND_LIBRARY(CRFPP_LIBRARY_INFO  NAMES  crfpp PATHS "${PROJECT_BINARY_DIR}/lib" )


## extend libraries
### http://www.cmake.org/cmake/help/v3.0/module/ExternalProject.html
### http://mirkokiefer.com/blog/2013/03/cmake-by-example/
### http://www.cmake.org/cmake/help/v2.8.10/cmake.html#module:ExternalProject

# EXEC_PROGRAM("echo \"ExternalProject_Add START \" " .)
# INCLUDE(ExternalProject)

# EXEC_PROGRAM("echo \"ExternalProject_Add END \" " .)

# ENDIF(CRFPP_LIBRARY_INFO)


# cp *.a ${PROJECT_BINARY_DIR}/lib
# ExternalProject_Get_Property(leveldb BINARY_DIR)
# MESSAGE(STATUS "leveldb  BINARY_DIR " ${BINARY_DIR})
# LINK_DIRECTORIES(${BINARY_DIR})


# INCLUDE_DIRECTORIES(${CRFPP_HEADER_DIR})
# MESSAGE(STATUS "###INCLUDE_DIRECTORIES " ${INCLUDE_DIRECTORIES})








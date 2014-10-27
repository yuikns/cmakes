SET(mongo_cxx_build "${CMAKE_CURRENT_BINARY_DIR}/mongo_cxx")

INCLUDE(ProcessorCount)
ProcessorCount(PROCESSOR_COUNT_VAL)

IF (APPLE)

EXTERNALPROJECT_ADD(
	mongo_cxx_proj
	GIT_REPOSITORY git@github.com:mongodb/mongo-cxx-driver.git
	GIT_TAG 26compat
	PREFIX ${CMAKE_CURRENT_BINARY_DIR}
	UPDATE_COMMAND ""
	CONFIGURE_COMMAND ""
	SOURCE_DIR ${mongo_cxx_build}
	# BINARY_DIR ${crfpp_build}
	BUILD_COMMAND scons -j${PROCESSOR_COUNT_VAL} --use-system-boost --full --ssl --prefix=${PROJECT_BINARY_DIR} --libpath=${PROJECT_BINARY_DIR}/lib --cpppath=${PROJECT_BINARY_DIR}/include --ssl install-mongoclient --64 --c++11 --libc++ --osx-version-min=10.7
	BUILD_IN_SOURCE 1
	INSTALL_COMMAND ""
	# INSTALL_COMMAND cp .libs/libcrfpp.a ${PROJECT_BINARY_DIR}/lib && cp crfpp.h ${PROJECT_BINARY_DIR}/include 
)

ELSEIF (UNIX)

EXTERNALPROJECT_ADD(
	mongo_cxx_proj
	GIT_REPOSITORY git@github.com:mongodb/mongo-cxx-driver.git
	GIT_TAG 26compat
	PREFIX ${CMAKE_CURRENT_BINARY_DIR}
	UPDATE_COMMAND ""
	CONFIGURE_COMMAND ""
	SOURCE_DIR ${mongo_cxx_build}
	# BINARY_DIR ${crfpp_build}
	BUILD_COMMAND scons -j${PROCESSOR_COUNT_VAL} --use-system-boost --full --ssl --prefix=${PROJECT_BINARY_DIR} --libpath=${PROJECT_BINARY_DIR}/lib --cpppath=${PROJECT_BINARY_DIR}/include --ssl install-mongoclient --64 --c++11
	BUILD_IN_SOURCE 1
	INSTALL_COMMAND ""
	# INSTALL_COMMAND cp .libs/libcrfpp.a ${PROJECT_BINARY_DIR}/lib && cp crfpp.h ${PROJECT_BINARY_DIR}/include 
)

ENDIF ()


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








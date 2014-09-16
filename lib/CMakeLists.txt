##  sublist
ADD_SUBDIRECTORY(iniparser)


# http://www.cmake.org/cmake/help/v3.0/command/find_library.html
# FIND_LIBRARY ( LEVELDB_LIBRARY_INFO NAMES "libleveldb" PATH_SUFFIXES "lib" "a" "so" "dylib")


## extend libraries
### http://www.cmake.org/cmake/help/v3.0/module/ExternalProject.html
### http://mirkokiefer.com/blog/2013/03/cmake-by-example/
### http://www.cmake.org/cmake/help/v2.8.10/cmake.html#module:ExternalProject

# EXEC_PROGRAM("echo \"ExternalProject_Add START \" " .)
# INCLUDE(ExternalProject)

INCLUDE(EXTERNALPROJECT)

FIND_LIBRARY( LEVELDB_LIBRARY_INFO  NAMES  leveldb PATHS "${PROJECT_BINARY_DIR}/lib" )

# UPDATE_COMMAND ""
# IF (LEVELDB_LIBRARY_INFO)
# 	MESSAGE(STATUS "leveldb is found : " "${LEVELDB_LIBRARY_INFO}")
# 	SET(LEVELDB_FOUND_FLAG TRUE)
# ELSE(LEVELDB_LIBRARY_INFO)
# 	MESSAGE(STATUS "leveldb is **NOT** found .. ")
SET(LEVELDB_FOUND_FLAG FALSE)
EXTERNALPROJECT_ADD(
 		LEVELDB_EX_PROJ
 		GIT_REPOSITORY git@github.com:yujing5b5d/leveldb.git
 		PREFIX ${CMAKE_CURRENT_BINARY_DIR}
 		UPDATE_COMMAND ""
 		CONFIGURE_COMMAND ""
 		BUILD_COMMAND make
 		BUILD_IN_SOURCE 1
 		INSTALL_COMMAND cp libleveldb.a ${PROJECT_BINARY_DIR}/lib && cp -r include/leveldb ${PROJECT_BINARY_DIR}/include
 	)
# ENDIF(LEVELDB_LIBRARY_INFO)

# FIND_LIBRARY(CRFPP_LIBRARY_INFO  NAMES  crfpp PATHS "${PROJECT_BINARY_DIR}/lib" )

# IF (CRFPP_LIBRARY_INFO)
# 	MESSAGE(STATUS "crfpp is found : " "${CRFPP_LIBRARY_INFO}")
# 	SET(CRFPP_FOUND_FLAG TRUE)
# ELSE(CRFPP_LIBRARY_INFO)
# 	MESSAGE(STATUS "crfpp is **NOT** found ..")
SET(CRFPP_FOUND_FLAG FALSE)
EXTERNALPROJECT_ADD(
 		CRFPP_EX_PROJ
 		GIT_REPOSITORY git@github.com:yujing5b5d/crfpp.git
 		PREFIX ${CMAKE_CURRENT_BINARY_DIR}
 		UPDATE_COMMAND ""
 		CONFIGURE_COMMAND ./configure
 		BUILD_COMMAND make -j8
 		BUILD_IN_SOURCE 1
 		INSTALL_COMMAND cp .libs/libcrfpp.a ${PROJECT_BINARY_DIR}/lib && cp crfpp.h ${PROJECT_BINARY_DIR}/include 
 	)
# ENDIF(CRFPP_LIBRARY_INFO)


# cp *.a ${PROJECT_BINARY_DIR}/lib
# ExternalProject_Get_Property(leveldb BINARY_DIR)
# MESSAGE(STATUS "leveldb  BINARY_DIR " ${BINARY_DIR})
# LINK_DIRECTORIES(${BINARY_DIR})


# INCLUDE_DIRECTORIES(${CRFPP_HEADER_DIR})
# MESSAGE(STATUS "###INCLUDE_DIRECTORIES " ${INCLUDE_DIRECTORIES})


# EXEC_PROGRAM("echo \"ExternalProject_Add END \" " .)

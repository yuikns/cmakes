# SET(leveldb_build "${CMAKE_CURRENT_BINARY_DIR}/leveldb")

# FIND_LIBRARY( LEVELDB_LIBRARY_INFO  NAMES  leveldb PATHS "${PROJECT_BINARY_DIR}/lib" )

EXTERNALPROJECT_ADD(
 		leveldb_proj
 		GIT_REPOSITORY git@github.com:yujing5b5d/leveldb.git
 		PREFIX ${CMAKE_CURRENT_BINARY_DIR}
 		UPDATE_COMMAND ""
 		CONFIGURE_COMMAND ""
 		# BINARY_DIR ${leveldb_build}
 		BUILD_COMMAND make
 		BUILD_IN_SOURCE 1
 		INSTALL_COMMAND cp libleveldb.a ${PROJECT_BINARY_DIR}/lib && cp -r include/leveldb ${PROJECT_BINARY_DIR}/include
 	)
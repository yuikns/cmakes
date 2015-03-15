SET(leveldb_build "${CMAKE_CURRENT_BINARY_DIR}/leveldb")

# FIND_LIBRARY( LEVELDB_LIBRARY_INFO  NAMES  leveldb PATHS "${PROJECT_BINARY_DIR}/lib" )
INCLUDE(ProcessorCount)
ProcessorCount(PROCESSOR_COUNT_VAL)


EXTERNALPROJECT_ADD(
	leveldb_proj
	GIT_REPOSITORY git@github.com:yuikns/leveldb.git
	PREFIX ${CMAKE_CURRENT_BINARY_DIR}
	UPDATE_COMMAND ""
	CONFIGURE_COMMAND ""
	SOURCE_DIR ${leveldb_build}
	# BINARY_DIR ${leveldb_build}
	BUILD_COMMAND make -j${PROCESSOR_COUNT_VAL}
	BUILD_IN_SOURCE 1
	INSTALL_COMMAND cp libleveldb.a ${PROJECT_BINARY_DIR}/lib && cp -r include/leveldb ${PROJECT_BINARY_DIR}/include
)

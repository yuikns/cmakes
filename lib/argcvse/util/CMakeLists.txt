SET(ARGCVSE_UTIL_VERSION 1.0)
SET(ARGCVSE_UTIL_SOVERSION 0.1)

#INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)


# ARGCV_UTIL start
AUX_SOURCE_DIRECTORY(. LIBARGCV_UTIL_SRC)
ADD_LIBRARY(argcvse_util SHARED ${LIBARGCV_UTIL_SRC})
SET_TARGET_PROPERTIES(argcvse_util PROPERTIES VERSION ${ARGCVSE_UTIL_VERSION} SOVERSION ${ARGCVSE_UTIL_SOVERSION})

# ADD_DEPENDENCIES(argcvse_util networks)




ADD_LIBRARY(argcvse_util_static STATIC ${LIBARGCV_UTIL_SRC})
SET_TARGET_PROPERTIES(argcvse_util_static PROPERTIES OUTPUT_NAME "argcvse_util")


ADD_DEPENDENCIES(argcvse_util leveldb_proj)
# ADD_DEPENDENCIES(argcvse_util m)

TARGET_LINK_LIBRARIES(argcvse_util m)
TARGET_LINK_LIBRARIES(argcvse_util leveldb)


ADD_DEPENDENCIES(argcvse_util_static leveldb_proj)
# ADD_DEPENDENCIES(argcvse_util_static m)

TARGET_LINK_LIBRARIES(argcvse_util_static m)
TARGET_LINK_LIBRARIES(argcvse_util_static leveldb)


# INSTALL (TARGETS argcvse_util argcvse_util_static
#     LIBRARY DESTINATION lib64
#     ARCHIVE DESTINATION lib64)

# INSTALL (FILES ${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/include/argcvse/util.hpp DESTINATION include/argcvse/ )

# ARGCV_UTIL end





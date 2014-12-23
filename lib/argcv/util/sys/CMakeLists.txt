SET(ARGCV_UTIL_SYS_VERSION 1.0)
SET(ARGCV_UTIL_SYS_SOVERSION 0.1)

# SET(LIBARGCV_UTIL_SYS_SRC oid.cc)
AUX_SOURCE_DIRECTORY(. LIBARGCV_UTIL_SYS_SRC)
ADD_LIBRARY(argcv_util_sys SHARED ${LIBARGCV_UTIL_SYS_SRC})
SET_TARGET_PROPERTIES(argcv_util_sys PROPERTIES VERSION ${ARGCV_UTIL_SYS_VERSION} SOVERSION ${ARGCV_UTIL_SYS_SOVERSION})

ADD_LIBRARY(argcv_util_sys_static STATIC ${LIBARGCV_UTIL_SYS_SRC})
SET_TARGET_PROPERTIES(argcv_util_sys_static PROPERTIES OUTPUT_NAME "argcv_util_sys")

#ADD_DEPENDENCIES(argcv_util_sys leveldb_proj)
ADD_DEPENDENCIES(argcv_util_sys argcv_util)
ADD_DEPENDENCIES(argcv_util_sys argcv_util_dict)
TARGET_LINK_LIBRARIES(argcv_util_sys leveldb)
TARGET_LINK_LIBRARIES(argcv_util_sys argcv_util)
TARGET_LINK_LIBRARIES(argcv_util_sys argcv_util_dict)

#ADD_DEPENDENCIES(argcv_util_sys_static leveldb_proj)
ADD_DEPENDENCIES(argcv_util_sys_static argcv_util_static)
ADD_DEPENDENCIES(argcv_util_sys_static argcv_util_dict_static)
TARGET_LINK_LIBRARIES(argcv_util_sys_static leveldb)
TARGET_LINK_LIBRARIES(argcv_util_sys_static argcv_util_static)
TARGET_LINK_LIBRARIES(argcv_util_sys_static argcv_util_dict_static)

# INSTALL (TARGETS argcv_util argcv_util_static
#     LIBRARY DESTINATION lib64
#     ARCHIVE DESTINATION lib64)

# INSTALL (FILES ${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/include/argcv/util.h DESTINATION include/argcv/ )




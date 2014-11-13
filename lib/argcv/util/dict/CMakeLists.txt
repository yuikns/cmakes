SET(ARGCV_UTIL_DICT_VERSION 1.0)
SET(ARGCV_UTIL_DICT_SOVERSION 0.1)

################### UTIL EXT : DICT

SET(LIBARGCV_UTIL_DICT_SRC dict.cc)
ADD_LIBRARY(argcv_util_dict SHARED ${LIBARGCV_UTIL_DICT_SRC})
SET_TARGET_PROPERTIES(argcv_util_dict PROPERTIES VERSION ${ARGCV_UTIL_DICT_VERSION} SOVERSION ${ARGCV_UTIL_DICT_SOVERSION})

ADD_LIBRARY(argcv_util_dict_static STATIC ${LIBARGCV_UTIL_DICT_SRC})
SET_TARGET_PROPERTIES(argcv_util_dict_static PROPERTIES OUTPUT_NAME "argcv_util_dict")

ADD_DEPENDENCIES(argcv_util_dict leveldb_proj)
ADD_DEPENDENCIES(argcv_util_dict argcv_util)

TARGET_LINK_LIBRARIES(argcv_util_dict leveldb)
TARGET_LINK_LIBRARIES(argcv_util_dict argcv_util)
TARGET_LINK_LIBRARIES(argcv_util_dict pthread)  # for leveldb

ADD_DEPENDENCIES(argcv_util_dict_static leveldb_proj)
ADD_DEPENDENCIES(argcv_util_dict_static argcv_util_static)
TARGET_LINK_LIBRARIES(argcv_util_dict_static leveldb)
TARGET_LINK_LIBRARIES(argcv_util_dict_static argcv_util_static)
TARGET_LINK_LIBRARIES(argcv_util_dict_static pthread) # for leveldb


# INSTALL (TARGETS argcv_util argcv_util_static
#     LIBRARY DESTINATION lib64
#     ARCHIVE DESTINATION lib64)

# INSTALL (FILES ${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/include/argcv/util.h DESTINATION include/argcv/ )




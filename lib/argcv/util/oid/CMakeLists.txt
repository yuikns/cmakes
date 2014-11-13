SET(ARGCV_UTIL_OID_VERSION 1.0)
SET(ARGCV_UTIL_OID_SOVERSION 0.1)

################### UTIL EXT : OID

SET(LIBARGCV_UTIL_OID_SRC oid.cc)
ADD_LIBRARY(argcv_util_oid SHARED ${LIBARGCV_UTIL_OID_SRC})
SET_TARGET_PROPERTIES(argcv_util_oid PROPERTIES VERSION ${ARGCV_UTIL_OID_VERSION} SOVERSION ${ARGCV_UTIL_OID_SOVERSION})

ADD_LIBRARY(argcv_util_oid_static STATIC ${LIBARGCV_UTIL_OID_SRC})
SET_TARGET_PROPERTIES(argcv_util_oid_static PROPERTIES OUTPUT_NAME "argcv_util_oid")

#ADD_DEPENDENCIES(argcv_util_oid leveldb_proj)
ADD_DEPENDENCIES(argcv_util_oid argcv_util)
ADD_DEPENDENCIES(argcv_util_oid argcv_util_dict)
TARGET_LINK_LIBRARIES(argcv_util_oid leveldb)
TARGET_LINK_LIBRARIES(argcv_util_oid argcv_util)
TARGET_LINK_LIBRARIES(argcv_util_oid argcv_util_dict)

#ADD_DEPENDENCIES(argcv_util_oid_static leveldb_proj)
ADD_DEPENDENCIES(argcv_util_oid_static argcv_util_static)
ADD_DEPENDENCIES(argcv_util_oid_static argcv_util_dict_static)
TARGET_LINK_LIBRARIES(argcv_util_oid_static leveldb)
TARGET_LINK_LIBRARIES(argcv_util_oid_static argcv_util_static)
TARGET_LINK_LIBRARIES(argcv_util_oid_static argcv_util_dict_static)

# INSTALL (TARGETS argcv_util argcv_util_static
#     LIBRARY DESTINATION lib64
#     ARCHIVE DESTINATION lib64)

# INSTALL (FILES ${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/include/argcv/util.h DESTINATION include/argcv/ )




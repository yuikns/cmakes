SET(ARGCV_GRAPH_VERSION 1.0)
SET(ARGCV_GRAPH_SOVERSION 0.1)

################### UTIL EXT : GRAPH
AUX_SOURCE_DIRECTORY(. LIBARGCV_GRAPH_SRC)
ADD_LIBRARY(argcv_graph SHARED ${LIBARGCV_GRAPH_SRC})
SET_TARGET_PROPERTIES(argcv_graph PROPERTIES VERSION ${ARGCV_GRAPH_VERSION} SOVERSION ${ARGCV_GRAPH_SOVERSION})

ADD_LIBRARY(argcv_graph_static STATIC ${LIBARGCV_GRAPH_SRC})
SET_TARGET_PROPERTIES(argcv_graph_static PROPERTIES OUTPUT_NAME "argcv_graph")

ADD_DEPENDENCIES(argcv_graph leveldb_proj)
ADD_DEPENDENCIES(argcv_graph argcv_util_oid_static)

TARGET_LINK_LIBRARIES(argcv_graph leveldb)
TARGET_LINK_LIBRARIES(argcv_graph pthread)
TARGET_LINK_LIBRARIES(argcv_graph argcv_util)
TARGET_LINK_LIBRARIES(argcv_graph argcv_util_dict)
TARGET_LINK_LIBRARIES(argcv_graph argcv_util_oid)


ADD_DEPENDENCIES(argcv_graph_static leveldb_proj)
ADD_DEPENDENCIES(argcv_graph_static argcv_util_oid_static)

TARGET_LINK_LIBRARIES(argcv_graph_static leveldb)
TARGET_LINK_LIBRARIES(argcv_graph_static pthread)
TARGET_LINK_LIBRARIES(argcv_graph_static argcv_util_static)
TARGET_LINK_LIBRARIES(argcv_graph_static argcv_util_dict_static)
TARGET_LINK_LIBRARIES(argcv_graph_static argcv_util_oid_static)

# INSTALL (TARGETS argcv_util argcv_util_static
#     LIBRARY DESTINATION lib64
#     ARCHIVE DESTINATION lib64)

# INSTALL (FILES ${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/include/argcv/util.h DESTINATION include/argcv/ )




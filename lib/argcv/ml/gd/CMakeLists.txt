SET(ARGCV_ML_GD_VERSION 1.0)
SET(ARGCV_ML_GD_SOVERSION 0.1)

################### UTIL EXT : GD

SET(LIBARGCV_ML_GD_SRC gd.cc)
ADD_LIBRARY(argcv_ml_gd SHARED ${LIBARGCV_ML_GD_SRC})
SET_TARGET_PROPERTIES(argcv_ml_gd PROPERTIES VERSION ${ARGCV_ML_GD_VERSION} SOVERSION ${ARGCV_ML_GD_SOVERSION})

ADD_LIBRARY(argcv_ml_gd_static STATIC ${LIBARGCV_ML_GD_SRC})
SET_TARGET_PROPERTIES(argcv_ml_gd_static PROPERTIES OUTPUT_NAME "argcv_ml_gd")

#ADD_DEPENDENCIES(argcv_ml_gd leveldb_proj)
#ADD_DEPENDENCIES(argcv_ml_gd argcv_util)
#ADD_DEPENDENCIES(argcv_ml_gd argcv_util_dict)
#TARGET_LINK_LIBRARIES(argcv_ml_gd leveldb)
#TARGET_LINK_LIBRARIES(argcv_ml_gd argcv_util)
#TARGET_LINK_LIBRARIES(argcv_ml_gd argcv_util_dict)

#ADD_DEPENDENCIES(argcv_ml_gd_static leveldb_proj)
#ADD_DEPENDENCIES(argcv_ml_gd_static argcv_util_static)
#ADD_DEPENDENCIES(argcv_ml_gd_static argcv_util_dict_static)
#TARGET_LINK_LIBRARIES(argcv_ml_gd_static leveldb)
#TARGET_LINK_LIBRARIES(argcv_ml_gd_static argcv_util_static)
#TARGET_LINK_LIBRARIES(argcv_ml_gd_static argcv_util_dict_static)

# INSTALL (TARGETS argcv_util argcv_util_static
#     LIBRARY DESTINATION lib64
#     ARCHIVE DESTINATION lib64)

# INSTALL (FILES ${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/include/argcv/util.h DESTINATION include/argcv/ )




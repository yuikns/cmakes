# all related files
# AUX_SOURCE_DIRECTORY(. DIR_TARGET)
# ADD_LIBRARY (relfiles ${DIR_TARGET})


# main exec
# SET(SRC_LIST argcvse.cc)
ADD_EXECUTABLE(cmake_sample main.cc)


ADD_DEPENDENCIES(cmake_sample iniparser_static argcvse_util_static)

IF(OPTIONS_USE_EXTERNAL_CRFPP)
	ADD_DEPENDENCIES(cmake_sample crfpp_proj)
ENDIF(OPTIONS_USE_EXTERNAL_CRFPP)

IF(OPTIONS_USE_EXTERNAL_LEVELDB)
	ADD_DEPENDENCIES(cmake_sample leveldb_proj)
ENDIF(OPTIONS_USE_EXTERNAL_LEVELDB)




#  executable target links
# TARGET_LINK_LIBRARIES(abcs relfiles)
TARGET_LINK_LIBRARIES(cmake_sample iniparser_static)
TARGET_LINK_LIBRARIES(cmake_sample leveldb)
#TARGET_LINK_LIBRARIES(cmake_sample crfpp)
TARGET_LINK_LIBRARIES(cmake_sample argcvse_util_static)

#  executable properties
SET_TARGET_PROPERTIES(cmake_sample PROPERTIES OUTPUT_NAME "cmake_sample")





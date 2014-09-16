# all related files
# AUX_SOURCE_DIRECTORY(. DIR_TARGET)
# ADD_LIBRARY (relfiles ${DIR_TARGET})


# main exec
# SET(SRC_LIST argcvse.cc)
ADD_EXECUTABLE(cmake_sample main.cc)

#  executable dependencies
# ADD_DEPENDENCIES(cmake_sample relfiles)
# ADD_DEPENDENCIES(cmake_sample iniparser_static)

# set_property(TARGET leveldb_proj PROPERTY EXCLUDE_FROM_ALL TRUE)
# set_property(TARGET crfpp_proj PROPERTY EXCLUDE_FROM_ALL TRUE)

ADD_DEPENDENCIES(cmake_sample iniparser_static crfpp_proj leveldb_proj)

# IF(LEVELDB_EX_PROJ)
# 	MESSAGE(STATUS "extend project leveldb to be finished ")
# 	ADD_DEPENDENCIES(cmake_sample LEVELDB_EX_PROJ)
# ENDIF()

# IF(CRFPP_EX_PROJ)
# 	MESSAGE(STATUS "extend project crfpp to be finished ")
# 	ADD_DEPENDENCIES(cmake_sample CRFPP_EX_PROJ)
# ENDIF()


#  executable target links
# TARGET_LINK_LIBRARIES(abcs relfiles)
TARGET_LINK_LIBRARIES(cmake_sample iniparser_static)
TARGET_LINK_LIBRARIES(cmake_sample leveldb)
TARGET_LINK_LIBRARIES(cmake_sample crfpp)

#  executable properties
SET_TARGET_PROPERTIES(cmake_sample PROPERTIES OUTPUT_NAME "cmake_sample")





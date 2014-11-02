# all related files
# AUX_SOURCE_DIRECTORY(. DIR_TARGET)
# ADD_LIBRARY (relfiles ${DIR_TARGET})


# main exec
# SET(SRC_LIST argcvse.cc)
ADD_EXECUTABLE(DictReader main.cc)


ADD_DEPENDENCIES(DictReader inihpp_static argcv_util_static argcv_util_dict_static)

#IF(OPTIONS_USE_EXTERNAL_CRFPP)
#	ADD_DEPENDENCIES(DictReader crfpp_proj)
#ENDIF(OPTIONS_USE_EXTERNAL_CRFPP)

#IF(OPTIONS_USE_EXTERNAL_LEVELDB)
ADD_DEPENDENCIES(DictReader leveldb_proj)
#ENDIF(OPTIONS_USE_EXTERNAL_LEVELDB)


#  executable target links
# TARGET_LINK_LIBRARIES(abcs relfiles)
TARGET_LINK_LIBRARIES(DictReader inihpp_static)
TARGET_LINK_LIBRARIES(DictReader leveldb)
TARGET_LINK_LIBRARIES(DictReader pthread) # for leveldb
#TARGET_LINK_LIBRARIES(DictReader crfpp)
TARGET_LINK_LIBRARIES(DictReader argcv_util_static)
TARGET_LINK_LIBRARIES(DictReader argcv_util_dict_static)

#  executable properties
SET_TARGET_PROPERTIES(DictReader PROPERTIES OUTPUT_NAME "DictReader")


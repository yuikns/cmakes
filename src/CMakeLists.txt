# all related files
# AUX_SOURCE_DIRECTORY(. DIR_TARGET)
# ADD_LIBRARY (relfiles ${DIR_TARGET})


# main exec
# SET(SRC_LIST argcvse.cc)
ADD_EXECUTABLE(DictReader main.cc)


ADD_DEPENDENCIES(DictReader iniparser_static argcvse_util_static)

IF(OPTIONS_USE_EXTERNAL_CRFPP)
	ADD_DEPENDENCIES(DictReader crfpp_proj)
ENDIF(OPTIONS_USE_EXTERNAL_CRFPP)

IF(OPTIONS_USE_EXTERNAL_LEVELDB)
	ADD_DEPENDENCIES(DictReader leveldb_proj)
ENDIF(OPTIONS_USE_EXTERNAL_LEVELDB)




#  executable target links
# TARGET_LINK_LIBRARIES(abcs relfiles)
TARGET_LINK_LIBRARIES(DictReader iniparser_static)
TARGET_LINK_LIBRARIES(DictReader leveldb)
#TARGET_LINK_LIBRARIES(DictReader crfpp)
TARGET_LINK_LIBRARIES(DictReader argcvse_util_static)

#  executable properties
SET_TARGET_PROPERTIES(DictReader PROPERTIES OUTPUT_NAME "DictReader")


#ADD_EXECUTABLE(hello_mongo hello_mongo.cc)

#IF(OPTIONS_USE_EXTERNAL_MONGO_CXX)
#	ADD_DEPENDENCIES(hello_mongo mongo_cxx_proj)
#ENDIF(OPTIONS_USE_EXTERNAL_MONGO_CXX)

#TARGET_LINK_LIBRARIES(hello_mongo pthread)
#TARGET_LINK_LIBRARIES(hello_mongo mongoclient)
#TARGET_LINK_LIBRARIES(hello_mongo boost_thread-mt)
#TARGET_LINK_LIBRARIES(hello_mongo boost_filesystem)
#TARGET_LINK_LIBRARIES(hello_mongo boost_program_options)
#TARGET_LINK_LIBRARIES(hello_mongo boost_system)
#TARGET_LINK_LIBRARIES(hello_mongo ssl)
#TARGET_LINK_LIBRARIES(hello_mongo crypto)

#SET_TARGET_PROPERTIES(hello_mongo PROPERTIES OUTPUT_NAME "hello_mongo")





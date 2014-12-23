# all related files
# AUX_SOURCE_DIRECTORY(. DIR_TARGET)
# ADD_LIBRARY (relfiles ${DIR_TARGET})


# main exec
# SET(SRC_LIST argcvse.cc)
ADD_EXECUTABLE(DictReader main.cc)


ADD_DEPENDENCIES(DictReader inihpp_static)
ADD_DEPENDENCIES(DictReader argcv_util_static)
ADD_DEPENDENCIES(DictReader argcv_util_dict_static)

#IF(OPTIONS_USE_EXTERNAL_CRFPP)
#	ADD_DEPENDENCIES(DictReader crfpp_proj)
#ENDIF(OPTIONS_USE_EXTERNAL_CRFPP)

#IF(OPTIONS_USE_EXTERNAL_LEVELDB)
ADD_DEPENDENCIES(DictReader leveldb_proj)
#ENDIF(OPTIONS_USE_EXTERNAL_LEVELDB)

ADD_DEPENDENCIES(DictReader argcv_util_oid_static)

#  executable target links
# TARGET_LINK_LIBRARIES(abcs relfiles)
TARGET_LINK_LIBRARIES(DictReader inihpp_static)
TARGET_LINK_LIBRARIES(DictReader leveldb)
TARGET_LINK_LIBRARIES(DictReader pthread) # for leveldb
#TARGET_LINK_LIBRARIES(DictReader crfpp)
TARGET_LINK_LIBRARIES(DictReader argcv_util_static)
TARGET_LINK_LIBRARIES(DictReader argcv_util_dict_static)
TARGET_LINK_LIBRARIES(DictReader argcv_util_oid_static)

#  executable properties
SET_TARGET_PROPERTIES(DictReader PROPERTIES OUTPUT_NAME "DictReader")

##################################### End Of DictReader ##################################

ADD_EXECUTABLE(HelloML hello_ml.cc)

ADD_DEPENDENCIES(HelloML inihpp_static)
ADD_DEPENDENCIES(HelloML argcv_util_static)
ADD_DEPENDENCIES(HelloML argcv_ml_gd_static)


TARGET_LINK_LIBRARIES(HelloML inihpp_static)
TARGET_LINK_LIBRARIES(HelloML argcv_util_static)
TARGET_LINK_LIBRARIES(HelloML argcv_ml_gd_static)


#  executable properties
SET_TARGET_PROPERTIES(HelloML PROPERTIES OUTPUT_NAME "HelloML")

##################################### End Of HelloML ##################################

ADD_EXECUTABLE(HelloGraph hello_graph.cc)

ADD_DEPENDENCIES(HelloGraph inihpp_static)
ADD_DEPENDENCIES(HelloGraph argcv_util_static)
ADD_DEPENDENCIES(HelloGraph argcv_graph_static)


TARGET_LINK_LIBRARIES(HelloGraph inihpp_static)
TARGET_LINK_LIBRARIES(HelloGraph argcv_util_static)
TARGET_LINK_LIBRARIES(HelloGraph argcv_graph_static)


#  executable properties
SET_TARGET_PROPERTIES(HelloGraph PROPERTIES OUTPUT_NAME "HelloGraph")

##################################### End Of HelloGraph ##################################


##############################################################
####
#### EVENT SECTION 
####
##############################################################

### SERVER 
ADD_EXECUTABLE(HelloEventS hello_event_s.cc)

#ADD_DEPENDENCIES(HelloEventS event)
ADD_DEPENDENCIES(HelloEventS event_mq)

TARGET_LINK_LIBRARIES(HelloEventS event)
TARGET_LINK_LIBRARIES(HelloEventS event_mq)

#  executable properties
SET_TARGET_PROPERTIES(HelloEventS PROPERTIES OUTPUT_NAME "HelloEventS")


### CLIENT
ADD_EXECUTABLE(HelloEventC hello_event_c.cc)

#ADD_DEPENDENCIES(HelloEventS event)
ADD_DEPENDENCIES(HelloEventC event_mq)

TARGET_LINK_LIBRARIES(HelloEventC event)
TARGET_LINK_LIBRARIES(HelloEventC event_mq)

#  executable properties
SET_TARGET_PROPERTIES(HelloEventC PROPERTIES OUTPUT_NAME "HelloEventC")















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



##############################################################
####
#### OpenGL Section
####
##############################################################

## OpenGL
FIND_PACKAGE(OpenGL REQUIRED)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

## OpenGL
FIND_PACKAGE(GLUT REQUIRED)
INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})

ADD_EXECUTABLE(HelloGL hello_gl.cc)

TARGET_LINK_LIBRARIES(HelloGL ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

#  executable properties
SET_TARGET_PROPERTIES(HelloGL PROPERTIES OUTPUT_NAME "HelloGL")


##############################################################
####
#### Mongo Section
####
##############################################################
ADD_EXECUTABLE(HelloMongo hello_mongo.cc)

ADD_DEPENDENCIES(HelloMongo inihpp_static argcv_util_static argcv_util_dict_static)

ADD_DEPENDENCIES(HelloMongo leveldb_proj)
ADD_DEPENDENCIES(HelloMongo mongo_cxx_proj)

TARGET_LINK_LIBRARIES(HelloMongo inihpp_static)
TARGET_LINK_LIBRARIES(HelloMongo leveldb)
TARGET_LINK_LIBRARIES(HelloMongo pthread) # for leveldb
#TARGET_LINK_LIBRARIES(HelloMongo crfpp)
TARGET_LINK_LIBRARIES(HelloMongo argcv_util_static)
TARGET_LINK_LIBRARIES(HelloMongo argcv_util_dict_static)

# for mongodb
TARGET_LINK_LIBRARIES(HelloMongo pthread)
TARGET_LINK_LIBRARIES(HelloMongo mongoclient)
TARGET_LINK_LIBRARIES(HelloMongo boost_thread-mt)
TARGET_LINK_LIBRARIES(HelloMongo boost_filesystem)
TARGET_LINK_LIBRARIES(HelloMongo boost_program_options)
TARGET_LINK_LIBRARIES(HelloMongo boost_system)
TARGET_LINK_LIBRARIES(HelloMongo ssl)
TARGET_LINK_LIBRARIES(HelloMongo crypto)

SET_TARGET_PROPERTIES(HelloMongo PROPERTIES OUTPUT_NAME "HelloMongo")

#################################################################
####
#### Protobuf
####
#################################################################

## NOT COMPLETED
INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf REQUIRED)

ADD_EXECUTABLE(HelloPb hello_protobuf.cc)

#ADD_DEPENDENCIES(HelloEventS event)
#ADD_DEPENDENCIES(HelloPb event_mq)

#TARGET_LINK_LIBRARIES(HelloPb event)
#TARGET_LINK_LIBRARIES(HelloPb event_mq)

#  executable properties
SET_TARGET_PROPERTIES(HelloPb PROPERTIES OUTPUT_NAME "HelloPb")












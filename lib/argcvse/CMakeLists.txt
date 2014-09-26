################################################################################

# include settings
#INCLUDE_DIRECTORIES(${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/lib)
#INCLUDE_DIRECTORIES(${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/lib/include)
#INCLUDE_DIRECTORIES(${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/include)
#INCLUDE_DIRECTORIES(${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/include/argcvse)

# lib settings
#LINK_DIRECTORIES(${${CMAKE_PROJECT_NAME}_BINARY_DIR}/lib)


###### SUBDIRECTORIES
# lib list
#ADD_SUBDIRECTORY(dirtrav)
# ADD_SUBDIRECTORY(index)
# ADD_SUBDIRECTORY(rpc)
# ADD_SUBDIRECTORY(analyzer)
ADD_SUBDIRECTORY(util)


# # main library
# ADD_LIBRARY (argcvse STATIC argcvse.cc)

# # ADD_DEPENDENCIES(util networks)

# SET_TARGET_PROPERTIES(argcvse PROPERTIES OUTPUT_NAME "argcvse")

# # depencies
# # ADD_DEPENDENCIES(argcvse index)
# ADD_DEPENDENCIES(argcvse argcvse_index_static)
# ADD_DEPENDENCIES(argcvse argcvse_rpc_static)
# ADD_DEPENDENCIES(argcvse argcvse_analyzer_static)
# ADD_DEPENDENCIES(argcvse argcvse_util_static)

# # link libraries
# #TARGET_LINK_LIBRARIES(argcvse index)
# TARGET_LINK_LIBRARIES(argcvse argcvse_index_static)
# TARGET_LINK_LIBRARIES(argcvse argcvse_rpc_static)
# TARGET_LINK_LIBRARIES(argcvse argcvse_analyzer_static)
# TARGET_LINK_LIBRARIES(argcvse argcvse_util_static)







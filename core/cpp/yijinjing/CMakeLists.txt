PROJECT(yijinjing)

############################################################

INCLUDE_DIRECTORIES(include)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(pybind)
ADD_SUBDIRECTORY(node)

SET(yijinjing_build_include_dir "${yijinjing_SOURCE_DIR}/include/kungfu")
# NOTE: Don't use PUBLIC_HEADER to install. It does not keep folder
# structure.
INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
INSTALL(DIRECTORY ${yijinjing_build_include_dir} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

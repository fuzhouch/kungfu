PROJECT(wingchun)

############################################################

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/cpp/yijinjing/include)
INCLUDE_DIRECTORIES(include)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(pybind)


SET(wingchun_build_include_dir "${wingchun_SOURCE_DIR}/include/kungfu")
# NOTE: Don't use PUBLIC_HEADER to install. It does not keep folder
# structure.
INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
INSTALL(DIRECTORY ${wingchun_build_include_dir} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

PROJECT(rxcpp)
INCLUDE(GNUInstallDirs)
SET(rxcpp_build_include_dir "${rxcpp_SOURCE_DIR}/include/rxcpp")
INSTALL(DIRECTORY ${rxcpp_build_include_dir} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

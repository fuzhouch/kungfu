PROJECT(spdlog)
INCLUDE(GNUInstallDirs)
SET(spdlog_build_include_dir "${spdlog_SOURCE_DIR}/include/spdlog")
INSTALL(DIRECTORY ${spdlog_build_include_dir} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

cmake_minimum_required (VERSION 3.7.0)

cmake_policy(SET CMP0054 NEW)

project (game)

include("${CMAKE_SOURCE_DIR}/3rd_party/3rd_party.prepare.cmake")

set (PROJECT_CMAKE_MODULE_DIR "${CMAKE_SOURCE_DIR}/project/cmake")

# 编译选项
#####################################################################
include("${PROJECT_CMAKE_MODULE_DIR}/CompilerOption.cmake")
include("${PROJECT_CMAKE_MODULE_DIR}/TargetOption.cmake")
list (APPEND CMAKE_MODULE_PATH "${PROJECT_CMAKE_MODULE_DIR}/modules")
include(IncludeDirectoryRecurse)
include(EchoWithColor)
include(FindConfigurePackage)

# 导入工程工具
include("${CMAKE_CURRENT_LIST_DIR}/project/cmake/ProjectTools.cmake")

# installer directory
set (PROJECT_INSTALL_BAS_DIR "${PROJECT_BINARY_DIR}/${PROJECT_BUILD_NAME}")
if(NOT EXISTS ${PROJECT_INSTALL_BAS_DIR})
    file(MAKE_DIRECTORY ${PROJECT_INSTALL_BAS_DIR})
    message(STATUS "create ${PROJECT_INSTALL_BAS_DIR} for build target.")
endif()

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_INSTALL_BAS_DIR}/lib")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_INSTALL_BAS_DIR}/lib")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_INSTALL_BAS_DIR}/bin")

set (PROJECT_INSTALL_RES_DIR "${PROJECT_INSTALL_BAS_DIR}/resource")
set (PROJECT_INSTALL_TOOLS_DIR  "${PROJECT_INSTALL_BAS_DIR}/tools")
set (PROJECT_INSTALL_SHARED_DIR "${PROJECT_INSTALL_TOOLS_DIR}/shared")
set (PROJECT_INSTALL_RES_PBD_DIR "${PROJECT_INSTALL_RES_DIR}/pbdesc")

file(MAKE_DIRECTORY ${PROJECT_INSTALL_SHARED_DIR})
file(MAKE_DIRECTORY ${PROJECT_INSTALL_RES_PBD_DIR})

# 导入项目配置
## 导入所有 macro 定义
include_macro_recurse(${PROJECT_3RD_PARTY_ROOT_DIR})


# 工程选项
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/gamed")




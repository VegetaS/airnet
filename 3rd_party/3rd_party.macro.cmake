# =========== 3rd_party ===========
set (PROJECT_3RD_PARTY_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

# =========== 3rd_party - protobuf ===========
include("${PROJECT_3RD_PARTY_ROOT_DIR}/protobuf.cmake")

# =========== 3rd_party - muduo ===========
include("${PROJECT_3RD_PARTY_ROOT_DIR}/muduo.cmake")


## 导入所有工程项目
add_project_recurse(${CMAKE_CURRENT_LIST_DIR})

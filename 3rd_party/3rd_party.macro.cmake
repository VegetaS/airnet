# =========== 3rd_party ===========
unset (PROJECT_3RD_PARTY_SRC_LIST)

# =========== 3rd_party - protobuf ===========
include("${PROJECT_3RD_PARTY_ROOT_DIR}/protobuf/protobuf.cmake")

# =========== 3rd_party - muduo ===========
include("${PROJECT_3RD_PARTY_ROOT_DIR}/muduo/muduo.cmake")


## 导入所有工程项目
add_project_recurse(${CMAKE_CURRENT_LIST_DIR})

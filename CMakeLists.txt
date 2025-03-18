cmake_minimum_required (VERSION 3.5)
project (chat)
 #配置编译选项
set(CMAKE_CXX_FLAGS,${CMAKE_CXX_FLAGS} -g)

#设置静态库生成路径
set(LIB_PATH ${PROJECT_SOURCE_DIR}/lib)
#配置最终的可执行文件输出的路径,输出到bin目录下
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#配置头文件的搜索路径
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/crypto)
include_directories(${PROJECT_SOURCE_DIR}/include/server)
include_directories(${PROJECT_SOURCE_DIR}/include/server/db)
include_directories(${PROJECT_SOURCE_DIR}/include/server/model)
include_directories(${PROJECT_SOURCE_DIR}/include/server/redis)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)



# 静态库的名字
set(MYCRYPTO_LIB mycrypto_lib)
#指定动态库的路径
#link_directories(/usr/local/ssl/lib64/)

#加载子目录
add_subdirectory (src)
add_subdirectory (crypto)
cmake_minimum_required (VERSION 3.5)
project(myclient)
aux_source_directory(. SRC_LIST)
#包含静态库文件
link_directories(${LIB_PATH})
#链接静态库
link_libraries(${MYCRYPTO_LIB})
add_executable(chatClient ${SRC_LIST})

target_link_libraries(chatClient pthread)
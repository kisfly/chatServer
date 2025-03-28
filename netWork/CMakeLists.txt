cmake_minimum_required (VERSION 3.5)
project(mynetwork)
set(CMAKE_CXX_STANDARD 17)


aux_source_directory(./ SRC)
#设置静态库生成路径
set(LIBRARY_OUTPUT_PATH ${LIB_PATH})
#将源文件制作为静态库文件
add_library(${MYNET_LIB} STATIC ${SRC})
#target_link_libraries(${MYNET_LIB})

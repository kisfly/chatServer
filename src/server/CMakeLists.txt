#定义了一个SRC_LIST,包含了该目录相爱所有的源文件
aux_source_directory(. SRC_LIST)
aux_source_directory(./db DB_LIST)
aux_source_directory(./model MODEL_LIST)
aux_source_directory(./redis REDIS_LIST)

#指定生成可执行文件
add_executable(chatServer ${SRC_LIST} ${DB_LIST} ${MODEL_LIST} ${REDIS_LIST})

#添加需要链接的动态库
target_link_libraries(chatServer muduo_net muduo_base mysqlclient hiredis pthread)
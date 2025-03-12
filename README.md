# chatServer
工作在nginx tcp负载均衡环境中的集群聊天服务器和客户端，基于muduo库实现，采用mysql实现数据存储，redis实现订阅-发布，使用数据库连接池管理数据库连接
需要使用nginx配置tcp负载均衡-以实现集群服务器，以及使用服务器中间件基于发布-订阅模式的redis来实现跨服务器通信
编译方式：
cd build
rm *rf *
cmake ..
make



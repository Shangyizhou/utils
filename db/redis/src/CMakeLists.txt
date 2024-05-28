# 我们可以在子目录找到所有的 src 文件，然后打包成动态库，动态库的路径已经在顶级Cmake中设置好了
set(SRCS
    RedisClient.cc
    Reply.cc

    )

add_library(redis-tool ${SRCS})
target_link_libraries(redis-tool pthread hiredis)



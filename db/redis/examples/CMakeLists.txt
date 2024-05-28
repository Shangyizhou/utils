# 每增加一个文件就重复写一次
# 默认链接已经生成好的 lib 

add_executable(SimpleTest SimpleTest.cc)
target_link_libraries(SimpleTest redis-tool)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/examples)

add_executable(test test.cc)
target_link_libraries(test hiredis)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/examples)
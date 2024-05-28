# 每增加一个文件就重复写一次
# 默认链接已经生成好的 lib 

add_executable(test HelloTest.cc)
target_link_libraries(test project)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/tests)


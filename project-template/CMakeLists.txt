# 指定使用 CMake 的最低版本号
cmake_minimum_required(VERSION 3.0)

# 设置项目名称
project(project C CXX)

# 设置C++编译参数（CMAKE_CXX_FLAGS是全局变量）
# set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11 -fPIC")

# 添加搜索路径（顶级Cmake目录）
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    )


# 设置库生成路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 引入第三方库，自动下载
find_package(CURL)

# 加载子目录
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(examples)
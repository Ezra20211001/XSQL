cmake_minimum_required(VERSION 2.8)
project("XSql" CXX)

SET(PROJECT ${CMAKE_CURRENT_SOURCE_DIR})
SET(BINDIR ${PROJECT})

# 添加依赖头文件目录
SET(INCDIR
    ${PROJECT}/BaseTools
    ${PROJECT}/Common
    ${PROJECT}/Depends/tinyxml
    src
)
INCLUDE_DIRECTORIES(${INCDIR})

# 设置宏定义，如果需要的话
# ADD_DEFINITIONS(-DNEW_LOG)

SET(SRCS
    ${PROJECT}/BaseTools/StringUtils.cpp
    ${PROJECT}/Depends/tinyxml/tinystr.cpp
    ${PROJECT}/Depends/tinyxml/tinyxml.cpp
    ${PROJECT}/Depends/tinyxml/tinyxmlerror.cpp
    ${PROJECT}/Depends/tinyxml/tinyxmlparser.cpp
    src/DataConnector.cpp
    src/DataProvider.cpp
    src/DataQuery.cpp
    src/XSqlLoader.cpp
    main.cpp
)

SET(LIBDIR
    /usr/lib64/mysql
)

SET(LIB_SO
    -lpthread
    -lmysqlclient
)

SET(RUN_MAIN "XSql")

LINK_DIRECTORIES(${LIBDIR})

# 设置C++编译选项，如果需要的话
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -march=native -Wall") 
# 添加可执行文件，并配相关文件
ADD_EXECUTABLE(${RUN_MAIN} ${SRCS})
# 添加依赖，保证动态库编译完后，才编译可执行程序
#ADD_DEPENDENCIES(${RUN_MAIN} ${LIB_DBSO})
# 链接依赖库
#TARGET_LINK_LIBRARIES(${RUN_MAIN} ${LIB_SO} ${LIB_DBSO})
TARGET_LINK_LIBRARIES(${RUN_MAIN} ${LIB_SO})

# 安装可执行文件到指定位置，并指定权限
INSTALL(TARGETS ${RUN_MAIN} DESTINATION ${BINDIR} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_EXECUTE)
# 指定运行时路径链接的库路径（同g++的rpath参数）
#SET_TARGET_PROPERTIES(${BIN_MAIN} PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

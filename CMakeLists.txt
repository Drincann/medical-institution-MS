cmake_minimum_required(VERSION 3.10)

project(MS VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(out main.c)
target_link_directories(out PUBLIC "${PROJECT_SOURCE_DIR}/lib")

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
  target_link_libraries(out libmysql)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux" OR CMAKE_SYSTEM_NAME MATCHES "Darwin")
  target_link_libraries(out mysqlclient)
endif()

target_include_directories(out PUBLIC "${PROJECT_SOURCE_DIR}/include")

if(CMAKE_SYSTEM_NAME MATCHES "Linux" OR CMAKE_SYSTEM_NAME MATCHES "Darwin")
  install(FILES ${PROJECT_SOURCE_DIR}/lib/libmysqlclient.21.dylib DESTINATION lib)
  install(FILES ${PROJECT_SOURCE_DIR}/lib/libssl.1.1.dylib DESTINATION lib)
  install(FILES ${PROJECT_SOURCE_DIR}/lib/libcrypto.1.1.dylib DESTINATION lib)
endif()

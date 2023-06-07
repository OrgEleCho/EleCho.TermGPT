cmake_minimum_required(VERSION 2.9)
project(TermGPT)
#cpp20
set(CMAKE_CXX_STANDARD 20)

#CJsonObject
#为cjsonobject构建一个静态库
set(CJSONOBJECT_SRC ${PROJECT_SOURCE_DIR}/CJsonObject/CJsonObject.cpp ${PROJECT_SOURCE_DIR}/CJsonObject/cJSON.c)
add_library(cjsonobject STATIC ${CJSONOBJECT_SRC})
include_directories(${PROJECT_SOURCE_DIR}/CJsonObject)

#inifile2
add_subdirectory("inifile2/src")
include_directories("inifile2/src")
#curl
find_package(CURL REQUIRED)

#src
file(GLOB_RECURSE DIR_SRCS "src/*.cpp" "src/colorize/*.cpp" "src/openai/*.cpp")

add_executable(TermGPT ${DIR_SRCS})
target_link_libraries(TermGPT cjsonobject)
target_link_libraries(TermGPT inifile)
target_link_libraries(TermGPT ${CURL_LIBRARIES})






cmake_minimum_required(VERSION 3.0)
project(BaiLuIME_CandidateGenerator_test1)
include_directories(../../../external/BaiLuIME_Common/thirdparty/json11/)
include_directories(../../../external/BaiLuIME_Common/include/)
include_directories(../../../include/)
SET(CORE_FILE_DIR ../../core/)
SET(CORE_SOURCE_FILE 
${CORE_FILE_DIR}/SimpleCandidateGenerator.cpp 
${CORE_FILE_DIR}/SimpleQuanPinGenerator.cpp
${CORE_FILE_DIR}/RimeIceDictionary.cpp
${CORE_FILE_DIR}/RimeCandidateGenerator.cpp
${CORE_FILE_DIR}/LocalTcpGenerator.cpp
${CORE_FILE_DIR}/TcpClient.cpp
../../../external/BaiLuIME_Common/thirdparty/json11/json11.cpp
)


include_directories(${CORE_FILE_DIR})
link_libraries(ws2_32.lib)
SET(SOURCE_FILE ./main.cpp)
add_executable(test1 ${SOURCE_FILE} ${CORE_SOURCE_FILE})
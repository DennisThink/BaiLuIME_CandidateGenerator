project(test3)
include_directories(${THIRD_PARTY_DIR}/doctest/doctest/)
include_directories(${THIRD_PARTY_DIR}/json11/)
include_directories(${THIRD_PARTY_DIR}/../include/)
include_directories(${CANDIDATE_GENERATOR_INCLUDE})
SET(CORE_FILE_DIR ../../core/)
SET(CORE_SOURCE_FILE 
${CORE_FILE_DIR}/SimpleCandidateGenerator.cpp 
${CORE_FILE_DIR}/SimpleQuanPinGenerator.cpp
${CORE_FILE_DIR}/RimeIceDictionary.cpp
${CORE_FILE_DIR}/RimeCandidateGenerator.cpp
${CORE_FILE_DIR}/LocalTcpGenerator.cpp
${CORE_FILE_DIR}/TcpClient.cpp
${CORE_FILE_DIR}/LevenshteinDistance.cpp
${CORE_FILE_DIR}/LongestCommonSubstringDistance.cpp
${CORE_FILE_DIR}/PinYinTable.cpp
${THIRD_PARTY_DIR}/json11/json11.cpp
)
link_libraries(ws2_32.lib)
SET(SOURCE_FILE ./main.cpp 
./test_QuanPinCandidateGenerator.cpp 
./test_RimeCandidateGenerator.cpp 
./test_LongestCommonSubstringDistance.cpp
./test_PinYinTable.cpp
${CORE_SOURCE_FILE})
add_executable(test3 ${SOURCE_FILE})
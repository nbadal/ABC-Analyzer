cmake_minimum_required(VERSION 3.17)
project(ABCAnalyzer)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

include_directories(AnalyzerSDK/include)
link_directories(AnalyzerSDK/lib)

add_library(${PROJECT_NAME} SHARED
        src/YamahaAbcAnalyzer.cpp
        src/YamahaAbcAnalyzer.h
        src/YamahaAbcAnalyzerResults.cpp
        src/YamahaAbcAnalyzerResults.h
        src/YamahaAbcAnalyzerSettings.cpp
        src/YamahaAbcAnalyzerSettings.h
        src/YamahaAbcSimulationDataGenerator.cpp
        src/YamahaAbcSimulationDataGenerator.h
        )
target_link_libraries(${PROJECT_NAME} Analyzer)

# Logic 2.x fix + install:
add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change @executable_path/libAnalyzer.dylib @rpath/libAnalyzer.dylib $<TARGET_FILE:${PROJECT_NAME}>
        COMMAND cp $<TARGET_FILE:${PROJECT_NAME}> /Users/nbadal/logic-analyzers/)
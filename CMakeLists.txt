cmake_minimum_required(VERSION 3.10)
project(Polly)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories for header files
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/libs/crow/include) # Path to Crow headers

# Source files
set(SOURCES
        ${PROJECT_SOURCE_DIR}/src/Polly.cpp
        ${PROJECT_SOURCE_DIR}/src/Display.cpp
        ${PROJECT_SOURCE_DIR}/src/LLMProcessing.cpp
        ${PROJECT_SOURCE_DIR}/src/TextToSpeech.cpp
        ${PROJECT_SOURCE_DIR}/src/VoiceCustomization.cpp
        ${PROJECT_SOURCE_DIR}/src/main.cpp
)

# Add the executable
add_executable(Polly ${SOURCES})

# Find and link Boost library
find_package(Boost REQUIRED COMPONENTS system)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(Polly ${Boost_LIBRARIES})
endif ()

# Link cURL, OpenSSL, and pthread
find_package(CURL REQUIRED)
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIR})
    target_link_libraries(Polly ${CURL_LIBRARIES})
endif ()

find_package(OpenSSL REQUIRED)
if (OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
    target_link_libraries(Polly ${OPENSSL_LIBRARIES})
endif ()

find_package(Threads REQUIRED)
target_link_libraries(Polly ${CMAKE_THREAD_LIBS_INIT})

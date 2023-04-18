cmake_minimum_required(VERSION 3.11)

include(FetchContent)

project(genetic_bioinf)
add_executable(app ${PROJECT_SOURCE_DIR}/src/main.cpp ${PROJECT_SOURCE_DIR}/src/config.cpp)

FetchContent_Declare(
    inipp
    GIT_REPOSITORY https://github.com/mcmtroffaes/inipp.git
    GIT_TAG c61e699682d3f1091209c2179f1d03f5fc593327
)
FetchContent_MakeAvailable(inipp)
target_link_libraries(app PRIVATE inipp::inipp)

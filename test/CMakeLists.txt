cmake_minimum_required(VERSION 3.20)

project(tests)

add_executable(tests main.cc)

target_link_libraries(tests assimp)
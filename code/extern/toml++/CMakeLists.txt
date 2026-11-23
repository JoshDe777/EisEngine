# file: extern/toml++/CMakeLists.txt

add_library(toml INTERFACE)
target_include_directories(toml INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(toml INTERFACE cxx_std_17)

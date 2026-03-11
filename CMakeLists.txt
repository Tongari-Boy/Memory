cmake_minimum_required(VERSION 3.20)
project(MyGame)

set(CMAKE_CXX_STANDARD 17)

# SDL2のパスを指定
set(SDL2_DIR "${CMAKE_SOURCE_DIR}/libs/SDL2")
set(SDL_TTF_DIR "${CMAKE_SOURCE_DIR}/libs/SDL_ttf")

# 追加：srcフォルダ以下の全cppを自動収集
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(MyGame ${SOURCES})

target_include_directories(MyGame PRIVATE
    ${SDL2_DIR}/include
    ${SDL_TTF_DIR}/include
    ${CMAKE_SOURCE_DIR}/src  # srcフォルダをインクルードパスに追加
)

target_link_libraries(MyGame PRIVATE
    ${SDL2_DIR}/lib/x64/SDL2.lib
    ${SDL2_DIR}/lib/x64/SDL2main.lib
    ${SDL_TTF_DIR}/lib/x64/SDL2_ttf.lib
)

# SDL2.dllをexeと同じ場所にコピー
add_custom_command(TARGET MyGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${SDL2_DIR}/lib/x64/SDL2.dll
    $<TARGET_FILE_DIR:MyGame>
)
add_custom_command(TARGET MyGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${SDL_TTF_DIR}/lib/x64/SDL2_ttf.dll
    $<TARGET_FILE_DIR:MyGame>
)

# フォントをexeと同じ場所にコピー
add_custom_command(TARGET MyGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/assets/arial.ttf
    $<TARGET_FILE_DIR:MyGame>
)
cmake_minimum_required(VERSION 3.11)

project(MyGame)

# set the source files
set(GAME_FILES game_files/player.c game_files/walls/wall.c)
set(RENDER src/engine/render/render/render_init.c src/engine/render/render/render.c src/engine/render/render/render_util.c src/engine/threading/thread_pool/thread_pool.c)
set(CAMERA src/engine/render/camera/camera.c)
set(IO src/engine/io/io.c)
set(OBJECT src/engine/object/collider/collider.c src/engine/object/collider/box_collider.c src/engine/object/renderer/renderer.c src/engine/object/renderer/box_renderer.c src/engine/object/renderer/mesh_renderer.c src/engine/object/game_object/game_object.c src/engine/object/model/model.c)
set(NETWORKING src/engine/network/server/server.c src/engine/network/client/client.c src/engine/network/room/room.c src/engine/network/network/network.c)
set(SCENE src/engine/object/map/scene.c)
set(CONFIG src/engine/common/config/config.c)
set(INPUT src/engine/input/input.c)
set(FILES deps/src/glad.c src/main.c src/engine/common/global/global.c src/engine/util/util.c ${RENDER} ${IO} ${GAME_FILES} ${SCENE} ${OBJECT} ${NETWORKING} ${CAMERA} ${CONFIG} ${INPUT})

include_directories(/Users/filiplukovic/Documents/projects/shadegin/shadegin/deps/include)

# find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# include libuv
include(ExternalProject)

set(LIBUV_SOURCE_DIR "${CMAKE_BINARY_DIR}/libuv-src")

# don't download libuv if it already exists
if(NOT EXISTS ${LIBUV_SOURCE_DIR})
    ExternalProject_Add(libuv
        GIT_REPOSITORY    https://github.com/libuv/libuv.git
        GIT_TAG           v1.x
        SOURCE_DIR        ${LIBUV_SOURCE_DIR}
        BINARY_DIR        "${CMAKE_BINARY_DIR}/libuv-build"
        CMAKE_ARGS        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/libuv-install
        INSTALL_COMMAND   ""
        TEST_COMMAND      ""
    )
else()
    ExternalProject_Add(libuv
        DOWNLOAD_COMMAND  ""
        SOURCE_DIR        ${LIBUV_SOURCE_DIR}
        BINARY_DIR        "${CMAKE_BINARY_DIR}/libuv-build"
        CMAKE_ARGS        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/libuv-install
        INSTALL_COMMAND   ""
        TEST_COMMAND      ""
    )
endif()


include_directories("${CMAKE_BINARY_DIR}/libuv-src/include")
link_directories("${CMAKE_BINARY_DIR}/libuv-build")

add_executable(MyGame ${FILES})
add_dependencies(MyGame libuv)
target_link_libraries(MyGame ${SDL2_LIBRARIES} uv)

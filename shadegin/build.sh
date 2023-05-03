# game_files="game_files/wall.c game_files/raycast.c game_files/player.c"
game_files="game_files/player.c game_files/walls/wall.c"
render="src/engine/render/render_init.c src/engine/render/render.c src/engine/render/camera.c src/engine/render/render_util.c src/engine/threading/thread_pool.c"
io="src/engine/io/io.c"
physics="src/engine/game_objects/collider/collider.c src/engine/game_objects/collider/box_collider.c src/engine/game_objects/renderer/renderer.c src/engine/game_objects/renderer/box_renderer.c src/engine/game_objects/game_object.c"
networking="src/engine/networking/server.c src/engine/networking/client.c src/engine/networking/room.c"
scene="src/engine/game_objects/map/scene.c"
files="src/glad.c src/main.c src/engine/global.c src/engine/util.c $render $io $game_files $scene $physics $networking"

# FULL PATH TO PROJECT
libs=""
# /Users/filiplukovic/Documents/projects/shadegin/lib/freetype.lib
# /Users/filiplukovic/Documents/projects/shadegin/lib/SDL2main.lib /Users/filiplukovic/Documents/projects/shadegin/lib/SDL2.lib 
gcc -Wall -g -I /Users/filiplukovic/Documents/projects/shadegin/include $files $libs -o mygame `sdl2-config --cflags --libs` $(pkg-config --cflags --libs SDL2_net)

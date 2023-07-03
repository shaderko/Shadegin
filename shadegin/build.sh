game_files="game_files/player.c game_files/walls/wall.c"

render="src/engine/render/render_init.c src/engine/render/render.c src/engine/render/render_util.c src/engine/threading/thread_pool.c"
camera="src/engine/render/camera.c"
io="src/engine/io/io.c"
physics="src/engine/game_objects/collider/collider.c src/engine/game_objects/collider/box_collider.c src/engine/game_objects/renderer/renderer.c src/engine/game_objects/renderer/box_renderer.c src/engine/game_objects/game_object.c"
networking="src/engine/networking/server.c src/engine/networking/client.c src/engine/networking/room.c"
scene="src/engine/game_objects/map/scene.c"
config="src/engine/config/config.c"
input="src/engine/input/input.c"
files="deps/src/glad.c src/main.c src/engine/global/global.c src/engine/util/util.c $render $io $game_files $scene $physics $networking $camera $config $input"

gcc -Wall -g3 -O0 -I /Users/filiplukovic/Documents/projects/shadegin/shadegin/deps/include $files -o mygame `sdl2-config --cflags --libs` $(pkg-config --cflags --libs SDL2_net)

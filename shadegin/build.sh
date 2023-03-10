game_files="game_files/wall.c game_files/raycast.c game_files/player.c"
render="src/engine/render/render_init.c src/engine/render/render.c src/engine/render/render_util.c src/engine/threading/thread_pool.c"
io="src/engine/io/io.c"
files="src/glad.c src/main.c src/engine/global.c $render $io $game_files"

# FULL PATH TO PROJECT
libs=""
# /Users/filiplukovic/Documents/projects/shadegin/lib/freetype.lib
# /Users/filiplukovic/Documents/projects/shadegin/lib/SDL2main.lib /Users/filiplukovic/Documents/projects/shadegin/lib/SDL2.lib 
gcc -Wall -g -I /Users/filiplukovic/Documents/projects/shadegin/include $files $libs -o mygame `sdl2-config --cflags --libs`

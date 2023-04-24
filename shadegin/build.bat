set game_files=game_files/player.c game_files/walls/wall.c
set render=src/engine/render/render_init.c src/engine/render/render.c src/engine/render/camera.c src/engine/render/render_util.c src/engine/threading/thread_pool.c
set physics=src/engine/game_objects/collider/collider.c src/engine/game_objects/collider/box_collider.c src/engine/game_objects/renderer/renderer.c src/engine/game_objects/renderer/box_renderer.c src/engine/game_objects/game_object.c
set io=src\engine\io\io.c
set networking=src/engine/networking/server.c src/engine/networking/client.c
set files=src\glad.c src\main.c src\engine\global.c src\engine\util.c nanosleep.c %render% %physics% %io% %game_files% %networking%
@REM FULL PATH TO PROJECT
set libs=F:\projects\shadegin\Shadegin\lib\SDL2main.lib F:\projects\shadegin\Shadegin\lib\SDL2.lib F:\projects\shadegin\Shadegin\lib\freetype.lib F:\projects\shadegin\Shadegin\lib\pthreadVC2.lib F:\projects\shadegin\Shadegin\lib\SDL2_net.lib

CL /Zi /O2 /I F:\projects\shadegin\Shadegin\include %files% /link %libs% /OUT:mygame.exe
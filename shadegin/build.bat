set game_files=game_files/wall.c game_files/raycast.c game_files/player.c
set render=src\engine\render\render_init.c src\engine\render\render.c src\engine\render\render_util.c
set io=src\engine\io\io.c
set files=src\glad.c src\main.c src\engine\global.c %render% %io% %game_files%
@REM FULL PATH TO PROJECT
set libs=F:\projects\shadegin\Shadegin\lib\SDL2main.lib F:\projects\shadegin\Shadegin\lib\SDL2.lib F:\projects\shadegin\Shadegin\lib\freetype.lib

CL /Zi /O2 /I F:\projects\shadegin\Shadegin\include %files% /link %libs% /OUT:mygame.exe
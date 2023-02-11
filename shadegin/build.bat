set files=src\glad.c src\main.c
@REM FULL PATH TO PROJECT
set libs=F:\projects\shadegin\Shadegin\lib\SDL2main.lib F:\projects\shadegin\Shadegin\lib\SDL2.lib F:\projects\shadegin\Shadegin\lib\freetype.lib

CL /Zi /I F:\projects\shadegin\Shadegin\include %files% /link %libs% /OUT:mygame.exe
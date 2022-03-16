ECHO "Script to copy the new generate files to the project"

ECHO

ECHO "First files comparaison"

WinMergeU Core/Src/ ../workspace/ASPD100/BSP/CubeMXGeneration/Src/
WinMergeU Core/Inc/ ../workspace/ASPD100/BSP/CubeMXGeneration/Inc/

ECHO

ECHO "Second files comparaison"

WinMergeU Core/Src/main.c ../workspace/ASPD100/Core/Src/main.c

ECHO

ECHO "Third files comparaison"

WinMergeU Core/Inc/main.h ../workspace/ASPD100/Core/Inc/main.h
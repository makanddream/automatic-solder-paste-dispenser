#!/bin/sh
# TODO
echo "Save a backup of necessary previous file to prevent modification"
cp ../workspace/ASPD100/BSP/CubeMXGeneration/Src/freertos.c ../workspace/ASPD100/BSP/CubeMXGeneration/Src/freertos_backup.c

echo "Copy the drivers folder generate by cubeMX"
cp -r Drivers ../workspace/ASPD100

echo "Copy the Core/Src folder generate by cubeMX"
cp -r Core/Src/ ../workspace/ASPD100/BSP/CubeMXGeneration

echo "Remove the main.c file, this file is not necessary"
rm -f ../workspace/ASPD100/BSP/CubeMXGeneration/Src/main.c

echo "Copy the Core/Inc folder generate by cubeMX"
cp -r Core/Inc/ ../workspace/ASPD100/BSP/CubeMXGeneration

echo "Remove the main.h file, this file is not necessary"
rm -f ../workspace/ASPD100/BSP/CubeMXGeneration/Inc/main.h

echo "Copy the Middlewares/Third_Party folder generate by cubeMX"
cp -r Middlewares/Third_Party/ ../workspace/ASPD100/Middlewares

sleep 4
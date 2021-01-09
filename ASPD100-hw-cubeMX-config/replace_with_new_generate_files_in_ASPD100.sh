#!/bin/sh
# TODO
echo "Save a backup of necessary previous file to prevent modification"
cp ../workspace/ASPD100/BSP/CubeMXGeneration/Src/freertos.c ../workspace/ASPD100/BSP/CubeMXGeneration/Src/freertos_backup.c
echo "Done !"

echo

echo "Copy the drivers folder generate by cubeMX"
cp -r Drivers ../workspace/ASPD100
echo "Done !"

echo

echo "Copy the Core/Src folder generate by cubeMX"
cp -r Core/Src/ ../workspace/ASPD100/BSP/CubeMXGeneration
echo "Done !"

echo

echo "Remove the main.c file, this file is not necessary"
rm -f ../workspace/ASPD100/BSP/CubeMXGeneration/Src/main.c
echo "Done !"

echo

echo "Copy the Core/Inc folder generate by cubeMX"
cp -r Core/Inc/ ../workspace/ASPD100/BSP/CubeMXGeneration
echo "Done !"

echo

echo "Remove the main.h file, this file is not necessary"
rm -f ../workspace/ASPD100/BSP/CubeMXGeneration/Inc/main.h
echo "Done !"

echo

echo "Copy the Middlewares/Third_Party folder generate by cubeMX"
cp -r Middlewares/Third_Party/ ../workspace/ASPD100/Middlewares
echo "Done !"


echo
read -p "Are you sure to copy the backup freertos file to the new freertos file? " -n 1 -r
echo    # (optional) move to a new line
if [[ $REPLY =~ ^[Yy]$ ]]
then
    cp ../workspace/ASPD100/BSP/CubeMXGeneration/Src/freertos_backup.c ../workspace/ASPD100/BSP/CubeMXGeneration/Src/freertos.c
	echo "Done !"
else
	echo "Don't forget you select "no" so the new freertos file is empty !"
fi

sleep 5
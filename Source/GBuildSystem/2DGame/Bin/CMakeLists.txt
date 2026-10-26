cmake_minimum_required(VERSION 3.10)
project(2DGame)
set(CMAKE_CXX_STANDARD 20)


add_library(2DCharacter STATIC 
/home/sprotzel/Documents/Development/GEngine/Source/GBuildSystem/2DGame/Source/2DCharacter/Private/2DCharacter.cpp 
/home/sprotzel/Documents/Development/GEngine/Source/GBuildSystem/2DGame/Source/2DCharacter/Private/3DCharacter.cpp 
)
target_link_libraries(2DCharacter

)


# GBuildSystem
The GBuildSystem is a compilation CLI tool to easily create GEngine projects
and compile them optimally.
Still in development...

## Usage:
GBuildSystem [flags] [command] [args]

flags:
    --debug or -D: enables debug flag in .GProj file

commands:
    new + Project Name - creates a new Project with source folder and .GProj project file
    module + Module Name - creates a new module with .GModule module file and Private + Public Folders
    build + Project - attempts to generate and execute a CMake file from a project, using all existing modules in the scope
    pinfo + Project - Prints Info about Project
    minfo + Module  - Prints Info about Module
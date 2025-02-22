cmake_minimum_required(VERSION 3.30)
project(Progetto_PR1 C)

set(CMAKE_C_STANDARD 99)

add_executable(Progetto_PR1 src/main.c
        src/strutture.c
        src/strutture.h
        src/file.c
        src/file.h
        src/gioco.c
        src/gioco.h
        src/colori-ansi.h
        src/debug.c
        src/debug.h
)

cmake_minimum_required(VERSION 3.30)
project(Progetto_PR1 C)

set(CMAKE_C_STANDARD 99)

add_executable(Progetto_PR1 main.c
        strutture.c
        strutture.h
        file.c
        file.h
        gioco.c
        gioco.h
        colori-ansi.h
)

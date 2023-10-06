cmake_minimum_required(VERSION 3.26)
project(UDP_ZTL_Set2023 C)

set(CMAKE_C_STANDARD 23)


add_executable(UDP_ZTL_Set2023
        server.c
        client.c
        UDP.c
        ZTL.h
        ZTL.c)

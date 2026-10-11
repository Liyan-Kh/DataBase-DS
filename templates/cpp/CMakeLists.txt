cmake_minimum_required(VERSION 3.20)
project(UniDB)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(UniDB
        main.cpp
        storage/Collection.cpp
        engine/ExecutionEngine.cpp
        parser/QueryParser.cpp
        storage/ArrayCollection.cpp
        storage/LinkedListCollection.cpp
        )

target_include_directories(UniDB PRIVATE
        .
        engine
        parser
        storage
        models
        )

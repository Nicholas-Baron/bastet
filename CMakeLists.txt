cmake_minimum_required(VERSION 3.10)

project(neobastet)

find_package(Curses)

add_executable(nbastet
    main.cpp 
    BastetBlockChooser.cpp
    BlockChooser.cpp
    Block.cpp
    BlockPosition.cpp
    Config.cpp
    Ui.cpp
    Well.cpp
    )

set_property(TARGET nbastet PROPERTY CXX_STANDARD 11)
target_link_libraries(nbastet PUBLIC ${CURSES_LIBRARIES})
target_include_directories(nbastet PUBLIC ${CURSES_INCLUDE_DIRS})
target_compile_options(nbastet PRIVATE -Wall -Wextra -O)

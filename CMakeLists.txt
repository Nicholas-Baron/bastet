cmake_minimum_required(VERSION 3.10)

project(bastet)

find_package(Curses)
find_package(Boost)

add_executable(tetris 
    main.cpp 
    BastetBlockChooser.cpp
    BlockChooser.cpp
    Block.cpp
    BlockPosition.cpp
    Config.cpp
    Ui.cpp
    Well.cpp
    )

set_property(TARGET tetris PROPERTY CXX_STANDARD 11)
target_link_libraries(tetris PUBLIC ${CURSES_LIBRARIES} boost_program_options)
target_include_directories(tetris PUBLIC ${CURSES_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
target_compile_options(tetris PRIVATE -Wall -Wextra -O)

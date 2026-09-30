cmake_minimum_required(VERSION 3.31)
project(lab8_9)

if(MSVC)
   add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror)
endif()

set(CMAKE_CXX_STANDARD 20)

add_executable(lab8_9 main.cpp
        domain/activitate.cpp
        repository/repository.cpp
        service/service.cpp
        ui/ui.cpp
        validator/validator.cpp
        test/test.cpp
)

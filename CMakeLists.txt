cmake_minimum_required(VERSION 3.19)
project(pdCalc)

set(CMAKE_CXX_STANDARD 14)

find_package(GTest)
if(GTEST_FOUND)
    set(Gtest_FOUND TRUE)
endif()
if(GTest_FOUND)
    include(GoogleTest)
endif()
enable_testing()
add_subdirectory(tests)

include_directories(src/utilities, src/backend, src/ui)

add_executable(pdCalc
        main.cpp
        src/utilities/Publisher.cpp src/utilities/Publisher.h
        src/utilities/Observer.cpp src/utilities/Observer.h
        src/utilities/Exception.h src/backend/Stack.cpp src/backend/Stack.h
        src/backend/Command.cpp src/backend/Command.h
        src/backend/CoreCommands.cpp src/backend/CoreCommands.h
        src/utilities/UserInterface.cpp src/utilities/UserInterface.h
        src/backend/CommandRepository.cpp src/backend/CommandRepository.h
        src/backend/CommandManager.cpp src/backend/CommandManager.h
        src/backend/CommandDispatcher.cpp src/backend/CommandDispatcher.h src/utilities/Tokenizer.cpp src/utilities/Tokenizer.h src/ui/cli/Cli.cpp src/ui/cli/Cli.h src/backend/AppObservers.cpp src/backend/AppObservers.h)
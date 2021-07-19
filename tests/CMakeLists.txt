include(GoogleTest)
add_executable(pdCalcTest
        StackTest.cpp
        PdCalcTest.cpp)
target_link_libraries(pdCalcTest GTest::GTest)
gtest_discover_tests(pdCalcTest)
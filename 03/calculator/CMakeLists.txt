# logic
add_library(calc_lib
    ast/BinaryExpression.cpp
    ast/IdentifierExpression.cpp
    ast/NumberExpression.cpp
    ast/DependencyVisitor.cpp
    ast/EvaluateVisitor.cpp

   context/Context.cpp

   dispatcher/CommandHandlers.cpp
   dispatcher/DispatcherMap.cpp
   dispatcher/OutputFormatter.cpp

   identifiers/DependencyGraph.cpp
   identifiers/FunctionNode.cpp
   identifiers/Variable.cpp

   parser/Parser.cpp

   Calculator.cpp
)

target_include_directories(calc_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

# exec
add_executable(calc
    main.cpp
)

target_link_libraries(calc
    PRIVATE
    calc_lib
)

# tests
enable_testing()

add_executable(calc_tests
    test/AstNode_test.cpp
    test/DependencyVisitor_test.cpp
    test/Variable_test.cpp
    test/DependencyGraph_test.cpp
    test/FunctionNode_test.cpp
    test/Context_test.cpp
)

target_link_libraries(calc_tests
    PRIVATE
    calc_lib
    Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(calc_tests)

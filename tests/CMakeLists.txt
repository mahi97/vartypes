set(TEST_LIBS
  vartypes-core
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Test
)

# --- VarVal test (primitives, no GUI) ---
add_executable(test_varval test_varval.cpp)
target_link_libraries(test_varval PRIVATE ${TEST_LIBS})
add_test(NAME test_varval COMMAND test_varval)
set_tests_properties(test_varval PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")

# --- VarTypes integration test ---
add_executable(test_vartypes test_vartypes.cpp)
target_link_libraries(test_vartypes PRIVATE ${TEST_LIBS})
add_test(NAME test_vartypes COMMAND test_vartypes)
set_tests_properties(test_vartypes PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")

# --- XML round-trip test ---
add_executable(test_xml test_xml.cpp)
target_link_libraries(test_xml PRIVATE ${TEST_LIBS})
add_test(NAME test_xml COMMAND test_xml)
set_tests_properties(test_xml PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")

# --- JSON round-trip test ---
add_executable(test_json test_json.cpp)
target_link_libraries(test_json PRIVATE ${TEST_LIBS} Qt${QT_VERSION_MAJOR}::Xml)
add_test(NAME test_json COMMAND test_json)
set_tests_properties(test_json PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")

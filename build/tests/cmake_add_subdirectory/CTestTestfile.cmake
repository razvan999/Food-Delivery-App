# CMake generated Testfile for 
# Source directory: /mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/classes/json/tests/cmake_add_subdirectory
# Build directory: /mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/build/tests/cmake_add_subdirectory
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(cmake_add_subdirectory_configure "/usr/bin/cmake" "-G" "Unix Makefiles" "-DCMAKE_CXX_COMPILER=/usr/bin/g++" "-Dnlohmann_json_source=/mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/classes/json" "/mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/classes/json/tests/cmake_add_subdirectory/project")
set_tests_properties(cmake_add_subdirectory_configure PROPERTIES  FIXTURES_SETUP "cmake_add_subdirectory" LABELS "not_reproducible" _BACKTRACE_TRIPLES "/mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/classes/json/tests/cmake_add_subdirectory/CMakeLists.txt;1;add_test;/mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/classes/json/tests/cmake_add_subdirectory/CMakeLists.txt;0;")
add_test(cmake_add_subdirectory_build "/usr/bin/cmake" "--build" ".")
set_tests_properties(cmake_add_subdirectory_build PROPERTIES  FIXTURES_REQUIRED "cmake_add_subdirectory" LABELS "not_reproducible" _BACKTRACE_TRIPLES "/mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/classes/json/tests/cmake_add_subdirectory/CMakeLists.txt;8;add_test;/mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/classes/json/tests/cmake_add_subdirectory/CMakeLists.txt;0;")

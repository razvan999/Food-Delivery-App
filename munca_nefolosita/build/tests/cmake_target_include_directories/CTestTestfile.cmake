# CMake generated Testfile for 
# Source directory: /mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/classes/json/tests/cmake_target_include_directories
# Build directory: /mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/build/tests/cmake_target_include_directories
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(cmake_target_include_directories_configure "/usr/bin/cmake" "-G" "Unix Makefiles" "-DCMAKE_CXX_COMPILER=/usr/bin/g++" "-Dnlohmann_json_source=/mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/classes/json" "/mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/classes/json/tests/cmake_target_include_directories/project")
set_tests_properties(cmake_target_include_directories_configure PROPERTIES  FIXTURES_SETUP "cmake_target_include_directories" LABELS "not_reproducible" _BACKTRACE_TRIPLES "/mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/classes/json/tests/cmake_target_include_directories/CMakeLists.txt;1;add_test;/mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/classes/json/tests/cmake_target_include_directories/CMakeLists.txt;0;")
add_test(cmake_target_include_directories_build "/usr/bin/cmake" "--build" ".")
set_tests_properties(cmake_target_include_directories_build PROPERTIES  FIXTURES_REQUIRED "cmake_target_include_directories" LABELS "not_reproducible" _BACKTRACE_TRIPLES "/mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/classes/json/tests/cmake_target_include_directories/CMakeLists.txt;8;add_test;/mnt/c/Users/razva/OneDrive/Desktop/licenta/code/Food-Delivery-App/classes/json/tests/cmake_target_include_directories/CMakeLists.txt;0;")

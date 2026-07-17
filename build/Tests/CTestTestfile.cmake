# CMake generated Testfile for 
# Source directory: /home/cellegen/Documents/GitHub/RatLab/Tests
# Build directory: /home/cellegen/Documents/GitHub/RatLab/build/Tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[ratlab_tests]=] "/home/cellegen/Documents/GitHub/RatLab/build/Tests/ratlab_tests")
set_tests_properties([=[ratlab_tests]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/cellegen/Documents/GitHub/RatLab/Tests/CMakeLists.txt;23;add_test;/home/cellegen/Documents/GitHub/RatLab/Tests/CMakeLists.txt;0;")
subdirs("../doctest")

#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "RatLab::ratlab_core_obj" for configuration ""
set_property(TARGET RatLab::ratlab_core_obj APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(RatLab::ratlab_core_obj PROPERTIES
  IMPORTED_COMMON_LANGUAGE_RUNTIME_NOCONFIG ""
  IMPORTED_OBJECTS_NOCONFIG "${_IMPORT_PREFIX}/lib/objects/ratlab_core_obj/Core/Types/type_conversion.cpp.o"
  )

list(APPEND _cmake_import_check_targets RatLab::ratlab_core_obj )
list(APPEND _cmake_import_check_files_for_RatLab::ratlab_core_obj "${_IMPORT_PREFIX}/lib/objects/ratlab_core_obj/Core/Types/type_conversion.cpp.o" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

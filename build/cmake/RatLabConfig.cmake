# =============================================================================
# RatLab — CMake Package Config
# =============================================================================
#
# This file is configured at build time and installed so that downstream
# projects can find RatLab via find_package(RatLab).


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was RatLabConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

# Include the exported targets file (created by install(EXPORT ...))
include("${CMAKE_CURRENT_LIST_DIR}/RatLabTargets.cmake")

# Convenience alias so consumers can write target_link_libraries(... RatLab::Core)
if(NOT TARGET RatLab::Core)
    add_library(RatLab::Core ALIAS ratlab_core)
endif()

check_required_components(RatLab)

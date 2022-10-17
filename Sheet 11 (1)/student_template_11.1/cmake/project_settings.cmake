# Set default build type if non was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE
      Release
      CACHE STRING "Choose the type of build." FORCE)
endif(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)

# Use ccache if available
find_program(CCACHE ccache)
if(CCACHE)
	message("Using ccache")
	set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
ENDIF(CCACHE)

# GENERATE COMPILE_COMMANDS.JSON
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add options for link time optimizations (LTO) aka interprocedural optimization
option(ENABLE_IPO
	"Enable interprocedural optimizations" OFF)
if(ENABLE_IPO)
	include(CheckIPOSupported)
	check_ipo_supported(RESULT result OUTPUT output)
	if(result)
		set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
	else()
		message(SEND_ERROR "IPO is not supported: ${output}")
	endif()
endif()

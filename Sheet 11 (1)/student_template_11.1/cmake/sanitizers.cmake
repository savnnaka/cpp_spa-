# Add new build types
message("* Adding build type AddressSanitized")
set(CMAKE_CXX_FLAGS_ADDRESSSANITIZED
    "-fsanitize=address,undefined,leak -fno-omit-frame-pointer -g -O1"
    CACHE STRING "Flags used by the C++ compiler during sanitized builds."
          FORCE)
set(CMAKE_C_FLAGS_ADDRESSSANITIZED
    "-fsanitize=address,undefined,leak -fno-omit-frame-pointer -g -O1"
    CACHE STRING "Flags used by the C compiler during sanitized builds." FORCE)
set(CMAKE_EXE_LINKER_FLAGS_ADDRESSSANITIZED
    "-fsanitize=address,undefined,leak -g"
    CACHE STRING "Flags used for linking binaries during coverage builds."
          FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_ADDRESSSANITIZED
    "-fsanitize=address,undefined,leak -g"
    CACHE STRING
          "Flags used by the shared libraries linker during sanitized builds."
          FORCE)
mark_as_advanced(
  CMAKE_CXX_FLAGS_ADDRESSSANITIZED CMAKE_C_FLAGS_ADDRESSSANITIZED
  CMAKE_EXE_LINKER_FLAGS_ADDRESSSANITIZED
  CMAKE_SHARED_LINKER_FLAGS_ADDRESSSANITIZED)

set(CMAKE_BUILD_TYPE
    "${CMAKE_BUILD_TYPE}"
    CACHE
      STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo \
MinSizeRel AddressSanitized."
      FORCE)

if(LEAPSDK_URL)
  FetchContent_Declare(
    LeapSDK
    GIT_REPOSITORY "${LEAPSDK_URL}"
    GIT_TAG        main
    GIT_PROGRESS   true
  )
  FetchContent_MakeAvailable(LeapSDK)
elseif(LEAPSDK_PATH)
  set(LeapSDK_SOURCE_DIR "${LEAPSDK_PATH}")
endif()

find_path(LEAPC_HEADERS LeapC.h
  HINTS
    "${LeapSDK_SOURCE_DIR}/include"
)
find_library(LEAPC_LIBRARY NAMES LeapC
  HINTS
    "${LeapSDK_SOURCE_DIR}/lib"
    "${LeapSDK_SOURCE_DIR}/lib/x64"
)

add_library(LeapC::Leap SHARED IMPORTED)
set_target_properties(LeapC::Leap PROPERTIES
    IMPORTED_LOCATION ${LEAPC_LIBRARY}
    IMPORTED_IMPLIB ${LEAPC_LIBRARY}
    INTERFACE_INCLUDE_DIRECTORIES "${LEAPC_HEADERS}"
)
if(WIN32)
  find_file(LEAPC_RUNTIME NAMES LeapC.dll HINTS "${LeapSDK_SOURCE_DIR}/lib/x64")
else()
  # Important: the official .dylib on mac is libLeapC.dylib but when linking against
  # it, the resulting external looks for libLeapC.5.dylib
  find_file(LEAPC_RUNTIME NAMES libLeapC.5.dylib HINTS "${LeapSDK_SOURCE_DIR}/lib/x64")
endif()

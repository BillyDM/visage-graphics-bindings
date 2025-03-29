if (APPLE)
  enable_language(OBJC)
  enable_language(OBJCXX)
endif ()

set(VISAGE_BUILD_TESTS OFF)
set(VISAGE_AMALGAMATED_BUILD ON)

include(FetchContent)
message(STATUS "Downloading Visage repository")
FetchContent_Declare(
  visage
  GIT_REPOSITORY https://github.com/VitalAudio/visage.git
  GIT_TAG 39e46bc5201431faaa9aa40307b560d2076a56b9
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(visage)

set(VISAGE_INCLUDE_PATH ${visage_SOURCE_DIR})
set(VISAGE_INCLUDE ${visage_BINARY_DIR}/include)

include(${VISAGE_INCLUDE_PATH}/cmake/compile_flags.cmake)
include(${VISAGE_INCLUDE_PATH}/cmake/testing_framework.cmake)

function(amalgamate_headers AMALGAMATED_HEADER_NAME FILE_HEADERS)
  set(FILE_CONTENTS "// Auto generated amalgamated header.\n")
  foreach (FILE ${FILE_HEADERS})
    set(FILE_CONTENTS "${FILE_CONTENTS}#include \"${FILE}\"\n")
  endforeach ()
  file(CONFIGURE OUTPUT "${AMALGAMATED_HEADER_NAME}" CONTENT "${FILE_CONTENTS}")
endfunction()

function(amalgamate_sources AMALGAMATED_SOURCE FILE_SOURCES)
  set(FILE_CONTENTS "// Auto generated amalgamated source.\n")
  foreach (FILE ${FILE_SOURCES})
    set_source_files_properties("${FILE}" PROPERTIES HEADER_FILE_ONLY TRUE)
    set(FILE_CONTENTS "${FILE_CONTENTS}#include \"${FILE}\"\n")
  endforeach ()
  file(CONFIGURE OUTPUT "${AMALGAMATED_SOURCE}" CONTENT "${FILE_CONTENTS}")
endfunction()

add_library(VisageGraphicsC STATIC visage_graphics_c.cpp)

target_include_directories(VisageGraphicsC SYSTEM
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE
    ${VISAGE_INCLUDE_PATH}
    ${VISAGE_INCLUDE}
    ${visage_SOURCE_DIR}/visage_file_embed
    ${visage_BINARY_DIR}/visage_graphics/VisageEmbeddedFonts_generated
  )
target_link_libraries(VisageGraphicsC PRIVATE
  VisageGraphics
  VisageUtils
)

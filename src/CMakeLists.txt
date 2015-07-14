set (SFML_INCLUDE "${PROJECT_SOURCE_DIR}/lib/sfml/include")
set (SFML_LIBS
    sfml-network
    sfml-audio
    sfml-graphics
    sfml-window
    sfml-system
)

set (NEX_INCLUDE "${PROJECT_SOURCE_DIR}/lib/nex/include")
set (NEX_LIBS
    nex-math
    nex-gfx
)

include_directories (cell-common ${NEX_INCLUDE} ${SFML_INCLUDE})

add_subdirectory (cell-common)
add_subdirectory (cell-simulation)

set (SFML_INCLUDE "${PROJECT_SOURCE_DIR}/lib/sfml/include")
set (SFML_LIBS
    sfml-network
    sfml-audio
    sfml-graphics
    sfml-window
    sfml-system
)

set (SCL_LIBS
    scl
    scl_math
)

include_directories (cell-common ${SCL_INC_DIR} ${SFML_INCLUDE})

add_subdirectory (cell-common)
add_subdirectory (cell-simulation)

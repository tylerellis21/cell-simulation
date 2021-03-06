project (cell-simulation)

include_directories (${SFML_INCLUDE} ${NEX_INCLUDE})

set (EXE_NAME cell-simulation)

set (SRCS
    main.cpp
    core/config.h
    core/config.cpp
    core/console.h
    core/console.cpp
    core/client.h
    core/client.cpp
    core/engine.h
    core/engine.cpp
    core/camera.h
    core/camera.cpp
    core/content.h
    core/content.cpp
    simulation/neuralnetwork.h
    simulation/neuralnetwork.cpp
    simulation/randomgen.h
    simulation/randomgen.cpp
    simulation/entity.h
    simulation/entity.cpp
    simulation/cell.h
    simulation/cell.cpp
    simulation/food.h
    simulation/food.cpp
    simulation/fire.h
    simulation/fire.cpp
    simulation/resource.h
    simulation/resource.cpp
    simulation/world.h
    simulation/world.cpp
    simulation/genetics/dna.h
    simulation/genetics/dna.cpp
    simulation/genetics/genome.h
    simulation/genetics/genome.cpp
    simulation/genetics/traits.h
    simulation/genetics/traits.cpp
    simulation/genetics/breeder.h
    simulation/genetics/breeder.cpp
    simulation/partitioning/spatialhash.h
    simulation/partitioning/spatialhash.cpp
    simulation/partitioning/hashnode.h
    simulation/partitioning/hashnode.cpp
    simulation/partitioning/hashutils.h
    simulation/partitioning/hashutils.cpp
    mathutils.h
    mathutils.cpp
)

include_directories (${SCL_INC_DIR})
add_executable (${EXE_NAME} ${SRCS})
target_link_libraries (${EXE_NAME} ${SFML_LIBS} ${SCL_LIBS} cell-common)

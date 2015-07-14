project (cell-simulation)

include_directories (${SFML_INCLUDE} ${NEX_INCLUDE})

set (EXE_NAME cell-simulation)

set (SRCS

    main.cpp

    typedefs.h

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

    simulation/entity.h
    simulation/entity.cpp

    simulation/cell.h
    simulation/cell.cpp

    simulation/food.h
    simulation/food.cpp

    simulation/resource.h
    simulation/resource.cpp

    simulation/world.h
    simulation/world.cpp

    simulation/genetics/genome.h
    simulation/genetics/genome.cpp

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

    irc/ircbot.h
    irc/ircbot.cpp

    irc/ircuser.h
    irc/ircuser.cpp

    irc/message/message.h
    irc/message/message.cpp

    irc/message/inmessage.h
    irc/message/inmessage.cpp

    irc/message/outmessage.h
    irc/message/outmessage.cpp
)

add_executable (${EXE_NAME} ${SRCS})
target_link_libraries (${EXE_NAME} ${SFML_LIBS} ${NEX_LIBS} cell-common)

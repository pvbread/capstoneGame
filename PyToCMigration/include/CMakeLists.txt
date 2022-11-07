set(HEADER_FILES
	./Characters/BaseCharacter.h
    ./Characters/Players/BasePlayer.h
    ./Characters/Players/Bass.h
    ./pch.h
    ./setRoundTurns.h
)

set(SOURCE_FILES
    ../src/test.cpp
    ../src/Characters/BaseCharacter.cpp
    ../src/Characters/Players/BasePlayer.cpp
    ../src/Characters/Players/Bass.cpp
    ../src/Utility/isTeamAlive.cpp
)

add_library(pythonMigration ${HEADER_FILES} ${SOURCE_FILES})

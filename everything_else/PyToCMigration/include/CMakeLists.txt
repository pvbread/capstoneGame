set(HEADER_FILES
	./Characters/BaseCharacter.h
    ./Characters/Players/BasePlayer.h
    ./Characters/Players/Bass.h
    ./pch.h
    ./Utility/setRoundTurns.h
    ./Utility/isTeamAlive.h
)

set(SOURCE_FILES
    ../src/test.cpp
    ../src/Characters/BaseCharacter.cpp
    ../src/Characters/Players/BasePlayer.cpp
    ../src/Characters/Players/Bass.cpp    
)

add_library(pythonMigration ${HEADER_FILES} ${SOURCE_FILES})

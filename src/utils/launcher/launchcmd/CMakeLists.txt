
set( project launchcmd )


set(BASE_SOURCES
${FIRST_GCC_PROGRAM_SOURCE} 
    launchcmd
${LAST_GCC_PROGRAM_SOURCE}
)

add_program(${project} ${BASE_SOURCES} )
SET_TARGET_PROPERTIES(${project} PROPERTIES
                  FOLDER utils/launcher
)

if( WIN32 )
set( project launchwin )

set(BASE_SOURCES
${FIRST_GCC_PROGRAM_SOURCE}
    launchcmd
${LAST_GCC_PROGRAM_SOURCE}
)

add_program(${project} WIN32 ${BASE_SOURCES} )
SET_TARGET_PROPERTIES(${project} PROPERTIES
                  FOLDER utils/launcher
)

endif( WIN32 )

add_subdirectory( portable )

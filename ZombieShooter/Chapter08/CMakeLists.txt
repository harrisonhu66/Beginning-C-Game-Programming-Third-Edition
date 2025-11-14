set(exe ZombieShooter08)
add_executable(${exe})

target_sources(${exe}
    PRIVATE
        ZombieArena.cpp
        Player.cpp

    PRIVATE # Make headers visible to IDE
        FILE_SET HEADERS
        FILES
            Player.h
)

target_compile_features(${exe} PRIVATE cxx_std_23)
target_link_libraries(${exe} 
    PRIVATE 
        sfml-graphics
        sfml-window
        sfml-system
        sfml-network
        sfml-audio
)

if(WIN32)
    add_custom_command(
        TARGET ${exe}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${exe}>
        VERBATIM
    )
    add_custom_command(TARGET ${exe} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:sfml-graphics>
            $<TARGET_FILE:sfml-window>
            $<TARGET_FILE:sfml-system>
            $<TARGET_FILE:sfml-network>
            $<TARGET_FILE:sfml-audio>
            $<TARGET_FILE_DIR:${exe}>
    )
endif()

set(RESOURCE_TYPES fonts graphics sound)

foreach(RES_TYPE IN LISTS RESOURCE_TYPES)
    set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${RES_TYPE}")
    set(DST_DIR "${CMAKE_CURRENT_BINARY_DIR}/${RES_TYPE}")

    if(EXISTS ${SRC_DIR})
        add_custom_command(
            TARGET ${exe} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${DST_DIR}
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${SRC_DIR} ${DST_DIR}
            COMMENT "Copying ${RES_TYPE} to build directory"
        )
    endif()
endforeach()
add_executable(TimerChapter01)
# 这一行不知道为啥没用, 每次都要手动设置startup很烦呐
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT TimerChapter01)

target_sources(TimerChapter01
    PRIVATE
        Timber.cpp
)

target_compile_features(TimerChapter01 PRIVATE cxx_std_20)
target_link_libraries(TimerChapter01 
    PRIVATE 
        sfml-graphics
        sfml-system
        sfml-window
        sfml-audio
)

if(WIN32)
    add_custom_command(
        TARGET TimerChapter01
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:TimerChapter01>
        VERBATIM
    )

    # set(SFML_DLL_DIR "${sfml_BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}") 
    # set(SFML_DLLS
    #     "${SFML_DLL_DIR}/sfml-graphics-d-2.dll"
    #     "${SFML_DLL_DIR}/sfml-window-d-2.dll"
    #     "${SFML_DLL_DIR}/sfml-system-d-2.dll"
    # )
    # add_custom_command(TARGET TimerChapter01 POST_BUILD
    #     COMMAND ${CMAKE_COMMAND} -E copy_if_different
    #         ${SFML_DLLS}
    #         $<TARGET_FILE_DIR:TimerChapter01>
    #     COMMENT "Copying SFML DLLs to executable directory"
    # )
    add_custom_command(TARGET TimerChapter01 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:sfml-graphics>
            $<TARGET_FILE:sfml-window>
            $<TARGET_FILE:sfml-system>
            $<TARGET_FILE:sfml-audio>
            $<TARGET_FILE_DIR:TimerChapter01>
    )
endif()

set(RESOURCE_TYPES fonts graphics sound)

foreach(RES_TYPE IN LISTS RESOURCE_TYPES)
    set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${RES_TYPE}")
    set(DST_DIR "${CMAKE_CURRENT_BINARY_DIR}/${RES_TYPE}")

    if(EXISTS ${SRC_DIR})
        add_custom_command(
            TARGET TimerChapter01 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${DST_DIR}
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${SRC_DIR} ${DST_DIR}
            COMMENT "Copying ${RES_TYPE} to build directory"
        )
    endif()
endforeach()
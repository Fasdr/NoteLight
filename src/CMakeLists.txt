add_subdirectory(gui)

add_executable(NoteLight)

target_sources(NoteLight
    PRIVATE
        main.cpp
)

target_link_libraries(NoteLight
    PRIVATE
        guiFunctional
)
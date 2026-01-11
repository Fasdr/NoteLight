add_subdirectory(GUI)

add_executable(NoteLight)

target_sources(NoteLight
    PRIVATE
        main.cpp
)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

qt_add_library(Launch)

target_sources(Launch
    PRIVATE
        launch.cpp
    PUBLIC
        FILE_SET HEADERS
        FILES
            launch.h
)

target_link_libraries(Launch
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

target_link_libraries(Launch
    PRIVATE
        MainWindow
)

target_link_libraries(NoteLight
    PRIVATE
        Launch
)
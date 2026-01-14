find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

qt_add_library(InputArea)

target_sources(InputArea
    PRIVATE
        inputArea.cpp
        inputAreaVisuals.cpp
    PUBLIC
        FILE_SET HEADERS
        FILES
            inputArea.h
)

target_link_libraries(InputArea
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

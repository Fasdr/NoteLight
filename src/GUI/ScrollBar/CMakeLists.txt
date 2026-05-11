find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

qt_add_library(ScrollBar)

target_sources(ScrollBar
    PRIVATE
        scrollBar.cpp
        scrollBarVisuals.cpp
    PUBLIC
        FILE_SET HEADERS
        FILES
            scrollBar.h
)

target_link_libraries(ScrollBar
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

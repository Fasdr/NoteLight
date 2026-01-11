find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

qt_add_library(TitleBar)

target_sources(TitleBar
    PRIVATE
        titleBar.cpp
    PUBLIC
        FILE_SET HEADERS
        FILES
            titleBar.h
)

target_link_libraries(TitleBar
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

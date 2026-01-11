add_subdirectory(TitleBar)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

qt_add_library(MainWindow)

target_sources(MainWindow
    PRIVATE
        mainWindow.cpp
    PUBLIC
        FILE_SET HEADERS
        FILES
            mainWindow.h
)

target_link_libraries(MainWindow
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

target_link_libraries(MainWindow
    PUBLIC
        TitleBar
)
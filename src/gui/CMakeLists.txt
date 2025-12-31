find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Quick VirtualKeyboard)

qt_standard_project_setup()

add_library(guiFunctional)

target_sources(guiFunctional
    PRIVATE
        startApp.cpp
    PUBLIC
        FILE_SET HEADERS
        FILES
            guiFunctional.h
)

qt_add_library(guiInternal
    guiInternal.h
    mainWindow.cpp
    writingArea.cpp
    drawingToolsMenu.cpp
    zoomControl.cpp
    simpleColorDialog.cpp
    penWidthSliderDialog.cpp
    positionControl.cpp
)

target_sources(guiInternal
    PUBLIC
        FILE_SET HEADERS
        FILES
            guiInternal.h
)

# add_library(guiInternal)

# target_sources(guiInternal
#     PRIVATE
#         mainWindow.cpp
#     PUBLIC
#         FILE_SET HEADERS
#         FILES
#             guiInternal.h
# )

target_link_libraries(guiFunctional
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Quick
        Qt6::VirtualKeyboard
)

target_link_libraries(guiInternal
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

target_link_libraries(guiFunctional
    PRIVATE
        guiInternal
)
set(COLORCHANGER_SOURCES
    ColorChangerComponent.cpp
    ColorChangerWidget.cpp
)

add_library(colorchanger SHARED ${COLORCHANGER_SOURCES})
target_link_libraries(colorchanger PRIVATE Qt6::Core Qt6::Widgets)

# For macOS, we need to set the bundle properties
if(APPLE)
    set_target_properties(colorchanger PROPERTIES
        BUNDLE TRUE
        FRAMEWORK FALSE
        PREFIX ""
        SUFFIX ".dylib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
    )
else()
    set_target_properties(colorchanger PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
        PREFIX "lib"
    )
endif() 
set(COUNTER_SOURCES
    CounterComponent.cpp
    CounterWidget.cpp
)

add_library(counter SHARED ${COUNTER_SOURCES})
target_link_libraries(counter PRIVATE Qt6::Core Qt6::Widgets)

# For macOS, we need to set the bundle properties
if(APPLE)
    set_target_properties(counter PROPERTIES
        BUNDLE TRUE
        FRAMEWORK FALSE
        PREFIX ""
        SUFFIX ".dylib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
    )
else()
    set_target_properties(counter PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
        PREFIX "lib"
    )
endif() 
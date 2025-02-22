set(MAINAPP_SOURCES
    main.cpp
    MainWindow.cpp
)

add_executable(mainapp ${MAINAPP_SOURCES})
target_link_libraries(mainapp 
    PRIVATE 
        Qt6::Core 
        Qt6::Widgets
        component-interfaces
)

# Add the interfaces directory to the include path
target_include_directories(mainapp PRIVATE ${CMAKE_SOURCE_DIR}) 
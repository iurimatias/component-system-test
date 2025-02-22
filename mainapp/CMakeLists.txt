set(MAINAPP_SOURCES
    main.cpp
    MainWindow.cpp
)

add_executable(mainapp ${MAINAPP_SOURCES})
target_link_libraries(mainapp PRIVATE Qt6::Core Qt6::Widgets) 
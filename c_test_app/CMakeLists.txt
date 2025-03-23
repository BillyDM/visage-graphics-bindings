add_executable(App main.c)
target_link_libraries(App PRIVATE VisageGraphicsC)

set_target_properties(App PROPERTIES MACOSX_BUNDLE TRUE)
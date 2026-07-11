find_package(Doxygen REQUIRED)

if (DOXYGEN_FOUND)
    set(DOC_SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${PROJECT_SOURCE_DIR}/README.md
            ${PROJECT_SOURCE_DIR}/diagrams
    )
    find_program(DOT_EXECUTABLE NAMES dot)
    if(DOT_EXECUTABLE)
        message(STATUS "Graphviz found: ${DOT_EXECUTABLE}")
        set(DOXYGEN_HAVE_DOT YES)
        set(DOXYGEN_DOT_PATH "${DOT_EXECUTABLE}") # Tells Doxygen exactly where 'dot' is
    else()
        message(WARNING "Graphviz (dot) not found! Diagrams will be disabled.")
        set(DOXYGEN_HAVE_DOT NO)
    endif()

    set(DOXYGEN_PROJECT_NAME "${PROJECT_NAME}")
    set(DOXYGEN_PROJECT_BRIEF "Developed by Luc Paul Labonté")
    set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/docs")
    set(DOXYGEN_RECURSIVE YES)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_LATEX NO)
    set(DOXYGEN_GENERATE_MAN YES)
    set(DOXYGEN_HAVE_DOT YES)

    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${PROJECT_SOURCE_DIR}/README.md")
    set(DOXYGEN_PLANTUML_JAR_PATH "${PROJECT_SOURCE_DIR}/dependencies/plantuml.jar")
    doxygen_add_docs(docs ${DOC_SOURCES} COMMENT "Generating API documentation")
endif ()

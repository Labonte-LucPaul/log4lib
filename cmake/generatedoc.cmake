find_package(Doxygen REQUIRED)
find_program(PDFLATEX_PROGRAM NAMES pdflatex)

if (DOXYGEN_FOUND)
    set(DOC_SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${PROJECT_SOURCE_DIR}/README.md
    )
    set(DOXYGEN_IMAGE_PATH ${PROJECT_SOURCE_DIR}/diagrams)

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
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/footer.tex.in
            ${CMAKE_CURRENT_BINARY_DIR}/docs/latex/footer.tex
            @ONLY)
    set(DOXYGEN_DOCSET_FEEDNAME "Luc")
    set(DOXYGEN_LATEX_FOOTER "${CMAKE_CURRENT_BINARY_DIR}/docs/latex/footer.tex")
    set(DOXYGEN_PROJECT_BRIEF "Provides a log mechanism to your library.")
    set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/docs")
    set(DOXYGEN_RECURSIVE YES)
    set(DOXYGEN_HAVE_DOT YES)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_MAN YES)
    # Single man page
    set(DOXYGEN_MAN_EXTENSION ".3")
    set(DOXYGEN_MAN_LINKS NO)
    set(DOXYGEN_HIDE_UNDOC_MEMBERS YES)
    set(DOXYGEN_SHOW_USED_FILES        NO)
    set(DOXYGEN_SHOW_FILES             NO)
    set(DOXYGEN_ALPHA_INDEX            NO)
    set(DOXYGEN_ROUTINE_SANITY         YES)

    set(DOXYGEN_GENERATE_LATEX YES) # generate PDF
    set(DOXYGEN_USE_PDFLATEX YES) # Optimizes output for pdflatex
    set(DOXYGEN_PDF_HYPERLINKS YES) # Clickable links inside the PDF

    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${PROJECT_SOURCE_DIR}/README.md")
    set(DOXYGEN_PLANTUML_JAR_PATH "${PROJECT_SOURCE_DIR}/dependencies/plantuml.jar")
    doxygen_add_docs(docs ${DOC_SOURCES} COMMENT "Generating API documentation")

    if(PDFLATEX_PROGRAM)
        add_custom_command(TARGET docs POST_BUILD
            COMMAND ${PDFLATEX_PROGRAM} refman.tex
            COMMAND ${PDFLATEX_PROGRAM} refman.tex
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/docs/latex"
            COMMENT "Compile refman.tex to refman.pdf..."
            VERBATIM
        )
    else ()
        message(WARNING "Could ont find '${PDFLATEX_PROGRAM}', not generating PDF doc.")
    endif ()
endif ()

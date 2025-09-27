find_package(Doxygen REQUIRED)

include(FetchContent)
FetchContent_Declare(doxygen-awesome-css
  GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css.git
  GIT_TAG v2.3.1
)
FetchContent_MakeAvailable(doxygen-awesome-css)

function(Doxygen target input)
  set(NAME "doxygen-${target}")
  set(DOXYGEN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/docs/${target})
  set(DOXYFILE_CONFIG ${CMAKE_BINARY_DIR}/${NAME}.Doxyfile)

  # Génère les options Doxygen à injecter dans le Doxyfile.in
  UseDoxygenAwesomeCss()
  UseDoxygenAwesomeExtensions()

  # Prépare les chemins à injecter
  set(DOX_INPUTS "${PROJECT_SOURCE_DIR}/${input}")
  set(EXTRA_STYLESHEET "${DOXYGEN_HTML_EXTRA_STYLESHEET}")
  string(REPLACE ";" " " EXTRA_FILES "${DOXYGEN_HTML_EXTRA_FILES}")
  set(HTML_HEADER "${DOXYGEN_HTML_HEADER}")

  # Configure le Doxyfile
  configure_file(${CMAKE_SOURCE_DIR}/cmake/Doxyfile.in ${DOXYFILE_CONFIG} @ONLY)

  # Déclare la cible
  add_custom_target(${NAME}
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_CONFIG}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Génération de la documentation Doxygen pour ${target}"
    VERBATIM
  )
endfunction()

macro(UseDoxygenAwesomeCss)
  set(DOXYGEN_GENERATE_TREEVIEW YES)
  set(DOXYGEN_HAVE_DOT YES)
  set(DOXYGEN_DOT_IMAGE_FORMAT svg)
  set(DOXYGEN_DOT_TRANSPARENT YES)
  set(DOXYGEN_HTML_EXTRA_STYLESHEET
    ${doxygen-awesome-css_SOURCE_DIR}/doxygen-awesome.css)
endmacro()

macro(UseDoxygenAwesomeExtensions)
  set(DOXYGEN_HTML_EXTRA_FILES
    ${doxygen-awesome-css_SOURCE_DIR}/doxygen-awesome-darkmode-toggle.js
    ${doxygen-awesome-css_SOURCE_DIR}/doxygen-awesome-fragment-copy-button.js
    ${doxygen-awesome-css_SOURCE_DIR}/doxygen-awesome-paragraph-link.js
    ${doxygen-awesome-css_SOURCE_DIR}/doxygen-awesome-interactive-toc.js
  )

  execute_process(COMMAND ${DOXYGEN_EXECUTABLE} -w html header.html footer.html style.css
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
  execute_process(COMMAND sed -i "/<\\/head>/r ${PROJECT_SOURCE_DIR}/cmake/doxygen_extra_headers" header.html
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
  set(DOXYGEN_HTML_HEADER ${CMAKE_BINARY_DIR}/header.html)
endmacro()

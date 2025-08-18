# Definiert den absoluten Pfad zum FreeType-Stammverzeichnis
# CMAKE_CURRENT_SOURCE_DIR ist das Verzeichnis, in dem sich die Include.cmake befindet,
# also "...\Depencies\FreeType". Der Pfad muss somit nur "." sein.
get_filename_component(FREETYPE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)
message(STATUS "FreeType-Verzeichnis: ${FREETYPE_ROOT}")

# Inkludiert die Header-Dateien, relativ zum FreeType-Stammverzeichnis
include_directories(${FREETYPE_ROOT}/include)

# Sucht nach der FreeType-Bibliothek, relativ zum FreeType-Stammverzeichnis
find_library(FREETYPE_LIBRARY
    NAMES freetype ft2
    HINTS ${FREETYPE_ROOT}/lib
)

if(FREETYPE_LIBRARY)
    message(STATUS "Gefundene FreeType-Bibliothek: ${FREETYPE_LIBRARY}")
else()
    message(FATAL_ERROR "FreeType-Bibliothek wurde nicht gefunden. Stellen Sie sicher, dass sie unter ${FREETYPE_ROOT}/lib existiert.")
endif()
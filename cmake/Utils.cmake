# Some utilities
#TODO: comment/document

function(directory_source_group ARG0 ARG1 ARG2)
    file(GLOB files "${ARG1}/${ARG2}")
    source_group(${ARG0} FILES ${files})
    set(dirs)
    file(GLOB glob "${ARG1}/*")
    foreach(match ${glob})
        if(IS_DIRECTORY ${match})
            list(APPEND dirs ${match})
        endif()
    endforeach()
    foreach(dir ${dirs})
        string(REGEX MATCH "([^/]*)$" filter ${dir})
        directory_source_group(${ARG0}\\${filter} ${dir} ${ARG2})
    endforeach()
endfunction()

macro(standard_paths ARG0 ARG1 ARG2)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ARG0}/${ARG1})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ARG0}/${ARG2})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ARG0}/${ARG2})
    foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
    endforeach()
    set(CMAKE_DEBUG_POSTFIX d)
    set(CMAKE_MINSIZEREL_POSTFIX min)
endmacro()

macro(obc_add_module NAME)
    set(SOURCES "")

    foreach(X ${ARGN})
        set(SOURCES "${SOURCES};${CMAKE_CURRENT_LIST_DIR}/src/${X}")
    endforeach()

    set(MODULE_NAME module_${NAME})

    add_library(${MODULE_NAME} INTERFACE)

    target_sources(${MODULE_NAME} INTERFACE
        ${SOURCES}
    )

    target_include_directories(${MODULE_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
endmacro()

macro(obc_add_submodule NAME MODULE)
    set(SOURCES "")

    foreach(X ${ARGN})
        set(SOURCES "${SOURCES};${CMAKE_CURRENT_LIST_DIR}/src/${X}")
    endforeach()

    set(MODULE_NAME module_${MODULE}_${NAME})

    add_library(${MODULE_NAME} INTERFACE)

    target_sources(${MODULE_NAME} INTERFACE
        ${SOURCES}
    )

    target_include_directories(${MODULE_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
endmacro()

macro(obc_add_module_header_only NAME)
    set(MODULE_NAME module_${NAME})

    add_library(${MODULE_NAME} INTERFACE)
    target_include_directories(${MODULE_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
endmacro()
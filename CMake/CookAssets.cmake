
function(cook_assets TARGET_NAME SRC DST)


    add_custom_target(${TARGET_NAME} ALL
        COMMENT "Transfering Editor content to Project"
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<CONFIG>/Content"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${SRC}" "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Content"
    )
endfunction()
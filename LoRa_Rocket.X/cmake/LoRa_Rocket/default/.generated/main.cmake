include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(LoRa_Rocket_default_library_list )

# Handle files with suffix s, for group default-XC16
if(LoRa_Rocket_default_default_XC16_FILE_TYPE_assemble)
add_library(LoRa_Rocket_default_default_XC16_assemble OBJECT ${LoRa_Rocket_default_default_XC16_FILE_TYPE_assemble})
    LoRa_Rocket_default_default_XC16_assemble_rule(LoRa_Rocket_default_default_XC16_assemble)
    list(APPEND LoRa_Rocket_default_library_list "$<TARGET_OBJECTS:LoRa_Rocket_default_default_XC16_assemble>")

endif()

# Handle files with suffix S, for group default-XC16
if(LoRa_Rocket_default_default_XC16_FILE_TYPE_assemblePreproc)
add_library(LoRa_Rocket_default_default_XC16_assemblePreproc OBJECT ${LoRa_Rocket_default_default_XC16_FILE_TYPE_assemblePreproc})
    LoRa_Rocket_default_default_XC16_assemblePreproc_rule(LoRa_Rocket_default_default_XC16_assemblePreproc)
    list(APPEND LoRa_Rocket_default_library_list "$<TARGET_OBJECTS:LoRa_Rocket_default_default_XC16_assemblePreproc>")

endif()

# Handle files with suffix c, for group default-XC16
if(LoRa_Rocket_default_default_XC16_FILE_TYPE_compile)
add_library(LoRa_Rocket_default_default_XC16_compile OBJECT ${LoRa_Rocket_default_default_XC16_FILE_TYPE_compile})
    LoRa_Rocket_default_default_XC16_compile_rule(LoRa_Rocket_default_default_XC16_compile)
    list(APPEND LoRa_Rocket_default_library_list "$<TARGET_OBJECTS:LoRa_Rocket_default_default_XC16_compile>")

endif()

# Handle files with suffix s, for group default-XC16
if(LoRa_Rocket_default_default_XC16_FILE_TYPE_dependentObject)
add_library(LoRa_Rocket_default_default_XC16_dependentObject OBJECT ${LoRa_Rocket_default_default_XC16_FILE_TYPE_dependentObject})
    LoRa_Rocket_default_default_XC16_dependentObject_rule(LoRa_Rocket_default_default_XC16_dependentObject)
    list(APPEND LoRa_Rocket_default_library_list "$<TARGET_OBJECTS:LoRa_Rocket_default_default_XC16_dependentObject>")

endif()

# Handle files with suffix elf, for group default-XC16
if(LoRa_Rocket_default_default_XC16_FILE_TYPE_bin2hex)
add_library(LoRa_Rocket_default_default_XC16_bin2hex OBJECT ${LoRa_Rocket_default_default_XC16_FILE_TYPE_bin2hex})
    LoRa_Rocket_default_default_XC16_bin2hex_rule(LoRa_Rocket_default_default_XC16_bin2hex)
    list(APPEND LoRa_Rocket_default_library_list "$<TARGET_OBJECTS:LoRa_Rocket_default_default_XC16_bin2hex>")

endif()


# Main target for this project
add_executable(LoRa_Rocket_default_image_7dTD83jU ${LoRa_Rocket_default_library_list})

set_target_properties(LoRa_Rocket_default_image_7dTD83jU PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf"
    RUNTIME_OUTPUT_DIRECTORY "${LoRa_Rocket_default_output_dir}")
target_link_libraries(LoRa_Rocket_default_image_7dTD83jU PRIVATE ${LoRa_Rocket_default_default_XC16_FILE_TYPE_link})

# Add the link options from the rule file.
LoRa_Rocket_default_link_rule( LoRa_Rocket_default_image_7dTD83jU)

# Call bin2hex function from the rule file
LoRa_Rocket_default_bin2hex_rule(LoRa_Rocket_default_image_7dTD83jU)


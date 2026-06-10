set(DEPENDENT_MP_BIN2HEXLoRa_Rocket_default_7dTD83jU "/opt/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFLoRa_Rocket_default_7dTD83jU "${CMAKE_CURRENT_LIST_DIR}/../../../../out/LoRa_Rocket/default.elf")
set(DEPENDENT_TARGET_DIRLoRa_Rocket_default_7dTD83jU "${CMAKE_CURRENT_LIST_DIR}/../../../../out/LoRa_Rocket")
set(DEPENDENT_BYPRODUCTSLoRa_Rocket_default_7dTD83jU ${DEPENDENT_TARGET_DIRLoRa_Rocket_default_7dTD83jU}/${sourceFileNameLoRa_Rocket_default_7dTD83jU}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRLoRa_Rocket_default_7dTD83jU}/${sourceFileNameLoRa_Rocket_default_7dTD83jU}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXLoRa_Rocket_default_7dTD83jU} ${DEPENDENT_DEPENDENT_TARGET_ELFLoRa_Rocket_default_7dTD83jU} --image ${sourceFileNameLoRa_Rocket_default_7dTD83jU} ${addressLoRa_Rocket_default_7dTD83jU} ${modeLoRa_Rocket_default_7dTD83jU} -mdfp=/home/mizkyosia/.mchp_packs/Microchip/dsPIC33F-GP-MC_DFP/1.4.235/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRLoRa_Rocket_default_7dTD83jU}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFLoRa_Rocket_default_7dTD83jU})
add_custom_target(
    dependent_produced_source_artifactLoRa_Rocket_default_7dTD83jU 
    DEPENDS ${DEPENDENT_TARGET_DIRLoRa_Rocket_default_7dTD83jU}/${sourceFileNameLoRa_Rocket_default_7dTD83jU}.s
    )

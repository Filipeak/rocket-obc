obc_add_submodule(lora drivers e19_433m20sc_driver.c sx126X_driver.c sx127X_driver.c)

target_link_libraries(${MODULE_NAME} INTERFACE module_drivers_hal)
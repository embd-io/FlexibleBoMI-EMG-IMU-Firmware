################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
AFE_ADS1291.obj: ../AFE_ADS1291.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib/MSP430F5xx_6xx" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib/MSP430F5xx_6xx/deprecated" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/include" --advice:power=all --define=__MSP430F5528__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="AFE_ADS1291.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

CommUART.obj: ../CommUART.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib/MSP430F5xx_6xx" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib/MSP430F5xx_6xx/deprecated" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/include" --advice:power=all --define=__MSP430F5528__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="CommUART.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Config.obj: ../Config.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib/MSP430F5xx_6xx" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib/MSP430F5xx_6xx/deprecated" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/include" --advice:power=all --define=__MSP430F5528__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="Config.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Service.obj: ../Service.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib/MSP430F5xx_6xx" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib/MSP430F5xx_6xx/deprecated" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/include" --advice:power=all --define=__MSP430F5528__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="Service.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

capteur_intertiel.obj: ../capteur_intertiel.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib/MSP430F5xx_6xx" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib/MSP430F5xx_6xx/deprecated" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/include" --advice:power=all --define=__MSP430F5528__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="capteur_intertiel.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib/MSP430F5xx_6xx" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib/MSP430F5xx_6xx/deprecated" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/include" --advice:power=all --define=__MSP430F5528__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="main.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

nRF.obj: ../nRF.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib/MSP430F5xx_6xx" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib/MSP430F5xx_6xx/deprecated" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/include" --advice:power=all --define=__MSP430F5528__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="nRF.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

nRF_config.obj: ../nRF_config.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib/MSP430F5xx_6xx" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib/MSP430F5xx_6xx/deprecated" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/include" --advice:power=all --define=__MSP430F5528__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="nRF_config.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

process_IMU.obj: ../process_IMU.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib/MSP430F5xx_6xx" --include_path="C:/Users/Etudiant/Dropbox/Code_MSP430/Up_to_date_Network/NETWORK_ACK/EMG_Sensor_CH1_PTX/driverlib/MSP430F5xx_6xx/deprecated" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/include" --advice:power=all --define=__MSP430F5528__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="process_IMU.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '



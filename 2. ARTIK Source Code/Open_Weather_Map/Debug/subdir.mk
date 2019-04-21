################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../._main.c \
../main.c \
../weather_api.c 

OBJS += \
./._main.o \
./main.o \
./weather_api.o 

C_DEPS += \
./._main.d \
./main.d \
./weather_api.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARTIK GCC C Compiler'
	arm-none-eabi-gcc  -D__TINYARA__ -I"C:/ARTIK/SDK/A053/v1.7/libsdk/extra/include" -I"C:/ARTIK/SDK/A053/v1.7/libsdk/extra/include/apps" -I"C:/ARTIK/SDK/A053/v1.7/libsdk/extra/framework_include" -I"C:/ARTIK/SDK/A053/v1.7/libsdk/extra/apps_include" -I"C:/ARTIK/SDK/A053/v1.7/libsdk/extra/apps_include/netutils" -I"C:/ARTIK/SDK/A053/v1.7/libsdk/extra/artik-sdk_include" -I"C:/ARTIK/SDK/A053/v1.7/libsdk/extra/artik-sdk_include/base" -I"C:/ARTIK/SDK/A053/v1.7/libsdk/extra/artik-sdk_include/connectivity" -I"C:/ARTIK/SDK/A053/v1.7/libsdk/extra/artik-sdk_include/systemio" -I"C:/ARTIK/SDK/A053/v1.7/libsdk/extra/artik-sdk_include/wifi" -I"C:/ARTIK/SDK/A053/v1.7/libsdk/extra/artik-sdk_include/lwm2m" -I"C:/ARTIK/SDK/A053/v1.7/libsdk/extra/artik-sdk_include/mqtt" -O0 -ffunction-sections -fdata-sections -fno-builtin -fno-strict-aliasing -fno-strength-reduce -fomit-frame-pointer -Wstrict-prototypes -Wshadow -Wno-implicit-function-declaration -Wno-unused-function -Wno-unused-but-set-variable -DAPP_STACKSIZE=18432 -DAPP_PRIORITY=100 -mcpu=cortex-r4 -mfpu=vfpv3 -g -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../._main.c \
../A053BasicKit.c \
../game.c \
../hext.c \
../hext_main.c 

OBJS += \
./._main.o \
./A053BasicKit.o \
./game.o \
./hext.o \
./hext_main.o 

C_DEPS += \
./._main.d \
./A053BasicKit.d \
./game.d \
./hext.d \
./hext_main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARTIK GCC C Compiler'
	arm-none-eabi-gcc  -D__TINYARA__ -I"C:/ARTIK/SDK/A053/v1.7/libsdk/extra/include" -I"C:/ARTIK/SDK/A053/v1.7/libsdk/extra/include/apps" -I"C:/ARTIK/SDK/A053/v1.7/libsdk/extra/framework_include" -I"C:/ARTIK/SDK/A053/v1.7/libsdk/extra/apps_include" -I"C:/ARTIK/SDK/A053/v1.7/libsdk/extra/apps_include/netutils" -I"C:/ARTIK/SDK/A053/v1.7/libsdk/extra/artik-sdk_include" -I"C:/ARTIK/SDK/A053/v1.7/libsdk/extra/artik-sdk_include/base" -I"C:/ARTIK/SDK/A053/v1.7/libsdk/extra/artik-sdk_include/connectivity" -I"C:/ARTIK/SDK/A053/v1.7/libsdk/extra/artik-sdk_include/systemio" -I"C:/ARTIK/SDK/A053/v1.7/libsdk/extra/artik-sdk_include/wifi" -I"C:/ARTIK/SDK/A053/v1.7/libsdk/extra/artik-sdk_include/lwm2m" -I"C:/ARTIK/SDK/A053/v1.7/libsdk/extra/artik-sdk_include/mqtt" -O0 -ffunction-sections -fdata-sections -fno-builtin -fno-strict-aliasing -fno-strength-reduce -fomit-frame-pointer -Wstrict-prototypes -Wshadow -Wno-implicit-function-declaration -Wno-unused-function -Wno-unused-but-set-variable -DAPP_STACKSIZE=18432 -DAPP_PRIORITY=100 -mcpu=cortex-r4 -mfpu=vfpv3 -g -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



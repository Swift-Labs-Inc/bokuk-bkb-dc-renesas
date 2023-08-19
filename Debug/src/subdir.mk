################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/adc.c \
../src/display.c \
../src/function.c \
../src/hal_entry.c \
../src/key.c \
../src/uart_connect.c \
../src/uart_debug.c 

C_DEPS += \
./src/adc.d \
./src/display.d \
./src/function.d \
./src/hal_entry.d \
./src/key.d \
./src/uart_connect.d \
./src/uart_debug.d 

OBJS += \
./src/adc.o \
./src/display.o \
./src/function.o \
./src/hal_entry.o \
./src/key.o \
./src/uart_connect.o \
./src/uart_debug.o 

SREC += \
RA2E1_BKB_DC3F61Q_ep01.srec 

MAP += \
RA2E1_BKB_DC3F61Q_ep01.map 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	$(file > $@.in,-mcpu=cortex-m23 -mthumb -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g -gdwarf-4 -D_RENESAS_RA_ -D_RA_CORE=CM23 -I"C:/Users/alex/e2_studio/workspace/RA2E1_BKB_DC3F61Q_ep01/src" -I"C:/Users/alex/e2_studio/workspace/RA2E1_BKB_DC3F61Q_ep01/ra/fsp/inc" -I"C:/Users/alex/e2_studio/workspace/RA2E1_BKB_DC3F61Q_ep01/ra/fsp/inc/api" -I"C:/Users/alex/e2_studio/workspace/RA2E1_BKB_DC3F61Q_ep01/ra/fsp/inc/instances" -I"C:/Users/alex/e2_studio/workspace/RA2E1_BKB_DC3F61Q_ep01/ra/arm/CMSIS_5/CMSIS/Core/Include" -I"C:/Users/alex/e2_studio/workspace/RA2E1_BKB_DC3F61Q_ep01/ra_gen" -I"C:/Users/alex/e2_studio/workspace/RA2E1_BKB_DC3F61Q_ep01/ra_cfg/fsp_cfg" -I"C:/Users/alex/e2_studio/workspace/RA2E1_BKB_DC3F61Q_ep01/ra_cfg/fsp_cfg/bsp" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"


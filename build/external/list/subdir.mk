# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../external/list/list.c \
../external/list/list_iterator.c \
../external/list/list_node.c 

OBJS += \
./external/list/list.o \
./external/list/list_iterator.o \
./external/list/list_node.o 

C_DEPS += \
./external/list/list.d \
./external/list/list_iterator.d \
./external/list/list_node.d 


# Each subdirectory must supply rules for building sources it contributes
external/list/%.o: ../external/list/%.c external/list/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"../external/list" -I"../ladder-parser" -I"../external/STC/include" -I"../ladder_parser/external/STC/include/stc" -O3 -g3 -ftest-coverage -fprofile-arcs -Wall -c -fmessage-length=0 -Wno-incompatible-pointer-types -Wno-switch -Wno-bool-compare -Wno-maybe-uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



C_SRCS += \
../ladder-parser/ladder_parser.c \
../ladder-parser/rung.c \
../ladder-parser/utils.c 

OBJS += \
./ladder-parser/ladder_parser.o \
./ladder-parser/rung.o \
./ladder-parser/utils.o 

C_DEPS += \
./ladder-parser/ladder_parser.d \
./ladder-parser/rung.d \
./ladder-parser/utils.d 


# Each subdirectory must supply rules for building sources it contributes
ladder-parser/%.o: ../ladder-parser/%.c ladder-parser/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"../external/list" -I"../ladder-parser" -I"../external/STC/include" -I"../external/STC/include/stc" -O3 -g3 -ftest-coverage -fprofile-arcs -Wall -c -fmessage-length=0 -Wno-incompatible-pointer-types -Wno-switch -Wno-bool-compare -Wno-maybe-uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



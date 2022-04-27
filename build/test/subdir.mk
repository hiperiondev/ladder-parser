C_SRCS += \
../test/test.c 

OBJS += \
./test/test.o 

C_DEPS += \
./test/test.d 


# Each subdirectory must supply rules for building sources it contributes
test/%.o: ../test/%.c test/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"../external/list" -I"../ladder-parser" -I"../external/STC/include" -I"../external/STC/include/stc" -O3 -g3 -Wall -c -fmessage-length=0 -Wno-incompatible-pointer-types -Wno-switch -Wno-bool-compare -Wno-maybe-uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



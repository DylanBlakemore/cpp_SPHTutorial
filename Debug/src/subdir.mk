################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/OutputFile.cpp \
../src/ParticleSystem.cpp \
../src/cpp_SPHTutorial.cpp \
../src/inputfile.cpp 

OBJS += \
./src/OutputFile.o \
./src/ParticleSystem.o \
./src/cpp_SPHTutorial.o \
./src/inputfile.o 

CPP_DEPS += \
./src/OutputFile.d \
./src/ParticleSystem.d \
./src/cpp_SPHTutorial.d \
./src/inputfile.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



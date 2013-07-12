################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Configuration.cpp \
../src/Evaluation.cpp \
../src/HillCimb.cpp \
../src/LTGA.cpp \
../src/MiddleLayer.cpp \
../src/Population.cpp \
../src/Pyramid.cpp \
../src/Util.cpp \
../src/main.cpp 

OBJS += \
./src/Configuration.o \
./src/Evaluation.o \
./src/HillCimb.o \
./src/LTGA.o \
./src/MiddleLayer.o \
./src/Population.o \
./src/Pyramid.o \
./src/Util.o \
./src/main.o 

CPP_DEPS += \
./src/Configuration.d \
./src/Evaluation.d \
./src/HillCimb.d \
./src/LTGA.d \
./src/MiddleLayer.d \
./src/Population.d \
./src/Pyramid.d \
./src/Util.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++11 -O3 -funroll-loops -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



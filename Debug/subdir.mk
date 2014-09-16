################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../FileIO.cpp \
../G308_Geometry.cpp \
../G308_ImageLoader.cpp \
../G308_Scene.cpp \
../TextureDemo.cpp 

OBJ_SRCS += \
../Box.obj \
../Bunny.obj \
../Sphere.obj \
../Table.obj \
../Teapot.obj \
../Torus.obj 

OBJS += \
./FileIO.o \
./G308_Geometry.o \
./G308_ImageLoader.o \
./G308_Scene.o \
./TextureDemo.o 

CPP_DEPS += \
./FileIO.d \
./G308_Geometry.d \
./G308_ImageLoader.d \
./G308_Scene.d \
./TextureDemo.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



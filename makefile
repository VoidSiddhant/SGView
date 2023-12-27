CC = g++
Incl = -I ./SGView/Include/
SRC = ./SGView/Source/SGImageProc.cpp ./SGView/Source/SGCore.cpp ./SGView/Source/SGStencil.cpp ./SGView/Source/SGImageEffect.cpp
SYS_LIB = -lglut -L/usr/lib -L/usr/X11R6/lib -lXi -lXmu -lGL -lGLU -lm -lOpenImageIO
file = cue.exr

clean :
		rm ./SGView/Libs/myLib.a
		rm main
compile:
		
		${CC} ${Incl} -c ${SRC}
		ar rcs myLib.a SGCore.o SGImageProc.o SGImageEffect.o SGStencil.o
		mv myLib.a ./SGView/Libs
		rm SGCore.o
		rm SGImageProc.o
		rm SGStencil.o
		rm SGImageEffect.o
	
build:
		${CC} ${Incl} -o sgviewapp main.cpp ./SGView/Libs/myLib.a ${SYS_LIB}

install :  compile build

run :  
		./sgviewapp -image ${file}

debug : install
		./sgviewapp -image ${file}
		
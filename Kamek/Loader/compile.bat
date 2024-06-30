@ECHO off

for %%f in (*.S) do (
C:\devkitPro\devkitPPC\bin\powerpc-eabi-as -mregnames %%f -o %%~nf.o
C:\devkitPro\devkitPPC\bin\powerpc-eabi-ld -Ttext 0x800046F0 --oformat binary %%~nf.o -o %%~nf.bin
del /f %%~nf.o
ren %%~nf.bin Loader.bin
)

pause
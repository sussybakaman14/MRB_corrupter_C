@echo off
windres resources.rc -O coff -o resources.res
gcc main.c resources.res -o MRB_corrupter.exe -lgdi32 -lwinmm -O2
echo Operazione Completata
pause
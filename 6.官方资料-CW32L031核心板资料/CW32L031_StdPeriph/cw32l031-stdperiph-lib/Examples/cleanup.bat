del *.bak /s
:: keil
del *.axf /s
del *.htm /s
del *.hex /s
del *.lnp /s
del *.sct /s
del *.crf /s
del *.d /s
del *.o /s
del *.dep /s
del *.map /s
del *.lst /s
del *.i /s
del *.scvd /s
del *.uvguix.* /s
del *.Socrates /s
del /s/q output
del JLink*.* /s

:: IAR
rem del *.hex /s
del *.out /s
del *.sim /s
del *.linf /s
del *.pbi /s
rem del *.o /s
del *.xcl /s
del *.browse /s
del *.pbd /s
del *.pbw /s
del Backup*.* /s
del *.ewt /s
del *.reggroups /s
del *.crun /s
del *.dbgdt /s
del *.cspy.* /s
del *.wsdt /s
del *.jlink /s
del *.ewd /s

del *.svd /s
del *.bak* /s
del *.orig /s

cd %~dp0
for /F "delims=" %%a in ('dir /ad/s/b output') do rd /s/q "%%~a"
for /F "delims=" %%a in ('dir /ad/s/b settings') do rd /s/q "%%~a"
for /F "delims=" %%a in ('dir /ad/s/b Release') do rd /s/q "%%~a"
for /F "delims=" %%a in ('dir /ad/s/b Debug') do rd /s/q "%%~a"
for /F "delims=" %%a in ('dir /ad/s/b RTE') do rd /s/q "%%~a"
for /F "delims=" %%a in ('dir /ad/s/b flashloader') do rd /s/q "%%~a"
del *.icf /s
pause
exit

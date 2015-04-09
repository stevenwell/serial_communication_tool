@PUSHD
@IF NOT EXIST %windir%\system32\mscomm32.ocx @copy  mscomm32.ocx %windir%\system32 /Y
@regsvr32 /s mscomm32.ocx
@POPD
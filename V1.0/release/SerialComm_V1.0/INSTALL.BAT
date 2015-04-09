@PUSHD
@copy  mscomm32.ocx %windir%\system32 /Y
@regsvr32 /s mscomm32.ocx
@POPD
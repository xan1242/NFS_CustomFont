@echo off
set /A NEWTPKMODE=0

IF "%~1"=="" GOTO info
IF "%~2"=="" GOTO info
IF "%~3"=="" GOTO info
IF NOT EXIST %~1 GOTO error
GOTO cont

:info
echo Usage: %0 FFN_name FEngFont_name [TPKmode: 0/1]
echo TPKmode 0 is for NFSU, NFSU2 and NFSMW, TPKmode 1 is for all newer ones
EXIT /B 1

:error
echo ERROR: %~1 does not exist
EXIT /B 1

:cont
set /A NEWTPKMODE=%3
NFSRealFont "%~dpnx1"
NFSRealFont -g "%~dpn1_cut.ffn" %2
gx -dxt3 "%~dpn1.fsh" -=%~dpn1.dds
COPY /Y %~dpn1.dds %2.dds
REM TPK GENERATOR FOR XNFSTPKTOOL
ECHO [TPK]> %2.ini
ECHO Name = %2>> %2.ini
<nul (set/p nlv=Version = 5)>> %2.ini
ECHO:>> %2.ini
ECHO Filename = %2.tpk>> %2.ini
<nul (set/p nlv=FilenameHash = 0x)>> %2.ini
strhash %2.tpk>> %2.ini
ECHO:>> %2.ini
<nul (set/p nlv=Animations = 0)>> %2.ini
ECHO:>> %2.ini
ECHO:>> %2.ini
<nul (set/p nlv=[)>> %2.ini
strhash %2>> %2.ini
<nul (set/p nlv=])>> %2.ini
ECHO:>> %2.ini
ECHO File = %~dpn2.dds>> %2.ini
ECHO Name = %2>> %2.ini
ECHO ClassNameHash = 0x1A93CF>> %2.ini
<nul (set/p nlv=ImageCompressionType = 0x24)>> %2.ini
ECHO:>> %2.ini
<nul (set/p nlv=PaletteCompressionType = 0x0)>> %2.ini
ECHO:>> %2.ini
<nul (set/p nlv=NumPaletteEntries = 0)>> %2.ini
ECHO:>> %2.ini
<nul (set/p nlv=TilableUV = 0)>> %2.ini
ECHO:>> %2.ini
<nul (set/p nlv=BiasLevel = 0)>> %2.ini
ECHO:>> %2.ini
<nul (set/p nlv=RenderingOrder = 5)>> %2.ini
ECHO:>> %2.ini
<nul (set/p nlv=ScrollType = 0)>> %2.ini
ECHO:>> %2.ini
<nul (set/p nlv=UsedFlag = 0)>> %2.ini
ECHO:>> %2.ini
<nul (set/p nlv=ApplyAlphaSorting = 0)>> %2.ini
ECHO:>> %2.ini
<nul (set/p nlv=AlphaUsageType = 2)>> %2.ini
ECHO:>> %2.ini
<nul (set/p nlv=AlphaBlendType = 1)>> %2.ini
ECHO:>> %2.ini
<nul (set/p nlv=Flags = 0x0)>> %2.ini
ECHO:>> %2.ini
<nul (set/p nlv=ScrollTimeStep = 0)>> %2.ini
ECHO:>> %2.ini
<nul (set/p nlv=ScrollSpeedS = 0)>> %2.ini
ECHO:>> %2.ini
<nul (set/p nlv=ScrollSpeedT = 0)>> %2.ini
ECHO:>> %2.ini
<nul (set/p nlv=OffsetS = 0)>> %2.ini
ECHO:>> %2.ini
<nul (set/p nlv=OffsetT = 256)>> %2.ini
ECHO:>> %2.ini
<nul (set/p nlv=ScaleS = 256)>> %2.ini
ECHO:>> %2.ini
<nul (set/p nlv=ScaleT = 0)>> %2.ini
ECHO:>> %2.ini
<nul (set/p nlv=PixelFormatUnk1 = 0x1)>> %2.ini
ECHO:>> %2.ini
<nul (set/p nlv=PixelFormatUnk2 = 0x5)>> %2.ini
ECHO:>> %2.ini
<nul (set/p nlv=PixelFormatUnk3 = 0x6)>> %2.ini
ECHO:>> %2.ini
REM TPK GENERATOR END
IF %NEWTPKMODE%==1 (
xnfstpktool -w %2.ini %2.tpk 
) ELSE (
xnfstpktool -w2 %2.ini %2.tpk
)
COPY /B %2.tpk + %2.bin %2.bun
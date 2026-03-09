set /p version=<..\\major_version.txt
@echo %version%
set /p old=<..\\build_number.txt
set /a new=%old%+1
@echo New build number: %new%
@echo Version: %version%.%new%
@echo %new% > ..\\build_number.txt
(
  echo // AVS - automatic version system by NNM Electric s.r.o.
  echo #ifndef AVS_BUILD_NUMBER
  echo #define AVS_BUILD_NUMBER %new%U
  echo #endif
  echo #ifndef AVS_VERSION_NUMBER
  echo #define AVS_VERSION_NUMBER %version%U
  echo #endif
  echo #ifndef AVS_BUILD_NUMBER_STR
  echo #define AVS_BUILD_NUMBER_STR "%new%"
  echo #endif
  echo #ifndef AVS_VERSION_FULL_STR
  echo #define AVS_VERSION_FULL_STR "%version%.%new%"
  echo #endif
) > ..\\build_number.h

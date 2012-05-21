if (${?VBFANALYSISPKG}) then
echo "already set"
else
setenv THISDIR `pwd`

setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:${THISDIR}/lib
setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:${THISDIR}/../NtuplePackage/lib
setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:${THISDIR}/../KinFitter/lib

if (${?DYLD_LIBRARY_PATH}) then
setenv DYLD_LIBRARY_PATH ${DYLD_LIBRARY_PATH}:${THISDIR}/lib
setenv DYLD_LIBRARY_PATH ${DYLD_LIBRARY_PATH}:${THISDIR}../NtuplePackage/lib
setenv DYLD_LIBRARY_PATH ${DYLD_LIBRARY_PATH}:${THISDIR}../KinFitter/lib
endif

setenv PATH ${PATH}:${THISDIR}/bin
setenv PATH ${PATH}:${THISDIR}/../NtuplePackage/bin
setenv PATH ${PATH}:${THISDIR}/../KinFitter/bin

setenv NTUPLEPKGINCLUDE ${THISDIR}/../NtuplePackage/interface
setenv NTUPLEPKGLIB ${THISDIR}/../NtuplePackage/lib

setenv KINFITPKGINCLUDE ${THISDIR}/../KinFitter/interface
setenv KINFITPKGLIB ${THISDIR}/../KinFitter/lib

setenv VBFANALYSISPKG ${THISDIR}/
endif

if (${?VBFANALYSISPKG}) then
echo "already set"
else
setenv THISDIR `pwd`

setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:${THISDIR}/lib
setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:${THISDIR}/../NtuplePackage/lib

#setenv DYLD_LIBRARY_PATH ${DYLD_LIBRARY_PATH}:${THISDIR}/lib

setenv PATH ${PATH}:${THISDIR}/bin
setenv PATH ${PATH}:${THISDIR}/../NtuplePackage/bin

setenv NTUPLEPKGINCLUDE ${THISDIR}/../NtuplePackage/interface
setenv NTUPLEPKGLIB ${THISDIR}/../NtuplePackage/lib

setenv VBFANALYSISPKG ${THISDIR}/
endif

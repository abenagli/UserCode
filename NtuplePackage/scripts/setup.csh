f (${?VBFANALYSISPKG}) then
echo "already set"
else
setenv THISDIR `pwd`

setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:${THISDIR}/lib
#setenv DYLD_LIBRARY_PATH ${DYLD_LIBRARY_PATH}:${THISDIR}/lib
setenv PATH ${PATH}:${THISDIR}/bin

setenv NTUPLEPKGINCLUDE ${THISDIR}/interface
setenv NTUPLEPKGLIB ${THISDIR}/lib
endif

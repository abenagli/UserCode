if (${?COMMONUTILS}) then
echo "already set"
else
setenv THISDIR `pwd`

setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:${THISDIR}/lib
#setenv DYLD_LIBRARY_PATH ${DYLD_LIBRARY_PATH}:${THISDIR}/lib
setenv PATH ${PATH}:${THISDIR}/bin

setenv COMMONUTILSINCLUDE ${THISDIR}/interface
setenv COMMONUTILSLIB ${THISDIR}/lib
endif

f (${?VBFANALYSISPKG}) then
echo "already set"
else
export THISDIR=`pwd`

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${THISDIR}/lib
export DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:${THISDIR}/lib

export PATH=${PATH}:${THISDIR}/bin

export NTUPLEPKGINCLUDE=${THISDIR}/interface
export NTUPLEPKGLIB=${THISDIR}/lib
endif
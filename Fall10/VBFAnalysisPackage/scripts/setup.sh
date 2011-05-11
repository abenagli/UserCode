if [ -n "${VBFANALYSISPKG}" ] ; then
echo "already set"
else
export THISDIR=`pwd`

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${THISDIR}/lib
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${THISDIR}/../NtuplePackage/lib
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${THISDIR}/../KinFitter/lib

if [ -n "${DYLD_LIBRARY_PATH}" ] ; then
export DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:${THISDIR}/lib
export DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:${THISDIR}/../NtuplePackage/lib
export DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:${THISDIR}/../KinFitter/lib
fi

export PATH=${PATH}:${THISDIR}/bin
export PATH=${PATH}:${THISDIR}/../NtuplePackage/bin
export PATH=${PATH}:${THISDIR}/../KinFitter/bin

export NTUPLEPKGINCLUDE=${THISDIR}/../NtuplePackage/interface
export NTUPLEPKGLIB=${THISDIR}/../NtuplePackage/lib

export KINFITPKGINCLUDE=${THISDIR}/../KinFitter/interface
export KINFITPKGLIB=${THISDIR}/../KinFitter/lib

export VBFANALYSISPKG=${THISDIR}/
fi

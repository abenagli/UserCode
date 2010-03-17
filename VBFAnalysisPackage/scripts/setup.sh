export THISDIR=`pwd`

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${THISDIR}/lib
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${THISDIR}/../NtuplePackage/lib

export DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:${THISDIR}/lib

export PATH=${PATH}:${THISDIR}/bin
export PATH=${PATH}:${THISDIR}/../NtuplePackage/bin

export NTUPLEPKGINCLUDE=${THISDIR}/../NtuplePackage/interface
export NTUPLEPKGLIB=${THISDIR}/../NtuplePackage/lib

export VBFANALYSISPKG=${THISDIR}/

cd ${THISDIR}/../NtuplePackage/
source scripts/setup.sh
cd -
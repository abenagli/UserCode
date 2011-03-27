if [ -n "${KINFITTERPKG}" ]; then
echo "already set"
else
export THISDIR=`pwd`

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${THISDIR}/lib

if [ -n "${DYLD_LIBRARY_PATH}" ]; then
export DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:${THISDIR}/lib
fi

export PATH=${PATH}:${THISDIR}/bin

export KINFITPKGINCLUDE=${THISDIR}/interface
export KINFITPKGLIB=${THISDIR}/lib
fi

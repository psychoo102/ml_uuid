#########################
# Remove old binary
#########################
if [ -e "./x64/ml_uuid.so" ]; then
    rm ./x64/ml_uuid.so
fi

#########################
# Prepare build
#########################
autoreconf -ifv
./configure CXXFLAGS='-m64 -g -O2 -fPIC' CFLAGS='-m64' LDFLAGS='-m64'
make clean

#########################
# Do build
#########################
echo Building...
make >_make-64.log

#########################
# Check for error
#########################
rc=$?
if [ $rc -ne 0 ]; then
    echo "Stopping: make returned error code $rc"
    exit 1
fi

#########################
# Copy binary file
#########################
mkdir x64
cp src/.libs/libml_uuid.so ./x64/ml_uuid.so
echo "Build completed "`pwd`"./x64/ml_uuid.so"
exit 0

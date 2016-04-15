#!/bin/bash

# First, get into the right directory
THISPATH=$(dirname "$BASH_SOURCE")
cd $THISPATH

# Now, we check to see that the version env var is set
${CPACK_PACKAGE_VERSION:?"Need to set CPACK_PACKAGE_VERSION env var!"}
${CPACK_DEBIAN_PACKAGE_ARCHITECTURE:?"Need to set CPACK_DEBIAN_PACKAGE_ARCHITECTURE env var!"}

# OK, now we get down to the business of building...
rm -rf ./build
mkdir build
cd build
cmake ..
make package
cpack -G DEB
mv ./*.deb ../installer-packages
echo "If you've gotten to this point, the package has been built"
echo "and exists at "$THISPATH"/installer-packages"
echo "Don't forget to add and commit the new package, and tag the commit!"

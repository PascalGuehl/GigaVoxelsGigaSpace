#!/bin/sh

# Set the working directory correctly
cd "$(dirname "$0")"

################## Script parameters ###################
packageVersion="1.0"
svnLogin=
gpgKey=
svnpath="svn+ssh://$svnLogin@scm.gforge.inria.fr/svnroot/gigavoxelsdist/trunk/"
#distributions=trusty wheezy
distributions=trusty
########################################################

workdir=gigaspace-$packageVersion

# Clean old working directory if still present.
rm -rf "$workdir"

# Get latest source from svn
svn co "$svnpath" "$workdir"
if ! test -d "$workdir"; then
	echo "Error, $workdir wasn't created correctly."
	exit 1
fi

cd "$workdir"

# Beware, for the cut -d\, we need a "space" character as delimiter
echo "Attempting to get svn revision number..."
svn=`svn info | grep 'Revision:' | cut -d\  -f2`
echo "Revision: $svn"

# Clean .svn directories
echo "Cleaning .svn files"
find -type d -name .svn|xargs rm -rf

# Clear previous DISTRIBUTIONS
rm Distributions/Windows/Distrib/*
rm Distributions/Linux/Distrib/*

# Modify the default settings to use the Linux one instead
cp ../linuxSettings.xml Data/Settings/GigaSpace.xml

# Compile the package for all available Cuda architectures
cmakefile="Development/CMake/GvSettings_CMakeImport.cmake"
echo 'list(APPEND CUDA_NVCC_FLAGS "-gencode=arch=compute_20,code=\"sm_20,compute_20\" -gencode=arch=compute_30,code=\"sm_30,compute_30\" -gencode=arch=compute_35,code=\"sm_35,compute_35\" ")' >> "$cmakefile"


for dist in "$distributions"; do
	# Find the changelog
	echo copying changelog for $dist
	cp -r ../debian ./
	rm debian/changelog
	cat ../debian/changelog | sed -e s/XXXXXX/"$svn"/g | sed -e s/YYYYYY/"$dist"/g > debian/changelog

	# Find the correct control file
	control=$( cd "../debian/" && pwd )"/control.$dist"
	if test -e $control; then
		cp $control debian/control
	else
		echo "Error: can't generate package for distribution $dist:"
		echo "Can't find file $control"
		continue
	fi

	debuild --no-lintian -S -k$gpgKey
done

cd ..
echo Cleaning...
rm -rf "$workdir"

echo Done.

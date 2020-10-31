#!/bin/sh
# Exit script on error
set -e

# Set the working directory correctly
cd "$(dirname "$0")"

# **************************************************************************
# CMAKE GENERATION
# **************************************************************************
chmod +x makeLibrary.sh
./makeLibrary.sh
chmod +x makeTools.sh
./makeTools.sh
chmod +x makeViewerPlugin.sh
./makeViewerPlugin.sh
chmod +x makeDemo.sh
./makeDemo.sh

# **************************************************************************
# COPY DATA AND SHADERS
# **************************************************************************
chmod +x updateData.sh
./updateData.sh
chmod +x updateShaders.sh
./updateShaders.sh

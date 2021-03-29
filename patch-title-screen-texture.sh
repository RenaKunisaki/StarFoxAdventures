#!/bin/sh
pushd misc-scripts/texconv
./__main__.py pack RGB5A3 1 ../../new-textures/01C6.00.RGB5A3.png ../../dump/gamefront/TEX0/01C6.00.tex 
cd ../../extractor/
./__main__.py packTextures ../dump/gamefront/TEX0/ ../dump/gamefront/newTEX0 0
cp ../dump/gamefront/newTEX0.bin ~/projects/sfa/changed/files/gamefront/TEX0.bin 
cp ../dump/gamefront/newTEX0.tab ~/projects/sfa/changed/files/gamefront/TEX0.tab
cp ../dump/gamefront/newTEX0.bin ~/projects/sfa/files/gamefront/TEX0.bin 
cp ../dump/gamefront/newTEX0.tab ~/projects/sfa/files/gamefront/TEX0.tab
popd

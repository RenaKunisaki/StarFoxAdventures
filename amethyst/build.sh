#!/bin/sh
# build script for my own system
# just moves some directories off a network mount for performance
DISCROOT=/home/rena/projects/sfa/files ORIGISO=~/projects/sfa/original.iso PATCHISO=~/projects/sfa/patched.iso make $*

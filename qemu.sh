#!/bin/sh
set -e
. ./iso.sh
if [ -z $1  ] ; then
  qemu-system-$(./target-triplet-to-arch.sh $HOST) -cdrom beanOS.iso
else
  qemu-system-$(./target-triplet-to-arch.sh $HOST) -s -S -cdrom beanOS.iso
fi


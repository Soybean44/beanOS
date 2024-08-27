#!/bin/bash
set -e
  ./iso.sh

qemu-system-x86_64 --cdrom beanos.iso

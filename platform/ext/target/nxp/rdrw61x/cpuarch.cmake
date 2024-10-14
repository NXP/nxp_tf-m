#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
# Copyright 2023-2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

# cpuarch.cmake is used to set things that related to the platform that are both
# immutable and global, which is to say they should apply to any kind of project
# that uses this platform. In practise this is normally compiler definitions and
# variables related to hardware.

# Set architecture and CPU
set(TFM_SYSTEM_PROCESSOR cortex-m33)
set(TFM_SYSTEM_ARCHITECTURE armv8-m.main)
set(TFM_SYSTEM_DSP 0)

# Set processor type for NXP MCUx SDK
add_definitions(-DCPU_RW612ETA2I)

# Define serial port ID
add_definitions(-DSERIAL_PORT_TYPE_UART=1)

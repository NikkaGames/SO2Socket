LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := c
LOCAL_CFLAGS := -w -s -Wno-error=format-security -fvisibility=hidden -fpermissive -fexceptions
LOCAL_CPPFLAGS := -w -s -Wno-error=format-security -fvisibility=hidden -Werror -std=c++17
LOCAL_CPPFLAGS += -Wno-error=c++11-narrowing -fpermissive -Wall -fexceptions
LOCAL_CFLAGS += -mllvm -fla -mllvm -split -mllvm -split_num=6 -mllvm -sub -mllvm -sub_loop=2 -mllvm -bcf_loop=2 -mllvm -bcf_prob=100
LOCAL_LDFLAGS += -Wl,--gc-sections,--strip-all,-llog
LOCAL_LDLIBS := -llog -landroid -lz
LOCAL_ARM_NEON := true
LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := hello.cpp \
     AES.cpp \
     base64.cpp \
     NikkaH/NikkaH.cpp \
     xdl/xdl.c \
     xdl/xdl_iterate.c \
     xdl/xdl_linker.c \
     xdl/xdl_lzma.c \
     xdl/xdl_util.c \
     KittyMemory/KittyMemory.cpp \
     KittyMemory/MemoryPatch.cpp \
     KittyMemory/MemoryBackup.cpp \
     KittyMemory/KittyUtils.cpp \
     hide.cpp \
     pmparser.cpp

include $(BUILD_SHARED_LIBRARY)

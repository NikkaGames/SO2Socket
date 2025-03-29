LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libdobby
LOCAL_CFLAGS := -w -s -Wno-error=format-security -fvisibility=hidden -fpermissive -fexceptions
LOCAL_CPPFLAGS := -w -s -Wno-error=format-security -fvisibility=hidden -Werror -std=c++17
LOCAL_CPPFLAGS += -Wno-error=c++11-narrowing -fpermissive -Wall -fexceptions
LOCAL_CFLAGS += -mllvm -fla -mllvm -split -mllvm -split_num=2 -mllvm -sub -mllvm -bcf -mllvm -bcf_loop=1 -mllvm -bcf_prob=85 -mllvm -sobf
LOCAL_LDFLAGS += -Wl,--gc-sections,--strip-all,-llog
LOCAL_ARM_MODE := arm
LOCAL_SRC_FILES := libraries/$(TARGET_ARCH_ABI)/libdobby.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libcurl
LOCAL_CFLAGS := -w -s -Wno-error=format-security -fvisibility=hidden -fpermissive -fexceptions
LOCAL_CPPFLAGS := -w -s -Wno-error=format-security -fvisibility=hidden -Werror -std=c++17
LOCAL_CPPFLAGS += -Wno-error=c++11-narrowing -fpermissive -Wall -fexceptions
LOCAL_CFLAGS += -mllvm -fla -mllvm -split -mllvm -split_num=2 -mllvm -sub -mllvm -bcf -mllvm -bcf_loop=1 -mllvm -bcf_prob=85 -mllvm -sobf
LOCAL_LDFLAGS += -Wl,--gc-sections,--strip-all,-llog
LOCAL_LDLIBS := -lc -lz
LOCAL_ARM_MODE := arm
LOCAL_SRC_FILES := libraries/$(TARGET_ARCH_ABI)/libcurl.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := c
LOCAL_CFLAGS := -w -s -Wno-error=format-security -fvisibility=hidden -fpermissive -fexceptions
LOCAL_CPPFLAGS := -w -s -Wno-error=format-security -fvisibility=hidden -Werror -std=c++17
LOCAL_CPPFLAGS += -Wno-error=c++11-narrowing -fpermissive -Wall -fexceptions
LOCAL_CFLAGS += -mllvm -fla -mllvm -split -mllvm -split_num=2 -mllvm -sub -mllvm -bcf -mllvm -bcf_loop=1 -mllvm -bcf_prob=85 -mllvm -sobf
LOCAL_LDFLAGS += -Wl,--gc-sections,--strip-all,-llog
LOCAL_LDLIBS := -llog -landroid -lz
LOCAL_ARM_NEON := true
LOCAL_ARM_MODE := arm

LOCAL_STATIC_LIBRARIES := libdobby libcurl
LOCAL_C_INCLUDES += $(LOCAL_PATH)/ImGui


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

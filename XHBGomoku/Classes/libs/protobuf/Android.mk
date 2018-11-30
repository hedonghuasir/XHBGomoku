LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := protobuf_lite_static

LOCAL_MODULE_FILENAME := libprotobuf-lite-android

ifeq ($(TARGET_ARCH_ABI),armeabi)
	LOCAL_ARM_MODE := arm
endif

LOCAL_SRC_FILES := \
src/google/protobuf/io/zero_copy_stream_impl_lite.cc \
src/google/protobuf/io/zero_copy_stream.cc \
src/google/protobuf/io/coded_stream.cc \
src/google/protobuf/wire_format_lite.cc \
src/google/protobuf/repeated_field.cc \
src/google/protobuf/message_lite.cc \
src/google/protobuf/generated_message_util.cc \
src/google/protobuf/extension_set.cc \
src/google/protobuf/stubs/once.cc \
src/google/protobuf/stubs/common.cc

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/src

LOCAL_C_INCLUDES := $(LOCAL_PATH)/src \
$(LOCAL_PATH)/../../src

LOCAL_STATIC_LIBRARIES := JavaScriptCore_static

LOCAL_CFLAG += -DGOOGLE_PROTOBUF_NO_RTTI -DGOOGLE_PROTOBUF_NO_THREAD_SAFETY -fPIC

LOCAL_EXPORT_CFLAGS := -DGOOGLE_PROTOBUF_NO_RTTI -DGOOGLE_PROTOBUF_NO_THREAD_SAFETY

include $(BUILD_STATIC_LIBRARY)

$(call import-module,JavaScriptCore/android)

// Copyright(c) 2017 POLYGONTEK
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http ://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "Precompiled.h"
#include "OpenGL.h"

BE_NAMESPACE_BEGIN

bool OpenGLBase::supportsPackedFloat = false;
bool OpenGLBase::supportsDepthClamp = false;
bool OpenGLBase::supportsDepthBoundsTest = false;
bool OpenGLBase::supportsDepthBufferFloat = false;
bool OpenGLBase::supportsPixelBuffer = false;
bool OpenGLBase::supportsTextureRectangle = false;
bool OpenGLBase::supportsTextureArray = false;
bool OpenGLBase::supportsTextureFilterAnisotropic = false;
bool OpenGLBase::supportsTextureCompressionS3TC = false;
bool OpenGLBase::supportsTextureCompressionLATC = false;
bool OpenGLBase::supportsTextureCompressionRGTC = false;
bool OpenGLBase::supportsTextureCompressionETC2 = false;
bool OpenGLBase::supportsTextureCompressionATC = false;
bool OpenGLBase::supportsDebugLabel = false;
bool OpenGLBase::supportsDebugMarker = false;
bool OpenGLBase::supportsDebugOutput = false;
bool OpenGLBase::supportsBufferStorage = false;
bool OpenGLBase::supportsCopyImage = false;

void OpenGLBase::Init() {
#ifdef GL_EXT_packed_float // 3.0
    supportsPackedFloat = gglext._GL_EXT_packed_float ? true : false;
#endif

#ifdef GL_ARB_depth_clamp // 3.2
    supportsDepthClamp = gglext._GL_ARB_depth_clamp ? true : false;
#endif

#ifdef GL_EXT_depth_bounds_test
    supportsDepthBoundsTest = gglext._GL_EXT_depth_bounds_test ? true : false;
#endif

#ifdef GL_NV_depth_buffer_float // 3.0
    supportsDepthBufferFloat = gglext._GL_NV_depth_buffer_float ? true : false;
#endif

#ifdef GL_ARB_pixel_buffer_object // 2.1
    supportsPixelBuffer = gglext._GL_ARB_pixel_buffer_object ? true : false;
#endif

#ifdef GL_ARB_texture_rectangle // 3.1
    supportsTextureRectangle = gglext._GL_ARB_texture_rectangle ? true : false;
#endif

#ifdef GL_EXT_texture_array // 3.0
    supportsTextureArray = gglext._GL_EXT_texture_array ? true : false;
#endif

#ifdef GL_EXT_texture_filter_anisotropic
    supportsTextureFilterAnisotropic = gglext._GL_EXT_texture_filter_anisotropic ? true : false;
#endif

#ifdef GL_EXT_texture_compression_s3tc
    supportsTextureCompressionS3TC = gglext._GL_EXT_texture_compression_s3tc ? true : false;
#endif

#ifdef GL_EXT_texture_compression_latc
    supportsTextureCompressionLATC = gglext._GL_EXT_texture_compression_latc ? true : false;
#endif

#ifdef GL_EXT_texture_compression_rgtc
    supportsTextureCompressionRGTC = gglext._GL_EXT_texture_compression_rgtc ? true : false;
#endif

#ifdef GL_ARB_ES3_compatibility
    supportsTextureCompressionETC2 = gglext._GL_ARB_ES3_compatibility ? true : false;
#endif

#ifdef GL_AMD_compressed_ATC_texture
    supportsTextureCompressionATC = gglext._GL_AMD_compressed_ATC_texture ? true : false;
#endif

#ifdef GL_EXT_debug_label
    supportsDebugLabel = gglext._GL_EXT_debug_label ? true : false;
#endif

#ifdef GL_EXT_debug_marker
    supportsDebugMarker = gglext._GL_EXT_debug_marker ? true : false;
#endif

#ifdef GL_ARB_debug_output
    supportsDebugOutput = gglext._GL_ARB_debug_output ? true : false;
#endif

#ifdef GL_ARB_buffer_storage
    supportsBufferStorage = gglext._GL_ARB_buffer_storage ? true : false;
#endif

#ifdef GL_ARB_copy_image
    supportsCopyImage = gglext._GL_ARB_copy_image ? true : false;
#endif
}

bool OpenGLBase::ImageFormatToGLFormat(Image::Format::Enum imageFormat, bool isSRGB, GLenum *glFormat, GLenum *glType, GLenum *glInternal) {
    return false;
}

Image::Format::Enum OpenGLBase::ToCompressedImageFormat(Image::Format::Enum inFormat, bool useNormalMap) {
    assert(0);
    return Image::Format::Unknown;
}

Image::Format::Enum OpenGLBase::ToUncompressedImageFormat(Image::Format::Enum inFormat) {
    if (!Image::IsCompressed(inFormat)) {
        assert(0);
        return inFormat;
    }

    Image::Format::Enum outFormat;

    switch (inFormat) {
    case Image::Format::DXN1:
    case Image::Format::DXN2:
    case Image::Format::RGB_PVRTC_2BPPV1:
    case Image::Format::RGB_PVRTC_4BPPV1:
    case Image::Format::RGB_8_ETC1:
    case Image::Format::RGB_8_ETC2:
    case Image::Format::RGB_ATC:
        outFormat = Image::Format::RGB_8_8_8;
        break;
    case Image::Format::DXT1:
    case Image::Format::DXT3:
    case Image::Format::DXT5:
    case Image::Format::RGBA_PVRTC_2BPPV1:
    case Image::Format::RGBA_PVRTC_4BPPV1:
    case Image::Format::RGBA_PVRTC_2BPPV2:
    case Image::Format::RGBA_PVRTC_4BPPV2:
    case Image::Format::RGBA_8_1_ETC2:
    case Image::Format::RGBA_8_8_ETC2:
    case Image::Format::RGBA_EA_ATC:
    case Image::Format::RGBA_IA_ATC:
        outFormat = Image::Format::RGBA_8_8_8_8;
        break;
    case Image::Format::R_11_EAC:
    case Image::Format::SignedR_11_EAC:
        outFormat = Image::Format::R_8;
        break;
    case Image::Format::RG_11_11_EAC:
    case Image::Format::SignedRG_11_11_EAC:
        outFormat = Image::Format::RGB_8_8_8;
        break;
    default:
        assert(0);
        outFormat = inFormat;
        break;
    }

    return outFormat;
}

// NOTE: Do not call gglXXX function in MyGLCheckError
extern "C" void MyGLCheckError(const char *msg) {
    char errCode[64];
    GLenum err = glGetError();
    
    if (err != GL_NO_ERROR) {
        switch (err) {
            case GL_INVALID_ENUM:
                strcpy(errCode, "GL_INVALID_ENUM");
                break;
            case GL_INVALID_VALUE:
                strcpy(errCode, "GL_INVALID_VALUE");
                break;
            case GL_INVALID_OPERATION:
                strcpy(errCode, "GL_INVALID_OPERATION");
                break;
            case GL_OUT_OF_MEMORY:
                strcpy(errCode, "GL_OUT_OF_MEMORY");
                break;
#ifdef GL_VERSION_1_1
            case GL_STACK_OVERFLOW:
                strcpy(errCode, "GL_STACK_OVERFLOW");
                break;
            case GL_STACK_UNDERFLOW:
                strcpy(errCode, "GL_STACK_UNDERFLOW");
                break;
#endif
            default:
                Str::snPrintf(errCode, sizeof(errCode), "unknown (%i)", err);
                break;
        }

        BE_WARNLOG("GL Error: %s in %s\n", errCode, msg);
    }
}

extern "C" void (*GGLCheckError)(const char *msg) = MyGLCheckError;

BE_NAMESPACE_END

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
#include "Render/Render.h"
#include "RenderInternal.h"

BE_NAMESPACE_BEGIN

VertexFormat vertexFormats[VertexFormat::Type::Count];

void VertexFormat::Init() {
    // Xy
    vertexFormats[VertexFormat::Type::Xy].Append(0, 0, RHI::VertexElement::Usage::Position, 2, RHI::VertexElement::Type::Float, false, 0);
    vertexFormats[VertexFormat::Type::Xy].Create();

    // XySt
    vertexFormats[VertexFormat::Type::XySt].Append(0, 0, RHI::VertexElement::Usage::Position, 2, RHI::VertexElement::Type::Float, false, 0);
    vertexFormats[VertexFormat::Type::XySt].Append(0, 8, RHI::VertexElement::Usage::TexCoord0, 2, RHI::VertexElement::Type::Float, false, 0);
    vertexFormats[VertexFormat::Type::XySt].Create();

    // XyStr
    vertexFormats[VertexFormat::Type::XyStr].Append(0, 0, RHI::VertexElement::Usage::Position, 2, RHI::VertexElement::Type::Float, false, 0);
    vertexFormats[VertexFormat::Type::XyStr].Append(0, 8, RHI::VertexElement::Usage::TexCoord0, 3, RHI::VertexElement::Type::Float, false, 0);
    vertexFormats[VertexFormat::Type::XyStr].Create();

    // XyzStr
    vertexFormats[VertexFormat::Type::XyzStr].Append(0, 0, RHI::VertexElement::Usage::Position, 3, RHI::VertexElement::Type::Float, false, 0);
    vertexFormats[VertexFormat::Type::XyzStr].Append(0, 12, RHI::VertexElement::Usage::TexCoord0, 3, RHI::VertexElement::Type::Float, false, 0);
    vertexFormats[VertexFormat::Type::XyzStr].Create();

    // Xyz
    vertexFormats[VertexFormat::Type::Xyz].Append(0, 0, RHI::VertexElement::Usage::Position, 3, RHI::VertexElement::Type::Float, false, 0);
    vertexFormats[VertexFormat::Type::Xyz].Create();

    // XyzColor
    vertexFormats[VertexFormat::Type::XyzColor].Append(0, 0, RHI::VertexElement::Usage::Position, 3, RHI::VertexElement::Type::Float, false, 0);
    vertexFormats[VertexFormat::Type::XyzColor].Append(0, 12, RHI::VertexElement::Usage::Color, 4, RHI::VertexElement::Type::UByte, true, 0);
    vertexFormats[VertexFormat::Type::XyzColor].Create();

#ifdef COMPRESSED_VERTEX_NORMAL_TANGENTS
    RHI::VertexElement::Type::Enum normalTangentType = RHI::VertexElement::Type::UByte;
    bool normalTangentNormalize = true;
#else
    RHI::VertexElement::Type::Enum normalTangentType = RHI::VertexElement::Type::Half;
    bool normalTangentNormalize = false;
#endif

    // GenericXyz
    vertexFormats[VertexFormat::Type::GenericXyz].Append(0, OFFSET_OF(VertexGenericLit, xyz), RHI::VertexElement::Usage::Position, 3, RHI::VertexElement::Type::Float, false, 0);
    vertexFormats[VertexFormat::Type::GenericXyz].Create();

    // GenericXyzSkinning1, GenericXyzSkinning4, GenericXyzSkinning8
    CreateSkinningVertexFormats(1, VertexFormat::Type::GenericXyz, 
        VertexFormat::Type::GenericXyzSkinning1,
        VertexFormat::Type::GenericXyzSkinning4,
        VertexFormat::Type::GenericXyzSkinning8);

    // GenericXyzSt
    vertexFormats[VertexFormat::Type::GenericXyzSt].Append(0, OFFSET_OF(VertexGenericLit, xyz), RHI::VertexElement::Usage::Position, 3, RHI::VertexElement::Type::Float, false, 0);
    vertexFormats[VertexFormat::Type::GenericXyzSt].Append(0, OFFSET_OF(VertexGenericLit, st), RHI::VertexElement::Usage::TexCoord0, 2, RHI::VertexElement::Type::Half, false, 0);
    vertexFormats[VertexFormat::Type::GenericXyzSt].Create();

    // GenericXyzStSkinning1, GenericXyzStSkinning4, GenericXyzStSkinning8
    CreateSkinningVertexFormats(1, VertexFormat::Type::GenericXyzSt,
        VertexFormat::Type::GenericXyzStSkinning1, 
        VertexFormat::Type::GenericXyzStSkinning4, 
        VertexFormat::Type::GenericXyzStSkinning8);

    // GenericXyzStColor
    vertexFormats[VertexFormat::Type::GenericXyzStColor].Append(0, OFFSET_OF(VertexGenericLit, xyz), RHI::VertexElement::Usage::Position, 3, RHI::VertexElement::Type::Float, false, 0);
    vertexFormats[VertexFormat::Type::GenericXyzStColor].Append(0, OFFSET_OF(VertexGenericLit, st), RHI::VertexElement::Usage::TexCoord0, 2, RHI::VertexElement::Type::Half, false, 0);
    vertexFormats[VertexFormat::Type::GenericXyzStColor].Append(0, OFFSET_OF(VertexGenericLit, color), RHI::VertexElement::Usage::Color, 4, RHI::VertexElement::Type::UByte, true, 0);
    vertexFormats[VertexFormat::Type::GenericXyzStColor].Create();

    // GenericXyzStColorSkinning1, GenericXyzStColorSkinning4, GenericXyzStColorSkinning8
    CreateSkinningVertexFormats(1, VertexFormat::Type::GenericXyzStColor, 
        VertexFormat::Type::GenericXyzStColorSkinning1, 
        VertexFormat::Type::GenericXyzStColorSkinning4, 
        VertexFormat::Type::GenericXyzStColorSkinning8);

    // GenericXyzNormal
    vertexFormats[VertexFormat::Type::GenericXyzNormal].Append(0, OFFSET_OF(VertexGenericLit, xyz), RHI::VertexElement::Usage::Position, 3, RHI::VertexElement::Type::Float, false, 0);
    vertexFormats[VertexFormat::Type::GenericXyzNormal].Append(0, OFFSET_OF(VertexGenericLit, normal), RHI::VertexElement::Usage::Normal, 3, normalTangentType, normalTangentNormalize, 0);
    vertexFormats[VertexFormat::Type::GenericXyzNormal].Create();

    // GenericXyzNormalSkinning1, GenericXyzNormalSkinning4, GenericXyzNormalSkinning8
    CreateSkinningVertexFormats(1, VertexFormat::Type::GenericXyzNormal, 
        VertexFormat::Type::GenericXyzNormalSkinning1, 
        VertexFormat::Type::GenericXyzNormalSkinning4, 
        VertexFormat::Type::GenericXyzNormalSkinning8);

    // GenericXyzStNT
    vertexFormats[VertexFormat::Type::GenericXyzStNT].Append(0, OFFSET_OF(VertexGenericLit, xyz), RHI::VertexElement::Usage::Position, 3, RHI::VertexElement::Type::Float, false, 0);
    vertexFormats[VertexFormat::Type::GenericXyzStNT].Append(0, OFFSET_OF(VertexGenericLit, st), RHI::VertexElement::Usage::TexCoord, 2, RHI::VertexElement::Type::Half, false, 0);
    vertexFormats[VertexFormat::Type::GenericXyzStNT].Append(0, OFFSET_OF(VertexGenericLit, normal), RHI::VertexElement::Usage::Normal, 3, normalTangentType, normalTangentNormalize, 0);
    vertexFormats[VertexFormat::Type::GenericXyzStNT].Append(0, OFFSET_OF(VertexGenericLit, tangent), RHI::VertexElement::Usage::Tangent, 4, normalTangentType, normalTangentNormalize, 0);
    vertexFormats[VertexFormat::Type::GenericXyzStNT].Create();

    // GenericXyzStNTSkinning1, GenericXyzStNTSkinning4, GenericXyzStNTSkinning8
    CreateSkinningVertexFormats(1, VertexFormat::Type::GenericXyzStNT,
        VertexFormat::Type::GenericXyzStNTSkinning1, 
        VertexFormat::Type::GenericXyzStNTSkinning4, 
        VertexFormat::Type::GenericXyzStNTSkinning8);

    // GenericXyzStColorNT
    vertexFormats[VertexFormat::Type::GenericXyzStColorNT].Append(0, OFFSET_OF(VertexGenericLit, xyz), RHI::VertexElement::Usage::Position, 3, RHI::VertexElement::Type::Float, false, 0);
    vertexFormats[VertexFormat::Type::GenericXyzStColorNT].Append(0, OFFSET_OF(VertexGenericLit, st), RHI::VertexElement::Usage::TexCoord0, 2, RHI::VertexElement::Type::Half, false, 0);
    vertexFormats[VertexFormat::Type::GenericXyzStColorNT].Append(0, OFFSET_OF(VertexGenericLit, color), RHI::VertexElement::Usage::Color, 4, RHI::VertexElement::Type::UByte, true, 0);
    vertexFormats[VertexFormat::Type::GenericXyzStColorNT].Append(0, OFFSET_OF(VertexGenericLit, normal), RHI::VertexElement::Usage::Normal, 3, normalTangentType, normalTangentNormalize, 0);
    vertexFormats[VertexFormat::Type::GenericXyzStColorNT].Append(0, OFFSET_OF(VertexGenericLit, tangent), RHI::VertexElement::Usage::Tangent, 4, normalTangentType, normalTangentNormalize, 0);
    vertexFormats[VertexFormat::Type::GenericXyzStColorNT].Create();

    // GenericXyzStColorNTSkinning1, GenericXyzStColorNTSkinning4, GenericXyzStColorNTSkinning8
    CreateSkinningVertexFormats(1, VertexFormat::Type::GenericXyzStColorNT,
        VertexFormat::Type::GenericXyzStColorNTSkinning1, 
        VertexFormat::Type::GenericXyzStColorNTSkinning4, 
        VertexFormat::Type::GenericXyzStColorNTSkinning8);

    if (renderGlobal.instancingMethod == Mesh::InstancingMethod::InstancedArrays) {
        CreateInstancingVertexFormats(1, VertexFormat::Type::GenericXyz, VertexFormat::Type::GenericXyzInstancing, false);
        CreateInstancingVertexFormats(2, VertexFormat::Type::GenericXyzSkinning1, VertexFormat::Type::GenericXyzInstancingSkinning1, true);
        CreateInstancingVertexFormats(2, VertexFormat::Type::GenericXyzSkinning4, VertexFormat::Type::GenericXyzInstancingSkinning4, true);
        CreateInstancingVertexFormats(2, VertexFormat::Type::GenericXyzSkinning8, VertexFormat::Type::GenericXyzInstancingSkinning8, true);

        CreateInstancingVertexFormats(1, VertexFormat::Type::GenericXyzSt, VertexFormat::Type::GenericXyzStInstancing, false);
        CreateInstancingVertexFormats(2, VertexFormat::Type::GenericXyzStSkinning1, VertexFormat::Type::GenericXyzStInstancingSkinning1, true);
        CreateInstancingVertexFormats(2, VertexFormat::Type::GenericXyzStSkinning4, VertexFormat::Type::GenericXyzStInstancingSkinning4, true);
        CreateInstancingVertexFormats(2, VertexFormat::Type::GenericXyzStSkinning8, VertexFormat::Type::GenericXyzStInstancingSkinning8, true);

        CreateInstancingVertexFormats(1, VertexFormat::Type::GenericXyzStColor, VertexFormat::Type::GenericXyzStColorInstancing, false);
        CreateInstancingVertexFormats(2, VertexFormat::Type::GenericXyzStColorSkinning1, VertexFormat::Type::GenericXyzStColorInstancingSkinning1, true);
        CreateInstancingVertexFormats(2, VertexFormat::Type::GenericXyzStColorSkinning4, VertexFormat::Type::GenericXyzStColorInstancingSkinning4, true);
        CreateInstancingVertexFormats(2, VertexFormat::Type::GenericXyzStColorSkinning8, VertexFormat::Type::GenericXyzStColorInstancingSkinning8, true);

        CreateInstancingVertexFormats(1, VertexFormat::Type::GenericXyzNormal, VertexFormat::Type::GenericXyzNormalInstancing, false);
        CreateInstancingVertexFormats(2, VertexFormat::Type::GenericXyzNormalSkinning1, VertexFormat::Type::GenericXyzNormalInstancingSkinning1, true);
        CreateInstancingVertexFormats(2, VertexFormat::Type::GenericXyzNormalSkinning4, VertexFormat::Type::GenericXyzNormalInstancingSkinning4, true);
        CreateInstancingVertexFormats(2, VertexFormat::Type::GenericXyzNormalSkinning8, VertexFormat::Type::GenericXyzNormalInstancingSkinning8, true);

        CreateInstancingVertexFormats(1, VertexFormat::Type::GenericXyzStNT, VertexFormat::Type::GenericXyzStNTInstancing, false);
        CreateInstancingVertexFormats(2, VertexFormat::Type::GenericXyzStNTSkinning1, VertexFormat::Type::GenericXyzStNTInstancingSkinning1, true);
        CreateInstancingVertexFormats(2, VertexFormat::Type::GenericXyzStNTSkinning4, VertexFormat::Type::GenericXyzStNTInstancingSkinning4, true);
        CreateInstancingVertexFormats(2, VertexFormat::Type::GenericXyzStNTSkinning8, VertexFormat::Type::GenericXyzStNTInstancingSkinning8, true);

        CreateInstancingVertexFormats(1, VertexFormat::Type::GenericXyzStColorNT, VertexFormat::Type::GenericXyzStColorNTInstancing, false);
        CreateInstancingVertexFormats(2, VertexFormat::Type::GenericXyzStColorNTSkinning1, VertexFormat::Type::GenericXyzStColorNTInstancingSkinning1, true);
        CreateInstancingVertexFormats(2, VertexFormat::Type::GenericXyzStColorNTSkinning4, VertexFormat::Type::GenericXyzStColorNTInstancingSkinning4, true);
        CreateInstancingVertexFormats(2, VertexFormat::Type::GenericXyzStColorNTSkinning8, VertexFormat::Type::GenericXyzStColorNTInstancingSkinning8, true);
    }

    // Occludee
    vertexFormats[VertexFormat::Type::Occludee].Append(0, 0, RHI::VertexElement::Usage::Position, 2, RHI::VertexElement::Type::Float, false, 0);
    vertexFormats[VertexFormat::Type::Occludee].Append(0, 8, RHI::VertexElement::Usage::TexCoord0, 3, RHI::VertexElement::Type::Float, false, 0);
    vertexFormats[VertexFormat::Type::Occludee].Append(0, 20, RHI::VertexElement::Usage::TexCoord1, 3, RHI::VertexElement::Type::Float, false, 0);
    vertexFormats[VertexFormat::Type::Occludee].Create();
}

void VertexFormat::CreateSkinningVertexFormats(int stream, int originalIndex, int skinning1Index, int skinning4Index, int skinning8Index) {
    RHI::VertexElement::Type::Enum weightType;

    if (sizeof(JointWeightType) == sizeof(byte)) {
        weightType = RHI::VertexElement::Type::UByte;
    } else {
        weightType = RHI::VertexElement::Type::Float;
    }

    vertexFormats[skinning1Index].CopyFrom(vertexFormats[originalIndex]);
    vertexFormats[skinning1Index].Append(stream, OFFSET_OF(VertexWeight1, jointIndex), RHI::VertexElement::Usage::WeightIndex, 1, RHI::VertexElement::Type::UInt, false, 0);
    vertexFormats[skinning1Index].Create();

    vertexFormats[skinning4Index].CopyFrom(vertexFormats[originalIndex]);
    vertexFormats[skinning4Index].Append(stream, OFFSET_OF(VertexWeight4, jointIndexes[0]), RHI::VertexElement::Usage::WeightIndex, 4, RHI::VertexElement::Type::UByte, false, 0);
    vertexFormats[skinning4Index].Append(stream, OFFSET_OF(VertexWeight4, jointWeights[0]), RHI::VertexElement::Usage::WeightValue, 4, weightType, true, 0);
    vertexFormats[skinning4Index].Create();

    vertexFormats[skinning8Index].CopyFrom(vertexFormats[originalIndex]);
    vertexFormats[skinning8Index].Append(stream, OFFSET_OF(VertexWeight8, jointIndexes[0]), RHI::VertexElement::Usage::WeightIndex0, 4, RHI::VertexElement::Type::UByte, false, 0);
    vertexFormats[skinning8Index].Append(stream, OFFSET_OF(VertexWeight8, jointIndexes[4]), RHI::VertexElement::Usage::WeightIndex1, 4, RHI::VertexElement::Type::UByte, false, 0);
    vertexFormats[skinning8Index].Append(stream, OFFSET_OF(VertexWeight8, jointWeights[0]), RHI::VertexElement::Usage::WeightValue0, 4, weightType, true, 0);
    vertexFormats[skinning8Index].Append(stream, OFFSET_OF(VertexWeight8, jointWeights[4]), RHI::VertexElement::Usage::WeightValue1, 4, weightType, true, 0);
    vertexFormats[skinning8Index].Create();
}

// Create instanced array vertex format.
void VertexFormat::CreateInstancingVertexFormats(int stream, int originalIndex, int instancingIndex, bool useVtfSkinning) {
    vertexFormats[instancingIndex].CopyFrom(vertexFormats[originalIndex]);
    vertexFormats[instancingIndex].Append(stream,  0, RHI::VertexElement::Usage::TexCoord1, 12, RHI::VertexElement::Type::Float, false, 1);  // localToWorldMatrix
    vertexFormats[instancingIndex].Append(stream, 48, RHI::VertexElement::Usage::TexCoord4, 4, RHI::VertexElement::Type::UByte, true, 1);    // entityColor

    if (useVtfSkinning) {
        if (renderGlobal.vertexTextureMethod == BufferCacheManager::VertexTextureMethod::Tbo) {
            vertexFormats[instancingIndex].Append(stream, 52, RHI::VertexElement::Usage::TexCoord5, 1, RHI::VertexElement::Type::UInt, false, 1);
        } else {
            vertexFormats[instancingIndex].Append(stream, 52, RHI::VertexElement::Usage::TexCoord5, 2, RHI::VertexElement::Type::Float, false, 1);
        }
    }

    vertexFormats[instancingIndex].Create();
}

void VertexFormat::Shutdown() {
    for (int i = 0; i < COUNT_OF(vertexFormats); i++) {
        vertexFormats[i].Delete();
    }
}

void VertexFormat::Append(int stream, int offset, RHI::VertexElement::Usage::Enum usage, int components, RHI::VertexElement::Type::Enum type, bool normalize, int divisor) {
    RHI::VertexElement element;
    element.stream      = stream;
    element.offset      = offset;
    element.usage       = usage;
    element.components  = components;
    element.type        = type;
    element.normalize   = normalize;
    element.divisor     = divisor;

    elements.Append(element);

    int typeSize;
    switch (type) {
    case RHI::VertexElement::Type::UByte:
        typeSize = 1; 
        break;
    case RHI::VertexElement::Type::Half:
        typeSize = 2;
        break;
    case RHI::VertexElement::Type::UInt:
    case RHI::VertexElement::Type::Float:
        typeSize = 4; 
        break;
    default: 
        assert(0); 
        break;
    }
}

void VertexFormat::CopyFrom(const VertexFormat &other) {
    elements = other.elements;
}

void VertexFormat::Create() {
    vertexFormatHandle = rhi.CreateVertexFormat(elements.Count(), elements.Ptr());
}

void VertexFormat::Delete() {
    if (vertexFormatHandle != RHI::NullVertexFormat) {
        rhi.DestroyVertexFormat(vertexFormatHandle);
        vertexFormatHandle = RHI::NullVertexFormat;
    }
}

BE_NAMESPACE_END

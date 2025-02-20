in LOWP uvec4 in_weightIndex0 : WEIGHT_INDEX0;
in LOWP uvec4 in_weightIndex1 : WEIGHT_INDEX1;
in MEDIUMP vec4 in_weightValue0 : WEIGHT_VALUE0;
in MEDIUMP vec4 in_weightValue1 : WEIGHT_VALUE1;

#ifdef VTF_SKINNING
	#ifdef USE_BUFFER_TEXTURE
		uniform samplerBuffer jointsMap;
	#else
		uniform sampler2D jointsMap;
        uniform HIGHP vec2 invJointsMapSize;
	#endif

	#ifndef INSTANCING
		uniform VTF_SKINNING_TC_TYPE skinningBaseTc;
	#endif
#else
    uniform HIGHP vec4 joints[MAX_SHADER_JOINTSX3];   // 4x3 matrix
#endif

void accumulateJointMatrices(out vec4 R0, out vec4 R1, out vec4 R2, int jointIndexOffset) {
	float invNorm = 1.0 / (dot(in_weightValue0, vec4(1.0)) + dot(in_weightValue1, vec4(1.0)));
	vec4 w0 = in_weightValue0 * invNorm;
	vec4 w1 = in_weightValue1 * invNorm;

#ifdef VTF_SKINNING
	#ifdef USE_BUFFER_TEXTURE
		int baseS = skinningBaseTc + (jointIndexOffset + int(in_weightIndex0.x)) * 3;
		R0 = w0.x * texelFetch(jointsMap, baseS + 0);
		R1 = w0.x * texelFetch(jointsMap, baseS + 1);
		R2 = w0.x * texelFetch(jointsMap, baseS + 2);

		if (w0.y > 0.0) {
			baseS = skinningBaseTc + (jointIndexOffset + int(in_weightIndex0.y)) * 3;
			R0 += w0.y * texelFetch(jointsMap, baseS + 0);
			R1 += w0.y * texelFetch(jointsMap, baseS + 1);
			R2 += w0.y * texelFetch(jointsMap, baseS + 2);

			if (w0.z > 0.0) {
				baseS = skinningBaseTc + (jointIndexOffset + int(in_weightIndex0.z)) * 3;
				R0 += w0.z * texelFetch(jointsMap, baseS + 0);
				R1 += w0.z * texelFetch(jointsMap, baseS + 1);
				R2 += w0.z * texelFetch(jointsMap, baseS + 2);

				if (w0.w > 0.0) {
					baseS = skinningBaseTc + (jointIndexOffset + int(in_weightIndex0.w)) * 3;
					R0 += w0.w * texelFetch(jointsMap, baseS + 0);
					R1 += w0.w * texelFetch(jointsMap, baseS + 1);
					R2 += w0.w * texelFetch(jointsMap, baseS + 2);

					if (w1.x > 0.0) {
						baseS = skinningBaseTc + (jointIndexOffset + int(in_weightIndex1.x)) * 3;
						R0 += w1.x * texelFetch(jointsMap, baseS + 0);
						R1 += w1.x * texelFetch(jointsMap, baseS + 1);
						R2 += w1.x * texelFetch(jointsMap, baseS + 2);

						if (w1.y > 0.0) {
							baseS = skinningBaseTc + (jointIndexOffset + int(in_weightIndex1.y)) * 3;
							R0 += w1.y * texelFetch(jointsMap, baseS + 0);
							R1 += w1.y * texelFetch(jointsMap, baseS + 1);
							R2 += w1.y * texelFetch(jointsMap, baseS + 2);

							if (w1.z > 0.0) {
								baseS = skinningBaseTc + (jointIndexOffset + int(in_weightIndex1.z)) * 3;
								R0 += w1.z * texelFetch(jointsMap, baseS + 0);
								R1 += w1.z * texelFetch(jointsMap, baseS + 1);
								R2 += w1.z * texelFetch(jointsMap, baseS + 2);

								if (w1.w > 0.0) {
									baseS = skinningBaseTc + (jointIndexOffset + int(in_weightIndex1.w)) * 3;
									R0 += w1.w * texelFetch(jointsMap, baseS + 0);
									R1 += w1.w * texelFetch(jointsMap, baseS + 1);
									R2 += w1.w * texelFetch(jointsMap, baseS + 2);
								}
							}
						}
					}
				}
			}
		}
	#else
		vec2 baseST = skinningBaseTc + vec2((float(jointIndexOffset) + float(in_weightIndex0.x)) * 3.0, 0.0);
		R0 = w0.x * tex2Dlod(jointsMap, vec4((baseST + vec2(0.0, 0.0)) * invJointsMapSize, 0.0, 0.0));
		R1 = w0.x * tex2Dlod(jointsMap, vec4((baseST + vec2(1.0, 0.0)) * invJointsMapSize, 0.0, 0.0));
		R2 = w0.x * tex2Dlod(jointsMap, vec4((baseST + vec2(2.0, 0.0)) * invJointsMapSize, 0.0, 0.0));

		if (w0.y > 0.0) {
			baseST = skinningBaseTc + vec2((float(jointIndexOffset) + float(in_weightIndex0.y)) * 3.0, 0.0);
			R0 += w0.y * tex2Dlod(jointsMap, vec4((skinningBaseTc + vec2(0.0, 0.0)) * invJointsMapSize, 0.0, 0.0));
			R1 += w0.y * tex2Dlod(jointsMap, vec4((skinningBaseTc + vec2(1.0, 0.0)) * invJointsMapSize, 0.0, 0.0));
			R2 += w0.y * tex2Dlod(jointsMap, vec4((skinningBaseTc + vec2(2.0, 0.0)) * invJointsMapSize, 0.0, 0.0));

			if (w0.z > 0.0) {
				baseST = skinningBaseTc + vec2((float(jointIndexOffset) + float(in_weightIndex0.z)) * 3.0, 0.0);
				R0 += w0.z * tex2Dlod(jointsMap, vec4((skinningBaseTc + vec2(0.0, 0.0)) * invJointsMapSize, 0.0, 0.0));
				R1 += w0.z * tex2Dlod(jointsMap, vec4((skinningBaseTc + vec2(1.0, 0.0)) * invJointsMapSize, 0.0, 0.0));
				R2 += w0.z * tex2Dlod(jointsMap, vec4((skinningBaseTc + vec2(2.0, 0.0)) * invJointsMapSize, 0.0, 0.0));

				if (w0.w > 0.0) {
					baseST = skinningBaseTc + vec2((float(jointIndexOffset) + float(in_weightIndex0.w)) * 3.0, 0.0);
					R0 += w0.w * tex2Dlod(jointsMap, vec4((skinningBaseTc + vec2(0.0, 0.0)) * invJointsMapSize, 0.0, 0.0));
					R1 += w0.w * tex2Dlod(jointsMap, vec4((skinningBaseTc + vec2(1.0, 0.0)) * invJointsMapSize, 0.0, 0.0));
					R2 += w0.w * tex2Dlod(jointsMap, vec4((skinningBaseTc + vec2(2.0, 0.0)) * invJointsMapSize, 0.0, 0.0));

					if (w1.x > 0.0) {
						baseST = skinningBaseTc + vec2((float(jointIndexOffset) + float(in_weightIndex1.x)) * 3.0, 0.0);
						R0 += w1.x * tex2Dlod(jointsMap, vec4((skinningBaseTc + vec2(0.0, 0.0)) * invJointsMapSize, 0.0, 0.0));
						R1 += w1.x * tex2Dlod(jointsMap, vec4((skinningBaseTc + vec2(1.0, 0.0)) * invJointsMapSize, 0.0, 0.0));
						R2 += w1.x * tex2Dlod(jointsMap, vec4((skinningBaseTc + vec2(2.0, 0.0)) * invJointsMapSize, 0.0, 0.0));

						if (w1.y > 0.0) {
							baseST = skinningBaseTc + vec2((float(jointIndexOffset) + float(in_weightIndex1.y)) * 3.0, 0.0);
							R0 += w1.y * tex2Dlod(jointsMap, vec4((skinningBaseTc + vec2(0.0, 0.0)) * invJointsMapSize, 0.0, 0.0));
							R1 += w1.y * tex2Dlod(jointsMap, vec4((skinningBaseTc + vec2(1.0, 0.0)) * invJointsMapSize, 0.0, 0.0));
							R2 += w1.y * tex2Dlod(jointsMap, vec4((skinningBaseTc + vec2(2.0, 0.0)) * invJointsMapSize, 0.0, 0.0));

							if (w1.z > 0.0) {
								baseST = skinningBaseTc + vec2((float(jointIndexOffset) + float(in_weightIndex1.z)) * 3.0, 0.0);
								R0 += w1.z * tex2Dlod(jointsMap, vec4((skinningBaseTc + vec2(0.0, 0.0)) * invJointsMapSize, 0.0, 0.0));
								R1 += w1.z * tex2Dlod(jointsMap, vec4((skinningBaseTc + vec2(1.0, 0.0)) * invJointsMapSize, 0.0, 0.0));
								R2 += w1.z * tex2Dlod(jointsMap, vec4((skinningBaseTc + vec2(2.0, 0.0)) * invJointsMapSize, 0.0, 0.0));

								if (w1.w > 0.0) {
									baseST = skinningBaseTc + vec2((float(jointIndexOffset) + float(in_weightIndex1.w)) * 3.0, 0.0);
									R0 += w1.w * tex2Dlod(jointsMap, vec4((skinningBaseTc + vec2(0.0, 0.0)) * invJointsMapSize, 0.0, 0.0));
									R1 += w1.w * tex2Dlod(jointsMap, vec4((skinningBaseTc + vec2(1.0, 0.0)) * invJointsMapSize, 0.0, 0.0));
									R2 += w1.w * tex2Dlod(jointsMap, vec4((skinningBaseTc + vec2(2.0, 0.0)) * invJointsMapSize, 0.0, 0.0));
								}
							}
						}
					}
				}
			}
		}
	#endif
#else
	int weightIndex = (jointIndexOffset + int(in_weightIndex0.x)) * 3;
	R0 = w0.x * joints[weightIndex + 0];
	R1 = w0.x * joints[weightIndex + 1];
	R2 = w0.x * joints[weightIndex + 2];

	weightIndex = (jointIndexOffset + int(in_weightIndex0.y)) * 3;
	R0 += w0.y * joints[weightIndex + 0];
	R1 += w0.y * joints[weightIndex + 1];
	R2 += w0.y * joints[weightIndex + 2];

	weightIndex = (jointIndexOffset + int(in_weightIndex0.z)) * 3;
	R0 += w0.z * joints[weightIndex + 0];
	R1 += w0.z * joints[weightIndex + 1];
	R2 += w0.z * joints[weightIndex + 2];

	weightIndex = (jointIndexOffset + int(in_weightIndex0.w)) * 3;
	R0 += w0.w * joints[weightIndex + 0];
	R1 += w0.w * joints[weightIndex + 1];
	R2 += w0.w * joints[weightIndex + 2];

	weightIndex = (jointIndexOffset + int(in_weightIndex1.x)) * 3;
	R0 += w1.x * joints[weightIndex + 0];
	R1 += w1.x * joints[weightIndex + 1];
	R2 += w1.x * joints[weightIndex + 2];

	weightIndex = (jointIndexOffset + int(in_weightIndex1.y)) * 3;
	R0 += w1.y * joints[weightIndex + 0];
	R1 += w1.y * joints[weightIndex + 1];
	R2 += w1.y * joints[weightIndex + 2];

	weightIndex = (jointIndexOffset + int(in_weightIndex1.z)) * 3;
	R0 += w1.z * joints[weightIndex + 0];
	R1 += w1.z * joints[weightIndex + 1];
	R2 += w1.z * joints[weightIndex + 2];

	weightIndex = (jointIndexOffset + int(in_weightIndex1.w)) * 3;
	R0 += w1.w * joints[weightIndex + 0];
	R1 += w1.w * joints[weightIndex + 1];
	R2 += w1.w * joints[weightIndex + 2];
#endif
}

$include "SkinningMatrixCommon.glsl"

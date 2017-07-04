#include "pch.h"
#include "muMath.h"
#include "muSIMD.h"
#include "muRawVector.h"

namespace mu {

#ifdef muEnableHalf
void FloatToHalf_Generic(half *dst, const float *src, size_t num)
{
    for (size_t i = 0; i < num; ++i) {
        dst[i] = src[i];
    }
}
void HalfToFloat_Generic(float *dst, const half *src, size_t num)
{
    for (size_t i = 0; i < num; ++i) {
        dst[i] = src[i];
    }
}
#endif // muEnableHalf

void InvertX_Generic(float3 *dst, size_t num)
{
    for (size_t i = 0; i < num; ++i) {
        dst[i].x *= -1.0f;
    }
}
void InvertX_Generic(float4 *dst, size_t num)
{
    for (size_t i = 0; i < num; ++i) {
        dst[i].x *= -1.0f;
    }
}

void InvertV(float2 *dst, size_t num)
{
    for (size_t i = 0; i < num; ++i) {
        dst[i].y = 1.0f - dst[i].y;
    }
}


void Scale_Generic(float *dst, float s, size_t num)
{
    for (size_t i = 0; i < num; ++i) {
        dst[i] *= s;
    }
}
void Scale_Generic(float3 *dst, float s, size_t num)
{
    for (size_t i = 0; i < num; ++i) {
        dst[i] *= s;
    }
}

void Normalize_Generic(float3 *dst, size_t num)
{
    for (size_t i = 0; i < num; ++i) {
        dst[i] = normalize(dst[i]);
    }
}

void Lerp_Generic(float *dst, const float *src1, const float *src2, size_t num, float w)
{
    const float iw = 1.0f - w;
    for (size_t i = 0; i < num; ++i) {
        dst[i] = src1[i] * w + src2[i] * iw;
    }
}

void MinMax_Generic(const float2 *src, size_t num, float2& dst_min, float2& dst_max)
{
    if (num == 0) { return; }
    float2 rmin = src[0];
    float2 rmax = src[0];
    for (size_t i = 1; i < num; ++i) {
        rmin = min(rmin, src[i]);
        rmax = max(rmax, src[i]);
    }
    dst_min = rmin;
    dst_max = rmax;
}

void MinMax_Generic(const float3 *src, size_t num, float3& dst_min, float3& dst_max)
{
    if (num == 0) { return; }
    float3 rmin = src[0];
    float3 rmax = src[0];
    for (size_t i = 1; i < num; ++i) {
        rmin = min(rmin, src[i]);
        rmax = max(rmax, src[i]);
    }
    dst_min = rmin;
    dst_max = rmax;
}

bool NearEqual_Generic(const float *src1, const float *src2, size_t num, float eps)
{
    for (size_t i = 0; i < num; ++i) {
        if (!near_equal(src1[i], src2[i], eps)) {
            return false;
        }
    }
    return true;
}

void MulPoints_Generic(const float4x4& m, const float3 src[], float3 dst[], size_t num_data)
{
    for (int i = 0; i < num_data; ++i) {
        dst[i] = mul_p(m, src[i]);
    }
}
void MulVectors_Generic(const float4x4& m, const float3 src[], float3 dst[], size_t num_data)
{
    for (int i = 0; i < num_data; ++i) {
        dst[i] = mul_v(m, src[i]);
    }
}

int RayTrianglesIntersectionIndexed_Generic(float3 pos, float3 dir, const float3 *vertices, const int *indices, int num_triangles, int& tindex, float& distance)
{
    int num_hits = 0;
    distance = FLT_MAX;

    for (int i = 0; i < num_triangles; ++i) {
        float d;
        if (ray_triangle_intersection(pos, dir, vertices[indices[i * 3 + 0]], vertices[indices[i * 3 + 1]], vertices[indices[i * 3 + 2]], d)) {
            ++num_hits;
            if (d < distance) {
                distance = d;
                tindex = i;
            }
        }
    }
    return num_hits;
}
int RayTrianglesIntersectionArray_Generic(float3 pos, float3 dir, const float3 *vertices, int num_triangles, int& tindex, float& distance)
{
    int num_hits = 0;
    distance = FLT_MAX;

    for (int i = 0; i < num_triangles; ++i) {
        float d;
        if (ray_triangle_intersection(pos, dir, vertices[i * 3 + 0], vertices[i * 3 + 1], vertices[i * 3 + 2], d)) {
            ++num_hits;
            if (d < distance) {
                distance = d;
                tindex = i;
            }
        }
    }
    return num_hits;
}

int RayTrianglesIntersectionSoA_Generic(float3 pos, float3 dir,
    const float *v1x, const float *v1y, const float *v1z,
    const float *v2x, const float *v2y, const float *v2z,
    const float *v3x, const float *v3y, const float *v3z,
    int num_triangles, int& tindex, float& distance)
{
    int num_hits = 0;
    distance = FLT_MAX;

    for (int i = 0; i < num_triangles; ++i) {
        float d;
        if (ray_triangle_intersection(pos, dir,
            { v1x[i], v1y[i], v1z[i] },
            { v2x[i], v2y[i], v2z[i] },
            { v3x[i], v3y[i], v3z[i] }, d))
        {
            ++num_hits;
            if (d < distance) {
                distance = d;
                tindex = i;
            }
        }
    }
    return num_hits;
}


bool PolyInside_Generic(const float px[], const float py[], int ngon, const float2 minp, const float2 maxp, const float2 pos)
{
    return poly_inside(px, py, ngon, minp, maxp, pos);
}

bool PolyInside_Generic(const float2 poly[], int ngon, const float2 minp, const float2 maxp, const float2 pos)
{
    return poly_inside(poly, ngon, minp, maxp, pos);
}

bool PolyInside_Generic(const float2 poly[], int ngon, const float2 pos)
{
    float2 minp, maxp;
    poly_minmax(poly, ngon, minp, maxp);
    return poly_inside(poly, ngon, minp, maxp, pos);
}

void GenerateTangentsIndexed_Generic(float4 *dst,
    const float3 *vertices, const float3 *normals, const float2 *uv, const int *indices, int num_triangles, int num_vertices)
{
    generate_tangents(dst, vertices, uv, normals, indices, num_triangles, num_vertices);
}
void GenerateTangentsSoA_Generic(float4 *dst,
    const float *v1x, const float *v1y, const float *v1z,
    const float *v2x, const float *v2y, const float *v2z,
    const float *v3x, const float *v3y, const float *v3z,
    const float *u1x, const float *u1y,
    const float *u2x, const float *u2y,
    const float *u3x, const float *u3y,
    const float3 *normals,
    const int *indices, int num_triangles, int num_vertices)
{
    generate_tangents_soa(dst,
        v1x, v1y, v1z, v2x, v2y, v2z, v3x, v3y, v3z,
        u1x, u1y, u2x, u2y, u3x, u3y,
        normals, indices, num_triangles, num_vertices);
}


#ifdef muEnableISPC
#include "MeshUtilsCore.h"
#include "MeshUtilsCore2.h"

#ifdef muEnableHalf
void FloatToHalf_ISPC(half *dst, const float *src, size_t num)
{
    ispc::FloatToHalf((uint16_t*)dst, src, (int)num);
}
void HalfToFloat_ISPC(float *dst, const half *src, size_t num)
{
    ispc::HalfToFloat(dst, (const uint16_t*)src, (int)num);
}
#endif // muEnableHalf

void InvertX_ISPC(float3 *dst, size_t num)
{
    ispc::InvertXF3((ispc::float3*)dst, (int)num);
}
void InvertX_ISPC(float4 *dst, size_t num)
{
    ispc::InvertXF4((ispc::float4*)dst, (int)num);
}

void Scale_ISPC(float *dst, float s, size_t num)
{
    ispc::ScaleF((float*)dst, s, (int)num * 1);
}
void Scale_ISPC(float3 *dst, float s, size_t num)
{
    ispc::ScaleF((float*)dst, s, (int)num * 3);
}

void Normalize_ISPC(float3 *dst, size_t num)
{
    ispc::Normalize((ispc::float3*)dst, (int)num);
}

void GenerateNormals_ISPC(
    float3 *dst, const float3 *p,
    const int *counts, const int *offsets, const int *indices, size_t num_points, size_t num_faces)
{
    memset(dst, 0, sizeof(float3)*num_points);

    for (size_t fi = 0; fi < num_faces; ++fi)
    {
        int count = counts[fi];
        const int *face = &indices[offsets[fi]];
        int i0 = face[0];
        int i1 = face[1];
        int i2 = face[2];
        float3 p0 = p[i0];
        float3 p1 = p[i1];
        float3 p2 = p[i2];
        float3 n = cross(p1 - p0, p2 - p0);
        for (int ci = 0; ci < count; ++ci) {
            dst[face[ci]] += n;
        }
    }

    ispc::Normalize((ispc::float3*)dst, (int)num_points);
}

void Lerp_ISPC(float *dst, const float *src1, const float *src2, size_t num, float w)
{
    ispc::Lerp(dst, src1, src2, (int)num, w);
}

void MinMax_ISPC(const float2 *src, size_t num, float2& dst_min, float2& dst_max)
{
    if (num == 0) { return; }
    ispc::MinMax2((ispc::float2*)src, (int)num, (ispc::float2&)dst_min, (ispc::float2&)dst_max);
}
void MinMax_ISPC(const float3 *src, size_t num, float3& dst_min, float3& dst_max)
{
    if (num == 0) { return; }
    ispc::MinMax3((ispc::float3*)src, (int)num, (ispc::float3&)dst_min, (ispc::float3&)dst_max);
}

bool NearEqual_ISPC(const float *src1, const float *src2, size_t num, float eps)
{
    return ispc::NearEqual(src1, src2, (int)num, eps);
}

void MulPoints_ISPC(const float4x4& m, const float3 src[], float3 dst[], size_t num_data)
{
    ispc::MulPoints3((ispc::float4x4&)m, (ispc::float3*)src, (ispc::float3*)dst, (int)num_data);
}
void MulVectors_ISPC(const float4x4& m, const float3 src[], float3 dst[], size_t num_data)
{
    ispc::MulVectors3((ispc::float4x4&)m, (ispc::float3*)src, (ispc::float3*)dst, (int)num_data);
}

int RayTrianglesIntersectionIndexed_ISPC(
    float3 pos, float3 dir, const float3 *vertices, const int *indices, int num_triangles, int& tindex, float& distance)
{
    return ispc::RayTrianglesIntersectionIndexed(
        (ispc::float3&)pos, (ispc::float3&)dir, (ispc::float3*)vertices, indices, num_triangles, tindex, distance);
}
int RayTrianglesIntersectionArray_ISPC(
    float3 pos, float3 dir, const float3 *vertices, int num_triangles, int& tindex, float& distance)
{
    return ispc::RayTrianglesIntersectionArray(
        (ispc::float3&)pos, (ispc::float3&)dir, (ispc::float3*)vertices, num_triangles, tindex, distance);
}
int RayTrianglesIntersectionSoA_ISPC(float3 pos, float3 dir,
    const float *v1x, const float *v1y, const float *v1z,
    const float *v2x, const float *v2y, const float *v2z,
    const float *v3x, const float *v3y, const float *v3z,
    int num_triangles, int& tindex, float& distance)
{
    return ispc::RayTrianglesIntersectionSoA(
        (ispc::float3&)pos, (ispc::float3&)dir, v1x, v1y, v1z, v2x, v2y, v2z, v3x, v3y, v3z, num_triangles, tindex, distance);
}


bool PolyInside_ISPC(const float px[], const float py[], int ngon, const float2 minp, const float2 maxp, const float2 pos)
{
    const int MaxXC = 64;
    float xc[MaxXC];

    int c = ispc::PolyInsideSoAImpl(px, py, ngon, (ispc::float2&)minp, (ispc::float2&)maxp, (ispc::float2&)pos, xc, MaxXC);
    std::sort(xc, xc + c);
    for (int i = 0; i < c; i += 2) {
        if (pos.x >= xc[i] && pos.x < xc[i + 1]) {
            return true;
        }
    }
    return false;
}

bool PolyInside_ISPC(const float2 poly[], int ngon, const float2 minp, const float2 maxp, const float2 pos)
{
    const int MaxXC = 64;
    float xc[MaxXC];

    int c = ispc::PolyInsideImpl((ispc::float2*)poly, ngon, (ispc::float2&)minp, (ispc::float2&)maxp, (ispc::float2&)pos, xc, MaxXC);
    std::sort(xc, xc + c);
    for (int i = 0; i < c; i += 2) {
        if (pos.x >= xc[i] && pos.x < xc[i + 1]) {
            return true;
        }
    }
    return false;
}

bool PolyInside_ISPC(const float2 poly[], int ngon, const float2 pos)
{
    float2 minp, maxp;
    MinMax(poly, ngon, minp, maxp);
    return PolyInside_ISPC(poly, ngon, minp, maxp, pos);
}

void GenerateTangentsIndexed_ISPC(float4 *dst,
    const float3 *vertices, const float3 *normals, const float2 *uv, const int *indices, int num_triangles, int num_vertices)
{
    // make soa-nized data
    RawVector<float> vsoa[9], usoa[6];
    for (auto& v : vsoa) { v.resize(num_triangles); }
    for (auto& v : usoa) { v.resize(num_triangles); }
    for (int ti = 0; ti < num_triangles; ++ti) {
        float3 v[] = {
            vertices[indices[ti * 3 + 0]],
            vertices[indices[ti * 3 + 1]],
            vertices[indices[ti * 3 + 2]],
        };
        float2 u[] = {
            uv[indices[ti * 3 + 0]],
            uv[indices[ti * 3 + 1]],
            uv[indices[ti * 3 + 2]],
        };
        for (int i = 0; i < 9; ++i) { vsoa[i][ti] = ((float*)v)[i]; }
        for (int i = 0; i < 6; ++i) { usoa[i][ti] = ((float*)u)[i]; }
    }

    RawVector<float3> tmp_tangents, tmp_binormals;
    tmp_tangents.resize_with_zeroclear(num_vertices);
    tmp_binormals.resize_with_zeroclear(num_vertices);

    ispc::GenerateTangentsSoA(
        (ispc::float4*)dst,
        vsoa[0].data(), vsoa[1].data(), vsoa[2].data(),
        vsoa[3].data(), vsoa[4].data(), vsoa[5].data(),
        vsoa[6].data(), vsoa[7].data(), vsoa[8].data(),

        usoa[0].data(), usoa[1].data(),
        usoa[2].data(), usoa[3].data(),
        usoa[4].data(), usoa[5].data(),

        (ispc::float3*)normals,
        num_triangles, num_vertices,
        indices,
        (ispc::float3*)tmp_tangents.data(),
        (ispc::float3*)tmp_binormals.data());
}

void GenerateTangentsSoA_ISPC(float4 *dst,
    const float *v1x, const float *v1y, const float *v1z,
    const float *v2x, const float *v2y, const float *v2z,
    const float *v3x, const float *v3y, const float *v3z,
    const float *u1x, const float *u1y,
    const float *u2x, const float *u2y,
    const float *u3x, const float *u3y,
    const float3 *normals,
    const int *indices, int num_triangles, int num_vertices)
{
    RawVector<float3> tmp_tangents, tmp_binormals;
    tmp_tangents.resize_with_zeroclear(num_vertices);
    tmp_binormals.resize_with_zeroclear(num_vertices);

    ispc::GenerateTangentsSoA(
        (ispc::float4*)dst,
        v1x, v1y, v1z,
        v2x, v2y, v2z,
        v3x, v3y, v3z,
        u1x, u1y,
        u2x, u2y,
        u3x, u3y,
        (ispc::float3*)normals,
        num_triangles, num_vertices,
        indices,
        (ispc::float3*)tmp_tangents.data(),
        (ispc::float3*)tmp_binormals.data());
}


#endif



#ifdef muEnableISPC
    #define Forward(Name, ...) Name##_ISPC(__VA_ARGS__)
#else
    #define Forward(Name, ...) Name##_Generic(__VA_ARGS__)
#endif

#ifdef muEnableHalf
void FloatToHalf(half *dst, const float *src, size_t num)
{
    Forward(FloatToHalf, dst, src, num);
}
void HalfToFloat(float *dst, const half *src, size_t num)
{
    Forward(HalfToFloat, dst, src, num);
}
#endif // muEnableHalf

void InvertX(float3 *dst, size_t num)
{
    Forward(InvertX, dst, num);
}
void InvertX(float4 *dst, size_t num)
{
    Forward(InvertX, dst, num);
}

void Scale(float *dst, float s, size_t num)
{
    Forward(Scale, dst, s, num);
}
void Scale(float3 *dst, float s, size_t num)
{
    Forward(Scale, dst, s, num);
}

void Normalize(float3 *dst, size_t num)
{
    Forward(Normalize, dst, num);
}

void Lerp(float *dst, const float *src1, const float *src2, size_t num, float w)
{
    Forward(Lerp, dst, src1, src2, num, w);
}
void Lerp(float2 *dst, const float2 *src1, const float2 *src2, size_t num, float w)
{
    Lerp((float*)dst, (const float*)src1, (const float*)src2, num * 2, w);
}
void Lerp(float3 *dst, const float3 *src1, const float3 *src2, size_t num, float w)
{
    Lerp((float*)dst, (const float*)src1, (const float*)src2, num * 3, w);
}

void MinMax(const float2 *p, size_t num, float2& dst_min, float2& dst_max)
{
    Forward(MinMax, p, num, dst_min, dst_max);
}
void MinMax(const float3 *p, size_t num, float3& dst_min, float3& dst_max)
{
    Forward(MinMax, p, num, dst_min, dst_max);
}

bool NearEqual(const float *src1, const float *src2, size_t num, float eps)
{
    return Forward(NearEqual, src1, src2, num, eps);
}
bool NearEqual(const float2 *src1, const float2 *src2, size_t num, float eps)
{
    return NearEqual((const float*)src1, (const float*)src2, num * 2, eps);
}
bool NearEqual(const float3 *src1, const float3 *src2, size_t num, float eps)
{
    return NearEqual((const float*)src1, (const float*)src2, num * 3, eps);
}

void MulPoints(const float4x4& m, const float3 src[], float3 dst[], size_t num_data)
{
    Forward(MulPoints, m, src, dst, num_data);
}
void MulVectors(const float4x4& m, const float3 src[], float3 dst[], size_t num_data)
{
    Forward(MulVectors, m, src, dst, num_data);
}

int RayTrianglesIntersectionIndexed(float3 pos, float3 dir, const float3 *vertices, const int *indices, int num_triangles, int& tindex, float& result)
{
    return Forward(RayTrianglesIntersectionIndexed, pos, dir, vertices, indices, num_triangles, tindex, result);
}
int RayTrianglesIntersectionArray(float3 pos, float3 dir, const float3 *vertices, int num_triangles, int& tindex, float& result)
{
    return Forward(RayTrianglesIntersectionArray, pos, dir, vertices, num_triangles, tindex, result);
}
int RayTrianglesIntersectionSoA(float3 pos, float3 dir,
    const float *v1x, const float *v1y, const float *v1z,
    const float *v2x, const float *v2y, const float *v2z,
    const float *v3x, const float *v3y, const float *v3z,
    int num_triangles, int& tindex, float& result)
{
    return Forward(RayTrianglesIntersectionSoA, pos, dir, v1x, v1y, v1z, v2x, v2y, v2z, v3x, v3y, v3z, num_triangles, tindex, result);
}

bool PolyInside(const float px[], const float py[], int ngon, const float2 minp, const float2 maxp, const float2 pos)
{
    return Forward(PolyInside, px, py, ngon, minp, maxp, pos);
}
bool PolyInside(const float2 poly[], int ngon, const float2 minp, const float2 maxp, const float2 pos)
{
    return Forward(PolyInside, poly, ngon, minp, maxp, pos);
}
bool PolyInside(const float2 poly[], int ngon, const float2 pos)
{
    return Forward(PolyInside, poly, ngon, pos);
}

void GenerateTangentsIndexed(float4 *dst,
    const float3 *vertices, const float3 *normals, const float2 *uv, const int *indices, int num_triangles, int num_vertices)
{
    return Forward(GenerateTangentsIndexed, dst, vertices, normals, uv, indices, num_triangles, num_vertices);
}
void GenerateTangentsSoA(float4 *dst,
    const float *v1x, const float *v1y, const float *v1z,
    const float *v2x, const float *v2y, const float *v2z,
    const float *v3x, const float *v3y, const float *v3z,
    const float *u1x, const float *u1y,
    const float *u2x, const float *u2y,
    const float *u3x, const float *u3y,
    const float3 *normals,
    const int *indices, int num_triangles, int num_vertices)
{
    return Forward(GenerateTangentsSoA, dst,
        v1x, v1y, v1z, v2x, v2y, v2z, v3x, v3y, v3z,
        u1x, u1y, u2x, u2y, u3x, u3y,
        normals, indices, num_triangles, num_vertices);
}

#undef Forward
} // namespace mu

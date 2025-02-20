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
#include "Math/Math.h"

BE_NAMESPACE_BEGIN

Line::Line(const LineSegment &lineSegment) {
    pos = lineSegment.a;
    dir = lineSegment.Dir();
}

Line::Line(const Ray &ray) {
    pos = ray.origin;
    dir = ray.dir;
}

Line Line::Translate(const Vec3 &translation) const {
    return Line(pos + translation, dir);
}

Line &Line::TranslateSelf(const Vec3 &translation) {
    pos += translation;
    return *this;
}

void Line::Transform(const Mat3 &transform) {
    pos = transform * pos;
    dir = transform * dir;
}

void Line::Transform(const Mat3x4 &transform) {
    pos = transform.Transform(pos);
    dir = transform.TransformNormal(dir);
}

void Line::Transform(const Mat4 &transform) {
    pos = transform * pos;
    dir = transform.TransformNormal(dir);
}

void Line::Transform(const Quat &transform) {
    pos = transform * pos;
    dir = transform * dir;
}

Vec3 Line::ClosestPoint(const Vec3 &point) const {
    float d = dir.Dot(point - pos);
    return GetPoint(d);
}

float Line::Distance(const Vec3 &point) const {
    Vec3 closestPoint = ClosestPoint(point);
    return closestPoint.Distance(point);
}

float Line::DistanceSqr(const Vec3 &point) const {
    Vec3 closestPoint = ClosestPoint(point);
    return closestPoint.DistanceSqr(point);
}

LineSegment Line::ToLineSegment(float d) const {
    return LineSegment(pos, GetPoint(d));
}

LineSegment Line::ToLineSegment(float dStart, float dEnd) const {
    return LineSegment(GetPoint(dStart), GetPoint(dEnd));
}

Ray Line::ToRay() const {
    return Ray(pos, dir);
}

Line Line::FromString(const char *str) {
    Line line;
    sscanf(str, "%f %f %f %f %f %f", &line.pos.x, &line.pos.y, &line.pos.z, &line.dir.x, &line.dir.y, &line.dir.z);
    return line;
}

BE_NAMESPACE_END

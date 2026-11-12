// Reine 3D-Mathematik ohne SFML oder OpenGL. Vektoren, Kamera, Projektion
// und Animationskurven sind dadurch separat und deterministisch testbar.
#pragma once
#include <algorithm>
#include <cmath>

namespace gym3d {
constexpr float pi = 3.14159265358979323846f;
struct Vec3 { float x, y, z; };
struct Vec2 { float x, y; };
inline Vec3 operator+(Vec3 a, Vec3 b) { return {a.x+b.x,a.y+b.y,a.z+b.z}; }
inline Vec3 operator-(Vec3 a, Vec3 b) { return {a.x-b.x,a.y-b.y,a.z-b.z}; }
inline Vec3 operator*(Vec3 a,float f) {return {a.x*f,a.y*f,a.z*f};}
inline float dot(Vec3 a,Vec3 b) {return a.x*b.x+a.y*b.y+a.z*b.z;}
inline Vec3 cross(Vec3 a,Vec3 b) {return {a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};}
// Teilt einen Vektor durch seine Länge. Für Null-/Kurzvektoren wird
// {0,0,0} geliefert, damit keine Division durch null entsteht.
inline Vec3 normalize(Vec3 a) {
    const float l=std::sqrt(dot(a,a));
    return l<1.e-6f?Vec3{0,0,0}:a*(1.f/l);
}
inline float clamp(float value,float lo,float hi) {return std::max(lo,std::min(hi,value));}
inline float easeOut(float t) {t=clamp(t,0.f,1.f);return 1.f-(1.f-t)*(1.f-t)*(1.f-t);}
inline float easeInOut(float t) {
    t=clamp(t,0.f,1.f);
    return t*t*(3.f-2.f*t);
}
struct Camera {
    float yaw=.21f;
    float pitch=.14f;
    float distance=16.f;
};
inline Vec3 eye(Camera c) {
    return {std::sin(c.yaw)*std::cos(c.pitch)*c.distance,
            std::sin(c.pitch)*c.distance,
            std::cos(c.yaw)*std::cos(c.pitch)*c.distance};
}
struct Basis { Vec3 right,up,forward; };
// Erzeugt die orthogonale Kamera-Basis aus Blickrichtung und Welt-Hochachse.
// Sie wird sowohl beim Zeichnen als auch beim Pixel-Raycast verwendet.
inline Basis basis(Camera c) {
    const auto forward=normalize(eye(c)*-1.f);
    const auto right=normalize(cross(forward,{0,1,0}));
    return {right,cross(right,forward),forward};
}
struct Screen {float x,y,depth;bool visible;};
// Perspektivische Projektion: mit wachsender Tiefe schrumpft die
// Abbildung. Punkte hinter der Nahgrenze werden als unsichtbar markiert.
inline Screen project(Vec3 point,Camera camera,Vec2 centre,float height,float fov=48.f) {
    const auto axis=basis(camera);
    const auto offset=point-eye(camera);
    const float depth=dot(offset,axis.forward);
    if(depth<=.05f)return {0,0,depth,false};
    const float scale=(height*.5f)/std::tan(fov*pi/360.f);
    return {centre.x+dot(offset,axis.right)*scale/depth,
            centre.y-dot(offset,axis.up)*scale/depth,
            depth,true};
}
inline Vec3 slot(int row,int col,int rows,int cols,float spacing=1.05f) {
    return {(static_cast<float>(col)-(cols-1)*.5f)*spacing,
            ((rows-1)*.5f-static_cast<float>(row))*spacing,0.f};
}
// Ease-out: gegen Ende wird die Fallbewegung langsamer.
inline float dropHeight(float elapsed,float seconds,float from,float to) {
    return from+(to-from)*easeOut(elapsed/std::max(.01f,seconds));
}
// Begrenzte Kamerawinkel und Mindestabstand schützen die Sichtbarkeit
// des Spielfelds auch bei 4 Spielern (10x11).
inline Camera adjustedCamera(Camera c,int rows,int cols) {
    c.pitch=clamp(c.pitch,-.38f,.55f);
    c.yaw=clamp(c.yaw,-.82f,.82f);
    const float d=std::max(static_cast<float>(cols)*1.35f,static_cast<float>(rows)*1.55f);
    c.distance=std::max(d,c.distance);
    return c;
}
} // namespace gym3d

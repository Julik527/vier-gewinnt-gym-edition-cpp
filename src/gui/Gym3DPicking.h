// Mausauswahl im 3D-Spiel: Pixelkoordinaten als Strahl in den Raum
// zurückrechnen und mit der Ebene des Spielbretts schneiden.
// Dieselbe Kamera-Geometrie wie beim Rendern verhindert versetzte Klicks.
#pragma once
#include "Gym3DMath.h"
#include <cmath>

namespace gym3d {
// Rückgabe: nullbasierte Spalte oder -1 (außerhalb/parallel/hinter Kamera).
// rows/cols und spacing müssen zur tatsächlich gezeichneten Arena passen.
inline int pickColumn(Vec2 pixel, Camera camera, Vec2 centre,
                      float viewportHeight, int rows, int cols,
                      float planeDepth = .22f, float spacing = 1.05f) {
    if(rows <= 0 || cols <= 0 || viewportHeight <= 0.f)
        return -1;
    camera = adjustedCamera(camera, rows, cols);
    const float scale = (viewportHeight * .5f) / std::tan(48.f * pi / 360.f);
    const auto axes = basis(camera);
    const auto origin = eye(camera);
    // Bildschirmverschiebung in einen Raumstrahl umrechnen.
    // Die Richtung muss hier nicht normiert werden (Schnittpunkt ist invariant).
    const Vec3 ray = axes.forward
        + axes.right * ((pixel.x - centre.x) / scale)
        + axes.up * ((centre.y - pixel.y) / scale);
    if(std::abs(ray.z) < 1.e-5f)
        return -1;  // Parallel to the board.
    const float distance = (planeDepth - origin.z) / ray.z;
    if(distance <= 0.f)
        return -1;  // Behind the camera.
    const auto target = origin + ray * distance;
    const float halfWidth = static_cast<float>(cols) * spacing * .5f;
    const float halfHeight = static_cast<float>(rows) * spacing * .5f;
    if(target.x < -halfWidth || target.x >= halfWidth ||
       target.y < -halfHeight || target.y >= halfHeight)
        return -1;  // Click outside the actual game board.
    const int col = static_cast<int>(std::floor((target.x + halfWidth) / spacing));
    return col >= 0 && col < cols ? col : -1;
}
} // namespace gym3d

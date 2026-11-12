/**
 * @file tests/scene3d_tests.cpp
 * @brief Testet die reine Kameramathematik und Spaltenauswahl der 3D-Arena.
 * @details Prüft Projektion, Kamerabasis, Sichtbarkeitsgrenzen, Animation und Pixel-Raycast auf das Spielfeld.
 * @par Abhängigkeiten
 * Gym3DMath.h und Gym3DPicking.h; kein OpenGL-Kontext nötig.
 * @par Wichtige Randbedingungen
 * Geometrische Unit-Tests bestätigen nicht die tatsächliche Darstellung auf einer Grafikkarte.
 */

#include "Gym3DMath.h"
#include "Gym3DPicking.h"
#include <cmath>
#include <iostream>
#include <stdexcept>
using namespace std;
static void check(bool ok,const char* error) {if(!ok)throw runtime_error(error);}
int main() {
    using namespace gym3d;
    const Camera c{0,0,16.f};
    const auto middle=project({0,0,0},c,{500,400},700);
    check(middle.visible && abs(middle.x-500.f)<.001f && abs(middle.y-400.f)<.001f,"centre projection");
    check(project({-1,0,0},c,{500,400},700).x < middle.x,"left placement");
    check(project({1,0,0},c,{500,400},700).x > middle.x,"right placement");
    check(project({0,1,0},c,{500,400},700).y < middle.y,"top placement");
    check(!project({0,0,20},c,{500,400},700).visible,"camera clipping");
    check(abs(dot(basis(c).right,basis(c).forward))<1.e-5f,"orthogonal view");
    check(abs(slot(0,0,6,7).x+3.15f)<.001f,"7x6 origin");
    check(abs(slot(9,10,10,11).x-5.25f)<.001f,"11x10 last column");
    for(int players=2;players<=4;++players) {
        const int rows=6+(players-2)*2,cols=rows+1;
        auto orb=adjustedCamera({.2f,.12f,4.f},rows,cols);
        check(orb.distance>=cols*1.35f,"camera fits all board sizes");
        for(int r=0;r<rows;++r)for(int x=0;x<cols;++x){
            const auto p=project(slot(r,x,rows,cols),orb,{520,400},690);
            check(p.visible && isfinite(p.x)&&isfinite(p.y),"valid projection");
        }
    }
    // Full click->ray->board round-trip, including extreme board sizes,
    // yaw and pitch: catches wrong-column clicks after camera rotation.
    const Vec2 centre{536.f,510.f};
    for(int players=2;players<=4;++players) {
        const int rows=6+(players-2)*2,cols=rows+1;
        for(float yaw : {-.75f,0.f,.68f}) {
            for(float pitch : {-.3f,.1f,.5f}) {
                Camera orbit=adjustedCamera({yaw,pitch,16.f},rows,cols);
                for(int r=0;r<rows;++r)for(int col=0;col<cols;++col) {
                    const auto projected=project(
                        slot(r,col,rows,cols)+Vec3{0,0,.22f},
                        orbit,centre,780.f);
                    check(projected.visible,"click target visible");
                    check(pickColumn({projected.x,projected.y},orbit,
                           centre,780.f,rows,cols)==col,
                           "orbit camera selects correct column");
                }
                const float edge=static_cast<float>(cols)*1.05f*.5f;
                const float top=static_cast<float>(rows)*1.05f*.5f;
                const auto outsideX=project({edge+1.f,0,.22f},orbit,centre,780.f);
                const auto outsideY=project({0,top+1.f,.22f},orbit,centre,780.f);
                check(pickColumn({outsideX.x,outsideX.y},orbit,centre,780.f,
                       rows,cols)==-1,"outside horizontal board rejected");
                check(pickColumn({outsideY.x,outsideY.y},orbit,centre,780.f,
                       rows,cols)==-1,"outside vertical board rejected");
            }
        }
    }
    check(pickColumn({0,0},{},centre,0.f,6,7)==-1,"invalid viewport rejected");
    check(abs(dropHeight(0,.5f,10,0)-10.f)<.00001f,"animation begins at original position");
    check(abs(dropHeight(1,.5f,10,0))<.00001f,"animation settles at target");
    check(abs(easeInOut(.5f)-.5f)<.0001f,"cinematic easing");
    cout<<"Gym Edition 3D: camera, projection, depth, board and animation OK\n";
}

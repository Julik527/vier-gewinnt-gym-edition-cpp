#include "Gym3DMath.h"
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
    check(abs(dropHeight(0,.5f,10,0)-10.f)<.00001f,"animation begins at original position");
    check(abs(dropHeight(1,.5f,10,0))<.00001f,"animation settles at target");
    check(abs(easeInOut(.5f)-.5f)<.0001f,"cinematic easing");
    cout<<"Gym Edition 3D: camera, projection, depth, board and animation OK\n";
}

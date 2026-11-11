// Isolated OpenGL compatibility rendering primitives; no gameplay state.
#pragma once
#include "Gym3DMath.h"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <algorithm>
#include <cmath>

namespace gym3d_render {
inline void color(sf::Color c,float strength=1.f,float alpha=1.f) {
    glColor4f(c.r/255.f*strength,c.g/255.f*strength,c.b/255.f*strength,alpha);
}
inline void cube(float x,float y,float z,float sx,float sy,float sz,sf::Color tint) {
    const float a=x-sx*.5f,b=x+sx*.5f;
    const float c=y-sy*.5f,d=y+sy*.5f;
    const float e=z-sz*.5f,f=z+sz*.5f;
    color(tint);
    glBegin(GL_QUADS);
    glNormal3f(0,0,1);glVertex3f(a,c,f);glVertex3f(b,c,f);glVertex3f(b,d,f);glVertex3f(a,d,f);
    glNormal3f(0,0,-1);glVertex3f(b,c,e);glVertex3f(a,c,e);glVertex3f(a,d,e);glVertex3f(b,d,e);
    glNormal3f(1,0,0);glVertex3f(b,c,f);glVertex3f(b,c,e);glVertex3f(b,d,e);glVertex3f(b,d,f);
    glNormal3f(-1,0,0);glVertex3f(a,c,e);glVertex3f(a,c,f);glVertex3f(a,d,f);glVertex3f(a,d,e);
    glNormal3f(0,1,0);glVertex3f(a,d,f);glVertex3f(b,d,f);glVertex3f(b,d,e);glVertex3f(a,d,e);
    glNormal3f(0,-1,0);glVertex3f(a,c,e);glVertex3f(b,c,e);glVertex3f(b,c,f);glVertex3f(a,c,f);
    glEnd();
}
// A real illuminated cylinder: front, side wall and back, all in world space.
inline void disc(float x,float y,float z,float radius,float thickness,sf::Color tint) {
    constexpr int segments=28;
    glPushMatrix();
    glTranslatef(x,y,z);
    color(tint);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,0,1);glVertex3f(0,0,thickness*.5f);
    for(int i=0;i<=segments;++i){const float a=2*gym3d::pi*i/segments;
        glVertex3f(std::cos(a)*radius,std::sin(a)*radius,thickness*.5f);}
    glEnd();
    glBegin(GL_QUAD_STRIP);
    for(int i=0;i<=segments;++i){const float a=2*gym3d::pi*i/segments;
        glNormal3f(std::cos(a),std::sin(a),0);
        glVertex3f(std::cos(a)*radius,std::sin(a)*radius,thickness*.5f);
        glVertex3f(std::cos(a)*radius,std::sin(a)*radius,-thickness*.5f);}
    glEnd();
    color(tint,.62f);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,0,-1);glVertex3f(0,0,-thickness*.5f);
    for(int i=0;i<=segments;++i){const float a=2*gym3d::pi*i/segments;
        glVertex3f(std::cos(a)*radius,std::sin(a)*radius,-thickness*.5f);}
    glEnd();
    glPopMatrix();
}
// Embossed barbell mark on the face of every volumetric token.
inline void dumbbellRelief(float x,float y,float z) {
    const sf::Color steel(228,238,248),highlight(135,189,235);
    cube(x,y,z+.17f,.53f,.09f,.07f,steel);
    cube(x-.24f,y,z+.18f,.11f,.35f,.09f,steel);
    cube(x+.24f,y,z+.18f,.11f,.35f,.09f,steel);
    cube(x-.31f,y,z+.20f,.045f,.26f,.06f,highlight);
    cube(x+.31f,y,z+.20f,.045f,.26f,.06f,highlight);
}
inline void ring(float x,float y,float z,float outer,float inner,sf::Color tint) {
    constexpr int segments=24;
    color(tint);
    glBegin(GL_QUAD_STRIP);
    glNormal3f(0,0,1);
    for(int i=0;i<=segments;++i){const float a=2.f*gym3d::pi*i/segments;
        glVertex3f(x+std::cos(a)*outer,y+std::sin(a)*outer,z);
        glVertex3f(x+std::cos(a)*inner,y+std::sin(a)*inner,z+.012f);}
    glEnd();
}
inline void cameraGL(gym3d::Camera camera,unsigned width,unsigned height) {
    const float aspect=static_cast<float>(width)/static_cast<float>(std::max(1u,height));
    constexpr float nearPlane=.18f;
    const float top=nearPlane*std::tan(48.f*gym3d::pi/360.f),right=top*aspect;
    glMatrixMode(GL_PROJECTION);glLoadIdentity();
    glFrustum(-right,right,-top,top,nearPlane,120.f);
    const auto basis=gym3d::basis(camera);
    const auto e=gym3d::eye(camera);
    const float matrix[]={
        basis.right.x,basis.up.x,-basis.forward.x,0,
        basis.right.y,basis.up.y,-basis.forward.y,0,
        basis.right.z,basis.up.z,-basis.forward.z,0,
        0,0,0,1
    };
    glMatrixMode(GL_MODELVIEW);glLoadIdentity();
    glMultMatrixf(matrix);
    glTranslatef(-e.x,-e.y,-e.z);
}
} // namespace gym3d_render

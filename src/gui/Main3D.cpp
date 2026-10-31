// JK AXIS | Vier Gewinnt - Gym Edition 3D.
// An additional, real OpenGL 3D presentation of the shared C++17 MatchSession.
// 2D and console versions remain independent, including GYM1 persistence.
#include "MatchSession.h"
#include "Gym3DMath.h"
#include "GymBeat.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <algorithm>
#include <array>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

using namespace std;
namespace {
constexpr float W=1440.f,H=900.f;
constexpr float viewportWidth=1072.f,viewportTop=120.f,viewportHeight=780.f;
const sf::Color white(244,248,255),muted(153,176,206),blue(63,131,242),green(69,224,180);
const array<sf::Color,4> colors={sf::Color(61,149,252),sf::Color(247,190,85),
                                 sf::Color(76,217,163),sf::Color(196,145,252)};
enum class Page { Intro,Menu,Names,Playing,Result,Help,ConfirmExit };
sf::Font loadFont() {
    sf::Font result;
    const array<const char*,6> choices={{
        "C:/Windows/Fonts/segoeui.ttf","C:/Windows/Fonts/arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf",
        "/usr/share/fonts/TTF/DejaVuSans.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf"
    }};
    for(const char* location:choices) if(result.openFromFile(location)) return result;
    throw runtime_error("Keine Systemschrift vorhanden (Segoe UI / DejaVu Sans).");
}
void color(sf::Color c,float strength=1.f,float alpha=1.f) {
    glColor4f(c.r/255.f*strength,c.g/255.f*strength,c.b/255.f*strength,alpha);
}
sf::Color forSymbol(char symbol) {
    switch(symbol) {case 'R':return colors[0];case 'B':return colors[1];
        case 'G':return colors[2];case 'Y':return colors[3];default:return muted;}
}
void cube(float x,float y,float z,float sx,float sy,float sz,sf::Color tint) {
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
void disc(float x,float y,float z,float radius,float thickness,sf::Color tint) {
    constexpr int segments=28;
    glPushMatrix();
    glTranslatef(x,y,z);
    color(tint);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,0,1);glVertex3f(0,0,thickness*.5f);
    for(int i=0;i<=segments;++i){const float a=2*gym3d::pi*i/segments;
        glVertex3f(cos(a)*radius,sin(a)*radius,thickness*.5f);}
    glEnd();
    glBegin(GL_QUAD_STRIP);
    for(int i=0;i<=segments;++i){const float a=2*gym3d::pi*i/segments;
        glNormal3f(cos(a),sin(a),0);
        glVertex3f(cos(a)*radius,sin(a)*radius,thickness*.5f);
        glVertex3f(cos(a)*radius,sin(a)*radius,-thickness*.5f);}
    glEnd();
    color(tint,.62f);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,0,-1);glVertex3f(0,0,-thickness*.5f);
    for(int i=0;i<=segments;++i){const float a=2*gym3d::pi*i/segments;
        glVertex3f(cos(a)*radius,sin(a)*radius,-thickness*.5f);}
    glEnd();
    glPopMatrix();
}
void ring(float x,float y,float z,float outer,float inner,sf::Color tint) {
    constexpr int segments=24;
    color(tint);
    glBegin(GL_QUAD_STRIP);
    glNormal3f(0,0,1);
    for(int i=0;i<=segments;++i){const float a=2.f*gym3d::pi*i/segments;
        glVertex3f(x+cos(a)*outer,y+sin(a)*outer,z);
        glVertex3f(x+cos(a)*inner,y+sin(a)*inner,z+.012f);}
    glEnd();
}
void cameraGL(gym3d::Camera camera,unsigned width,unsigned height) {
    const float aspect=static_cast<float>(width)/static_cast<float>(max(1u,height));
    constexpr float nearPlane=.18f;
    const float top=nearPlane*tan(48.f*gym3d::pi/360.f),right=top*aspect;
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
struct Hit {sf::FloatRect area;int action;};
class Arena3D {
public:
    Arena3D():window_(sf::VideoMode({1440u,900u}),"JK AXIS | Gym Edition 3D"),
              font_(loadFont()) {
        window_.setFramerateLimit(60);
        camera_.distance=16.f;
        window_.setKeyRepeatEnabled(false);
    }
    void run(int maxFrames=-1) {
        sf::Clock clock;int frames=0;
        while(window_.isOpen()) {
            const float dt=min(clock.restart().asSeconds(),.06f);
            elapsed_+=dt;
            introTime_+=(page_==Page::Intro?dt:0.f);
            if(page_==Page::Intro && introTime_>4.f) page_=Page::Menu;
            if(messageTime_>0.f)messageTime_=max(0.f,messageTime_-dt);
            if(dropActive_) {
                dropTime_+=dt;
                if(dropTime_>=.44f) {
                    dropActive_=false;
                    if(match_.drop(dropCol_)) {
                        if(match_.finished()) {page_=Page::Result;victoryTime_=0.f;}
                    }
                }
            }
            if(page_==Page::Result) victoryTime_+=dt;
            while(const auto event=window_.pollEvent()) {
                if(event->is<sf::Event::Closed>()) {
                    if(match_.active()) page_=Page::ConfirmExit;
                    else window_.close();
                }
                if(const auto* key=event->getIf<sf::Event::KeyPressed>()) keyPressed(key->code);
                if(const auto* typed=event->getIf<sf::Event::TextEntered>()) typedText(typed->unicode);
                if(const auto* wheel=event->getIf<sf::Event::MouseWheelScrolled>())
                    camera_.distance=gym3d::clamp(camera_.distance-wheel->delta*1.1f,12.f,30.f);
                if(const auto* mouse=event->getIf<sf::Event::MouseButtonPressed>()) {
                    if(mouse->button==sf::Mouse::Button::Right) {drag_=true;lastMouse_=mouse->position;}
                    if(mouse->button==sf::Mouse::Button::Left) click(window_.mapPixelToCoords(mouse->position));
                }
                if(const auto* mouse=event->getIf<sf::Event::MouseButtonReleased>())
                    if(mouse->button==sf::Mouse::Button::Right) drag_=false;
                if(const auto* moved=event->getIf<sf::Event::MouseMoved>()) {
                    if(drag_) {
                        const auto dx=moved->position.x-lastMouse_.x;
                        const auto dy=moved->position.y-lastMouse_.y;
                        camera_.yaw=gym3d::clamp(camera_.yaw+dx*.007f,-.82f,.82f);
                        camera_.pitch=gym3d::clamp(camera_.pitch-dy*.006f,-.35f,.55f);
                        lastMouse_=moved->position;
                    }
                }
            }
            if(!window_.isOpen())break;
            draw();
            if(maxFrames>0 && ++frames>=maxFrames)window_.close();
        }
    }
private:
    sf::RenderWindow window_;
    sf::Font font_;
    MatchSession match_;
    unique_ptr<GymBeat> beat_;
    bool music_=false;
    int volume_=35;
    Page page_=Page::Intro;
    gym3d::Camera camera_{};
    float elapsed_=0.f,introTime_=0.f,victoryTime_=0.f;
    int playerCount_=2;
    vector<string> names_;
    string nameInput_;
    string info_;
    float messageTime_=0.f;
    int selected_=3;
    bool dropActive_=false;
    float dropTime_=0.f;
    int dropCol_=0,dropRow_=0,dropPlayer_=0;
    bool drag_=false;
    sf::Vector2i lastMouse_{};
    vector<Hit> hits_;

    void notice(string message) {info_=std::move(message);messageTime_=4.f;}
    void musicToggle() {
        if(music_) {if(beat_)beat_->stop(); music_=false;notice("MUSIK PAUSIERT");return;}
        try {
            if(!beat_)beat_=make_unique<GymBeat>();
            beat_->setVolume(static_cast<float>(volume_));
            music_=beat_->play();
            notice(music_?"ORIGINAL SOUNDTRACK AKTIV":"AUDIOAUSGABE NICHT VERFUEGBAR");
        } catch(const exception&) {music_=false;notice("TON NICHT VERFUEGBAR - SPIEL WEITERHIN MOEGLICH");}
    }
    void volumeBy(int step) {
        volume_=std::clamp(volume_+step,0,100);
        if(beat_)beat_->setVolume(static_cast<float>(volume_));
        notice("MUSIKLAUTSTAERKE: "+to_string(volume_)+" %");
    }
    void startNames() {
        names_.clear();nameInput_.clear();page_=Page::Names;
        notice("NAMEN EINGEBEN, ENTER BESTAETIGT");
    }
    void commitName() {
        if(!MatchSession::validName(nameInput_)) {notice("NAME: 1-24 ZEICHEN, NICHT LEER");return;}
        if(find(names_.begin(),names_.end(),nameInput_)!=names_.end()) {
            notice("BITTE EINEN EINDEUTIGEN NAMEN EINGEBEN");return;
        }
        names_.push_back(nameInput_);
        nameInput_.clear();
        if(static_cast<int>(names_.size())==playerCount_) {
            if(match_.begin(names_)) {
                selected_=match_.board().columns()/2;
                page_=Page::Playing;notice("SPIEL GESTARTET - VIEL ERFOLG!");
            } else notice("SPIELSTART FEHLGESCHLAGEN");
        }
    }
    void typedText(unsigned int code) {
        if(page_!=Page::Names)return;
        if(code==8) {if(!nameInput_.empty())nameInput_.pop_back();return;}
        if(code<32 || code==127 || code>126 || nameInput_.size()>=24)return;
        nameInput_.push_back(static_cast<char>(code));
    }
    void startDrop() {
        if(page_!=Page::Playing||!match_.active()||dropActive_)return;
        const int row=match_.board().nextFreeRow(selected_);
        if(row<0){notice("DIESE SPALTE IST VOLL");return;}
        dropCol_=selected_;dropRow_=row;dropPlayer_=match_.current();
        dropTime_=0.f;dropActive_=true;
    }
    void act(int action) {
        switch(action) {
            case 1: startNames();break;
            case 2: if(match_.active())page_=Page::Playing;break;
            case 3:
                if(match_.active()) {notice("ZUERST SPIEL SICHERN ODER MENUE VERLASSEN");break;}
                if(match_.load()) {selected_=match_.board().columns()/2;page_=Page::Playing;notice("SPIELSTAND GELADEN");}
                else notice("KEIN GUELTIGER SPIELSTAND GEFUNDEN");
                break;
            case 4: page_=Page::Help;break;
            case 5: page_=Page::Menu;break;
            case 6: if(match_.active()&&!dropActive_&&match_.save())notice("SPIELSTAND GESPEICHERT");else notice("SPEICHERN NICHT MOEGLICH");break;
            case 7: if(match_.active()&&!dropActive_&&match_.undo())notice("LETZTER ZUG RUECKGAENGIG");else notice("RUECKGAENGIG NICHT MOEGLICH");break;
            case 8: startDrop();break;
            case 9: if(match_.rematch()){page_=Page::Playing;selected_=match_.board().columns()/2;notice("REVANCHE GESTARTET");}break;
            case 10: musicToggle();break;
            case 11: volumeBy(-10);break;
            case 12: volumeBy(10);break;
            case 13: window_.close();break;
            case 14: if(match_.active()&&!dropActive_)match_.save();window_.close();break;
            case 15: page_=match_.active()?Page::Playing:Page::Menu;break;
            case 20: playerCount_=2;break;
            case 21: playerCount_=3;break;
            case 22: playerCount_=4;break;
        }
    }
    void keyPressed(sf::Keyboard::Key key) {
        using K=sf::Keyboard::Key;
        if(key==K::F9) {musicToggle();return;}
        if(key==K::F10) {volumeBy(-10);return;}
        if(key==K::F11) {volumeBy(10);return;}
        if(key==K::Escape) {
            if(page_==Page::Intro)page_=Page::Menu;
            else if(page_==Page::Playing||page_==Page::Result)page_=Page::Menu;
            else if(page_==Page::ConfirmExit)page_=match_.active()?Page::Playing:Page::Menu;
            else page_=Page::Menu;
            return;
        }
        if(page_==Page::Intro){page_=Page::Menu;return;}
        if(page_==Page::Menu) {
            if(key==K::Num2||key==K::Numpad2)playerCount_=2;
            if(key==K::Num3||key==K::Numpad3)playerCount_=3;
            if(key==K::Num4||key==K::Numpad4)playerCount_=4;
            if(key==K::Enter)startNames();
            if(key==K::L)act(3);
        } else if(page_==Page::Names) {
            if(key==K::Enter)commitName();
            if(key==K::Backspace&&!nameInput_.empty())nameInput_.pop_back();
        } else if(page_==Page::Playing) {
            const int columns=match_.board().columns();
            if(key==K::Left)selected_=(selected_+columns-1)%columns;
            if(key==K::Right)selected_=(selected_+1)%columns;
            if(key==K::Space||key==K::Enter)startDrop();
            if(key==K::U)act(7);
            if(key==K::S)act(6);
            if(key==K::A)camera_.yaw=gym3d::clamp(camera_.yaw-.13f,-.82f,.82f);
            if(key==K::D)camera_.yaw=gym3d::clamp(camera_.yaw+.13f,-.82f,.82f);
            if(key==K::Q)camera_.pitch=gym3d::clamp(camera_.pitch-.10f,-.35f,.55f);
            if(key==K::E)camera_.pitch=gym3d::clamp(camera_.pitch+.10f,-.35f,.55f);
            if(key==K::C){camera_.yaw=.21f;camera_.pitch=.14f;notice("KAMERA ZENTRIERT");}
        } else if(page_==Page::Result&&key==K::Enter)act(9);
        else if(page_==Page::Help&&key==K::Enter)act(5);
    }
    void click(sf::Vector2f p) {
        for(auto it=hits_.rbegin();it!=hits_.rend();++it) {
            if(it->area.contains(p)) {act(it->action);return;}
        }
        if(page_==Page::Intro){page_=Page::Menu;return;}
        if(page_!=Page::Playing||p.x>=viewportWidth||p.y<viewportTop)return;
        const auto cam=gym3d::adjustedCamera(camera_,
                match_.board().rows(),match_.board().columns());
        const int cols=match_.board().columns();
        float best=1.e8f;int col=0;
        for(int i=0;i<cols;++i) {
            const auto q=gym3d::project(gym3d::slot(0,i,match_.board().rows(),cols),
                    cam,{viewportWidth*.5f,viewportTop+viewportHeight*.5f},viewportHeight);
            const float dist=abs(q.x-p.x);
            if(dist<best){best=dist;col=i;}
        }
        if(best<85.f) {selected_=col;startDrop();}
    }
    void text(string s,float x,float y,unsigned size=20,sf::Color tint=white,bool bold=false) {
        sf::Text label(font_,sf::String::fromUtf8(s.begin(),s.end()),size);
        label.setFillColor(tint);
        if(bold)label.setStyle(sf::Text::Bold);
        label.setPosition({x,y});
        window_.draw(label);
    }
    void rect(float x,float y,float w,float h,sf::Color fill,sf::Color outline=sf::Color::Transparent) {
        sf::RectangleShape shape({w,h});shape.setPosition({x,y});
        shape.setFillColor(fill);
        shape.setOutlineColor(outline);shape.setOutlineThickness(outline==sf::Color::Transparent?0:1);
        window_.draw(shape);
    }
    void button(string title,float x,float y,float width,int action,bool active=true) {
        const sf::FloatRect bounds({x,y},{width,52});
        rect(x,y,width,52,active?sf::Color(25,58,96):sf::Color(25,39,54),
             active?sf::Color(51,100,158):sf::Color(32,51,75));
        text(std::move(title),x+16,y+12,18,active?white:muted,true);
        if(active)hits_.push_back({bounds,action});
    }
    void logo(float x,float y,float scale) {
        const array<sf::Vector2f,5> markA={{{17,18},{50,18},{50,75},{77,102},{59,120}}};
        // The two official angular paths remain recognizable, not redesigned.
        sf::ConvexShape a(5);for(size_t i=0;i<markA.size();++i)a.setPoint(i,{markA[i].x*scale,markA[i].y*scale});
        a.setPosition({x,y});a.setFillColor(sf::Color(218,232,250));window_.draw(a);
        sf::ConvexShape b(4);
        b.setPoint(0,{114*scale,18*scale});b.setPoint(1,{114*scale,57*scale});
        b.setPoint(2,{77*scale,93*scale});b.setPoint(3,{60*scale,75*scale});
        b.setPosition({x,y});b.setFillColor(blue);window_.draw(b);
    }
    void renderWorld() {
        const auto size=window_.getSize();
        const unsigned vw=max(1u,static_cast<unsigned>(size.x*viewportWidth/W));
        const unsigned vh=max(1u,static_cast<unsigned>(size.y*viewportHeight/H));
        glViewport(0,0,static_cast<GLsizei>(vw),static_cast<GLsizei>(vh));
        glClearDepth(1.0);glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);glDepthFunc(GL_LEQUAL);
        glEnable(GL_NORMALIZE);
        glEnable(GL_LIGHTING);glEnable(GL_LIGHT0);glEnable(GL_LIGHT1);
        glEnable(GL_COLOR_MATERIAL);glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glShadeModel(GL_SMOOTH);
        const GLfloat ambient[]={.18f,.23f,.35f,1};
        const GLfloat mainLight[]={.85f,.90f,1.f,1.f};
        const GLfloat fill[]={.24f,.55f,.92f,1};
        const GLfloat pos0[]={-6.f,9.f,12.f,1},pos1[]={8.f,5.f,7.f,1};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE,mainLight);
        glLightfv(GL_LIGHT0,GL_POSITION,pos0);
        glLightfv(GL_LIGHT1,GL_DIFFUSE,fill);
        glLightfv(GL_LIGHT1,GL_POSITION,pos1);
        const bool demo=page_==Page::Intro||page_==Page::Menu||page_==Page::Names||page_==Page::Help;
        const int rows=demo?6:match_.board().rows();
        const int cols=demo?7:match_.board().columns();
        auto camera=gym3d::adjustedCamera(camera_,rows,cols);
        if(page_==Page::Intro){camera.yaw=.22f+.33f*sin(elapsed_*.68f);camera.pitch=.13f+.08f*sin(elapsed_*.37f);}
        if(page_==Page::Result)camera.yaw=gym3d::clamp(camera.yaw+.06f*sin(victoryTime_*1.4f),-.8f,.8f);
        cameraGL(camera,vw,vh);
        glClearColor(.035f,.07f,.13f,1);
        // An infinite-grid training platform grounded in real 3D coordinates.
        glDisable(GL_LIGHTING);glLineWidth(1.f);
        glBegin(GL_LINES);glColor3f(.10f,.23f,.37f);
        for(int i=-14;i<=14;i+=2){
            glVertex3f(static_cast<float>(i),-5.9f,-7.f);
            glVertex3f(static_cast<float>(i),-5.9f,7.f);
            glVertex3f(-14.f,-5.9f,static_cast<float>(i)*.5f);
            glVertex3f(14.f,-5.9f,static_cast<float>(i)*.5f);
        }
        glEnd();glEnable(GL_LIGHTING);
        const float width=cols*1.05f,height=rows*1.05f;
        cube(0,-height*.5f-.38f,-.35f,width+1.3f,.23f,.76f,sf::Color(35,89,151));
        cube(0,height*.5f+.38f,-.35f,width+1.3f,.19f,.75f,sf::Color(45,103,167));
        cube(-width*.5f-.33f,0,-.34f,.25f,height+.95f,.78f,sf::Color(44,92,155));
        cube(width*.5f+.33f,0,-.34f,.25f,height+.95f,.78f,sf::Color(44,92,155));
        for(int i=0;i<=cols;++i) {
            const float x=(i-cols*.5f)*1.05f;
            cube(x,0,-.21f,.09f,height,.25f,sf::Color(34,70,119));
        }
        for(int r=0;r<rows;++r)for(int c=0;c<cols;++c) {
            const auto p=gym3d::slot(r,c,rows,cols);
            disc(p.x,p.y,-.35f,.43f,.045f,sf::Color(9,24,43));
            ring(p.x,p.y,.005f,.49f,.43f,sf::Color(44,100,165));
            char symbol='.';
            if(demo) {
                static const array<const char*,6> showcase={{".......",".......",".......","...B...","..RRB..",".RBRBB."}};
                symbol=showcase[static_cast<size_t>(r)][c];
            } else symbol=match_.board().cells()[r][c];
            if(symbol!='.') {
                const sf::Color tint=forSymbol(symbol);
                disc(p.x,p.y,.21f,.39f,.22f,tint);
                ring(p.x,p.y,.34f,.35f,.31f,sf::Color(203,229,250));
            }
        }
        // Selected column: pulsing emissive holographic indicator in front of the board.
        if(page_==Page::Playing&&!dropActive_) {
            const auto p=gym3d::slot(0,selected_,rows,cols);
            const float pulse=.5f+.5f*sin(elapsed_*4.6f);
            glDisable(GL_LIGHTING);glLineWidth(3.f);
            ring(p.x,p.y+.72f,.25f,.27f+.035f*pulse,.24f,colors[static_cast<size_t>(match_.current())]);
            glEnable(GL_LIGHTING);
        }
        // Real-world drop: spawn above the top rail and ease to actual Board::nextFreeRow.
        if(dropActive_) {
            const auto p=gym3d::slot(dropRow_,dropCol_,rows,cols);
            const float target=p.y;
            const float y=gym3d::dropHeight(dropTime_,.44f,height*.5f+1.6f,target);
            disc(p.x,y,.23f,.39f,.22f,colors[static_cast<size_t>(dropPlayer_)]);
        }
        if(page_==Page::Result&&!match_.winLine().empty()) {
            glDisable(GL_LIGHTING);
            for(const auto& point:match_.winLine()) {
                const auto p=gym3d::slot(point.first,point.second,rows,cols);
                ring(p.x,p.y,.41f,.48f+.04f*sin(elapsed_*4.f),.43f,green);
            }
            glPointSize(4.5f);
            glBegin(GL_POINTS);
            for(int i=0;i<145;++i) {
                const float seed=static_cast<float>(i);
                const float fall=fmod(victoryTime_*(.5f+fmod(seed,7.f)*.09f)+seed*.071f,1.f);
                const float x=sin(seed*17.73f)*(width*.55f);
                const float y=height*.6f-fall*(height+2.f);
                const float z=.7f+sin(seed*8.31f)*2.1f;
                const auto tint=colors[static_cast<size_t>(i%4)];
                color(tint,1.f,.8f);glVertex3f(x,y,z);
            }
            glEnd();glEnable(GL_LIGHTING);
        }
        glDisable(GL_LIGHTING);glDisable(GL_DEPTH_TEST);
        // Return all OpenGL state to SFML before the 2D interactive HUD.
        glViewport(0,0,static_cast<GLsizei>(size.x),static_cast<GLsizei>(size.y));
    }
    void draw() {
        window_.clear(sf::Color(6,14,28));
        window_.pushGLStates();
        renderWorld();
        window_.popGLStates();
        window_.resetGLStates();
        hits_.clear();
        rect(0,0,W,117,sf::Color(9,24,44));
        rect(0,113,W,4,blue);
        rect(0,113,180,4,green);
        logo(22,11,.67f);
        text("JK AXIS  /  VIER GEWINNT",124,22,27,white,true);
        text("GYM EDITION   3D SIGNATURE ARENA",126,65,17,muted);
        rect(1073,117,367,H-117,sf::Color(10,26,47));
        rect(1071,117,2,H-117,sf::Color(45,91,153));
        if(page_==Page::Intro) {
            const float t=gym3d::easeOut(introTime_/2.4f);
            text("DIE ARENA",116,210,55,white,true);
            text("WIRD LEBENDIG.",116,275,55,green,true);
            text("3D  /  PRECISION  /  POWER",118,351,22,muted);
            text(introTime_>1.2f?"HEBEN. SETZEN. SIEGEN.":"",118,399,23,white,true);
            rect(119,454,380*t,4,green);
            text("ENTER ODER KLICK = INTRO UEBERSPRINGEN",118,823,18,muted);
        } else if(page_==Page::Menu) {
            text("DEINE ARENA.",94,192,47,white,true);
            text("DREHEN. PLANEN. GEWINNEN.",94,250,23,green,true);
            text("Echtes 3D-Spielbrett mit freier Kamerasteuerung.",95,297,19,muted);
        } else if(page_==Page::Names) {
            text("DEIN TEAM",94,184,44,white,true);
            text("SPIELER "+to_string(names_.size()+1)+" VON "+to_string(playerCount_),95,249,23,green,true);
            rect(90,312,720,68,sf::Color(19,46,77),blue);
            text(nameInput_+"_",110,326,27,white,true);
            text("NAME EINGEBEN UND ENTER DRUECKEN",94,419,20,muted);
            for(size_t i=0;i<names_.size();++i)
                text(to_string(i+1)+".  "+names_[i],96,486+40.f*i,22,white);
        } else if(page_==Page::Playing||page_==Page::Result) {
            text("DIE 3D-ARENA",88,135,21,white,true);
            if(page_==Page::Result)
                text(match_.winner().empty()?"UNENTSCHIEDEN!":"SIEG: "+match_.winner(),80,806,31,green,true);
            else text(dropActive_?"DEIN STEIN FAELLT ...":"SPALTE "+to_string(selected_+1)+
                 " / "+to_string(match_.board().columns())+"  -  ZUG "+to_string(match_.board().tokenCount()+1),
                 82,821,21,muted);
        } else if(page_==Page::Help) {
            text("3D STEUERUNG",96,196,44,white,true);
            const array<string,8> help={{
                "MAUS LINKS      /  SPALTE SETZEN",
                "MAUS RECHTS    /  KAMERA DREHEN",
                "MAUSRAD            /  ZOOM",
                "PFEILE LINKS/RECHTS / SPALTE",
                "LEERTASTE         /  STEIN EINWERFEN",
                "A / D, Q / E     /  KAMERA",
                "U = UNDO   S = SPEICHERN   C = RESET",
                "F9 = MUSIK   F10/F11 = LAUTSTAERKE"
            }};
            for(size_t i=0;i<help.size();++i)text(help[i],100,275+61.f*i,20,muted);
        } else if(page_==Page::ConfirmExit) {
            text("SPIEL VERLASSEN?",95,222,39,white,true);
            text("Deine laufende Runde ist noch nicht beendet.",96,288,21,muted);
        }
        text("MATCH CONTROL",1100,156,23,white,true);
        rect(1100,196,305,2,blue);
        if(page_==Page::Menu) {
            text("SPIELERANZAHL",1100,221,17,muted,true);
            text(to_string(playerCount_)+" SPIELER  /  "+to_string(7+(playerCount_-2)*2)+"x"+
                 to_string(6+(playerCount_-2)*2),1100,256,23,green,true);
            button("2 SPIELER",1100,302,146,20);
            button("3 SPIELER",1255,302,146,21);
            button("4 SPIELER",1100,362,300,22);
            button("NEUES MATCH",1100,455,300,1);
            button("SPIELSTAND LADEN",1100,515,300,3);
            if(match_.active())button("RUNDE FORTSETZEN",1100,575,300,2);
            button("STEUERUNG",1100,675,300,4);
        } else if(page_==Page::Names) {
            text("GIB DEINEN NAMEN EIN",1100,247,18,muted);
            button("ZURUECK",1100,695,300,5);
        } else if(page_==Page::Playing||page_==Page::Result) {
            text("RUNDE  "+to_string(match_.round()),1100,217,21,muted,true);
            for(size_t i=0;i<match_.players().size();++i) {
                const auto& player=match_.players()[i];
                const float y=265+static_cast<float>(i)*55.f;
                const bool current=page_==Page::Playing&&static_cast<int>(i)==match_.current();
                rect(1099,y,305,48,current?sf::Color(31,75,115):sf::Color(17,38,61));
                rect(1101,y+7,7,33,colors[i]);
                text(player.getName().substr(0,17),1116,y+5,17,current?white:muted,true);
                text(to_string(match_.wins(player.getName()))+" W",1328,y+7,17,colors[i],true);
            }
            const float buttonY=510;
            if(page_==Page::Playing) {
                button("STEIN SETZEN",1100,buttonY,300,8,!dropActive_);
                button("RUECKGAENGIG",1100,buttonY+60,300,7,!dropActive_);
                button("SPEICHERN",1100,buttonY+120,300,6,!dropActive_);
            } else {
                button("REVANCHE",1100,buttonY+40,300,9);
            }
            button("HAUPTMENUE",1100,743,300,5);
        } else if(page_==Page::Help) {
            button("ZURUECK",1100,734,300,5);
        } else if(page_==Page::ConfirmExit) {
            button("SICHERN UND BEENDEN",1100,422,300,14);
            button("OHNE SICHERN BEENDEN",1100,484,300,13);
            button("ABBRECHEN",1100,546,300,15);
        }
        rect(1095,812,310,1,sf::Color(47,94,143));
        button(music_?"MUSIK PAUSE":"MUSIK START",1099,825,177,10);
        button("-",1282,825,55,11);
        button("+",1343,825,55,12);
        text("VOL "+to_string(volume_)+"%",1099,780,16,music_?green:muted,true);
        if(messageTime_>0.f)text(info_.substr(0,87),68,875,15,green);
        window_.display();
    }
};
} // namespace

int main(int argc,char** argv) {
    try {
        if(argc>1 && string(argv[1])=="--self-test") {
            MatchSession sample;
            if(!sample.begin({"Ada","Ben"})||!sample.drop(0)||
                sample.board().tokenCount()!=1)return 1;
            const auto projection=gym3d::project({0,0,0},{0,.1f,16.f},{540,480},780);
            if(!projection.visible||!isfinite(projection.x))return 2;
            cout<<"Gym Edition 3D scene and shared match core: OK\n";
            return 0;
        }
        Arena3D app;
        if(argc>1&&string(argv[1])=="--window-smoke")app.run(12);
        else app.run();
        return 0;
    } catch(const exception& error) {
        cerr<<"Gym Edition 3D: "<<error.what()<<'\n';
        return 1;
    }
}

//
//  Spaceship.cpp
//  hw3
//
//  Created by Michael Felix on 10/25/23.
//

#include "Spaceship.hpp"


Spaceship::Spaceship(float x, float y)
:   Object(x, y, 0.f),
    GraphicObject(x, y, 0.f),
    AnimatedObject(x, y, 0.f, 0.f, 0.f, 0.f),
    red_(0.5f),
    green_(0.5f),
    blue_(0.5f)

{
    
}


void Spaceship::draw() const {
    glPushMatrix();
    
    //    move to the center of the disk
    glTranslatef(getX(), getY(), 0.f);
    
    // apply rotation
    glRotatef(getAngle(), 0.f, 0.f, 1.f);
    
    //    apply the radius as a scale
    glScalef(1.f, 1.f, 1.f);
    
    glColor3f(red_, green_, blue_);
    glBegin(GL_POLYGON);
    // spaceship model
    glVertex2f(0.0f, 0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.2f, -0.3f);
    glVertex2f(-0.2f, -0.3f);
    glVertex2f(-0.5f, -0.5f);
    glEnd();
    
    //    restore the original coordinate system (origin, axes, scale)
    glPopMatrix();
}


void Spaceship::update(float dt) {
    
}

bool Spaceship::isInside(const WorldPoint& pt)
{
    return false;
}
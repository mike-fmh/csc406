//
//  PolyEarRight.hpp
//  Version1
//
//  Created by Michael Felix on 9/28/23.
//

#ifndef PolyEarRight_hpp
#define PolyEarRight_hpp

#include "PolyShape.hpp"
#include <stdio.h>
#include <string>

class PolyWingRight: public PolyShape {
private:
    // shape points loaded from file shapeCoords.txt in loadShape()
    const char* _myLoadedFilepath;
    static int _numLoadedPnts;
    static const int _maxLoadedPnts; // stop loading from the file after this limit is reached
    static float** _loadedShapePnts; // Nx2 float
    
public:
    PolyWingRight(std::string filepath, float centerX, float centerY, float angle, float scaleX, float scaleY, float r, float g, float b);
    ~PolyWingRight();
    
    void draw() const override;
    
    void initFromPolygon();
    void initFromFile(const char* filepath);
    //disabled constructors & operators
    PolyWingRight() = delete;
    PolyWingRight(const PolyWingRight& obj) = delete;    // copy
    PolyWingRight(PolyWingRight&& obj) = delete;        // move
    PolyWingRight& operator = (const PolyWingRight& obj) = delete;    // copy operator
    PolyWingRight& operator = (PolyWingRight&& obj) = delete;        // move operator
};

#endif /* PolyEarRight_hpp */

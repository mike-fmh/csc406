//
//  House3D.cpp
//  hw4
//
//  Created by Michael Felix on 11/21/23.
//

#include "House3D.h"

using namespace graphics3d;

std::vector<unsigned int> House3D::faceVertexCounts_;
unsigned int House3D::numFaces_ = 0;


House3D::House3D(float scaleX, float scaleY, const Pose& pose, const Motion& motion)
:   GraphicObject3D(pose, motion),
    XYZ_(nullptr),
    scaleX_(scaleX),
    scaleY_(scaleY)
{
    initFromFile_("/Users/michaelfelix/Desktop/house.obj");
}


void House3D::initFromVectors_(std::vector<std::vector<float>>& vertices, std::vector<std::vector<int>>& faces) {
    numFaces_ = (int)faces.size();
    
    XYZ_ = new GLfloat**[numFaces_];
    for (unsigned int i = 0; i < numFaces_; i++) {
        XYZ_[i] = new GLfloat*[faces[i].size()];
        faceVertexCounts_.push_back((int)faces[i].size());
        for (unsigned int j = 0; j < faces[i].size(); j++) {
            XYZ_[i][j] = new GLfloat[3];
            XYZ_[i][j][0] = vertices[faces[i][j]][0];
            XYZ_[i][j][1] = vertices[faces[i][j]][1];
            XYZ_[i][j][2] = vertices[faces[i][j]][2];
            
           // std::cout << "(" << XYZ_[i][j][0] << ", " << XYZ_[i][j][1] << "," << XYZ_[i][j][2]<< "," << ") ";
        }
        //std::cout << std::endl;
    }
}


void House3D::initFromFile_(const char* filepath) {
    // load from an obj file
    std::ifstream file_data(filepath);
    std::vector<std::vector<float>> vertices;
    std::vector<std::vector<int>> faces;
    if (!file_data.is_open()) {
        std::cout << "Error: Unable to open file " << filepath << std::endl;
        // if the file can't be opened, load the hard-coded values instead
        return;
    }
    
    std::string tempVal = "";
    std::string firstVal = "";
    std::string curVal = "";
    std::string line;
    bool vertex = false;
    bool face = false;
    
    float x, y, z;
    char letter;
    
    std::vector<int> thisFace;
    while (std::getline(file_data, line)) {
        for (int i = 0; i < line.size(); i++) {
            letter = line.at(i);
            if ((!vertex) && (!face)) {
                if ((letter == *"v") && (i == 0)) { // line starts with a v
                    vertex = true;
                }
            } else if ((vertex) && (!face)) {
                if ((line.at(0) == *"f") && (line.at(1) == *" ")) { // line starts with 'f '
                    vertex = false;
                    face = true;
                    continue;
                } else if ((line.at(0) != *"v") && (line.at(1) != *" ")) { // line doesn't start with v
                    continue;
                }
                if (letter == *" ") {
                    // space means a value ended
                    if (firstVal == "") {
                        try {
                            // if this value is a valid float, take it as the first vertex coord
                            stof(tempVal);
                            firstVal = tempVal;
                        } catch (const std::invalid_argument& ia) {
                            firstVal = "";
                        }
                    } else {
                        curVal = tempVal;
                    }
                    tempVal = "";
                } else {
                    tempVal += letter;
                }
            } else if ((!vertex) && (face)) {
                if ((line.at(0) != *"f") && (line.at(1) != *" ")) { // line doesn't start with 'f '
                    continue;
                }
                if (letter == *" ") {
                    // space means a value ended
                    try {
                        // if this value is a valid int, take it as a value
                        
                        thisFace.push_back(stoi(tempVal) - 1);
                        // when faces are listed in obj files they index starting at 1
                        // but vectors are indexed starting at 0
                    } catch (const std::invalid_argument& ia) {}
                    tempVal = "";
                } else {
                    tempVal += letter;
                }
            }
        }
        // end of the line
        
        if ((vertex) && (!face)) {
            try {
                x = stof(firstVal);
                y = stof(curVal);
                z = stof(tempVal);
                vertices.push_back(std::vector<float>{x, y, z});
             //   std::cout << firstVal << ", " << curVal << ", " << tempVal << std::endl;
            } catch (const std::invalid_argument& ia) {} // no conversion in stof()
        } else if ((!vertex) && (face)) {
            if (thisFace.size() > 0) {
                try {
                    // add final face value
                    thisFace.push_back(stoi(tempVal) - 1);
                } catch (const std::invalid_argument& ia) {}
                faces.push_back(thisFace);
                
                /*
                std::cout << "(" << thisFace.size() << ")\n";
                for (int j = 0; j < thisFace.size(); j++) {
                    std::cout<< thisFace.at(j) << ", ";
                }
                std::cout << std::endl;
                */
                
                thisFace.clear();
            }
        }
        firstVal = "";
        curVal = "";
        tempVal = "";
    }
    initFromVectors_(vertices, faces);
}


House3D::~House3D() {
    for (unsigned int i=0; i<numFaces_; i++)
    {
        for (unsigned int j=0; j<faceVertexCounts_[i]; j++)
        {
            delete []XYZ_[i][j];
        }
        delete []XYZ_[i];
    }
    delete []XYZ_;
}


void House3D::draw() const
{
    glPushMatrix();
    applyPose();

    setCurrentMaterial(getMaterial());

    for (unsigned int i=0; i<numFaces_; i++)
    {
        glBegin(GL_TRIANGLE_STRIP);
            for (unsigned int j=0; j<faceVertexCounts_[i]; j++)
            {
                glVertex3fv(XYZ_[i][j]);
            }
            
        glEnd();
    }

    glPopMatrix();
}
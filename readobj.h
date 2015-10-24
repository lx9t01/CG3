#pragma once
#include <string>
#include <vector>
#include </usr/local/include/Eigen/Dense>
#include <stdio.h>

using namespace Eigen;
using namespace std;

struct vertex {
  float x, y, z, w;
  vertex(float x1, float y1, float z1, float w1) {
    x = x1;
    y = y1;
    z = z1;
    w = w1;
  }
};

struct vertexn{
    float x, y, z, w;
    vertexn(float x1, float y1, float z1 , float w1) {
        x = x1;
        y = y1;
        z = z1;
        w = w1;
    }
};

struct face {
  int a, b, c;
    int an, bn, cn;
  face(int a1, int a2, int b1, int b2, int c1, int c2) {
    a = a1;
    b = b1;
    c = c1;
      an=a2;
      bn=b2;
      cn=c2;
  }
};

struct light{
    float pos[3];
    int color[3];
    float k;
};

class GraphObj {
 public:
  GraphObj(char* s);
  GraphObj();
  bool readObj(char const* s);
  Matrix4f readTrans(char const* s);
  Matrix4f rotation (float a, float b, float c, float d);
  Matrix4f translation (float a, float b, float c);
  Matrix4f scaling (float a, float b, float c);
  string getName();//readobj.cpp
  unsigned int getNumber();//readobj.cpp
  bool setName(string s);//readobj.cpp
  bool setNumber(unsigned int n);//readobj.cpp
    string getPrintName();
    bool setPrintName(string s);//readobj.cpp
    int dupMore();//readobj.cpp
    int getDupNumber();//readobj.cpp
  Matrix4f matrix=Matrix4f::Identity(4,4);
    Matrix4f inv_matrix=Matrix4f::Zero(4,4);
    vector<vertex> getVertex();//readtrans.cpp
    vector<face> getFace();//readtrans.cpp
    bool setVertex(vector<vertex>);
    bool setVertex_NDC(vector<vertex> v1);
    vector<vertex> getVertex_NDC();
  


 private:
  string objname;
    string printname;
  unsigned int objnumber;
  int dupnumber=0;
  vector<vertex> v;
  vector<face> f;
    vector <vertex> v_NDC;
    vector<vertexn> vn;
    
    
};

class camera{
    
public:
    Matrix4f setInvC(float cam_p[3], float cam_o[4]);
    Matrix4f getInvC();
    
private:
    Matrix4f inv_C=Matrix4f::Identity(4,4);
    
};
#include <iostream>
#include <sstream>
#include <fstream>
#include "readobj.h"

using namespace std;
using Eigen::MatrixXd;

void split(string s, int &s1, int &s2);

GraphObj::GraphObj(char* s){
  ifstream objfile(s);
  string ch;
  string x, y, z;
    string str;
    int a1,b1,c1,a2,b2,c2;
    
  if (objfile.is_open()) {
    while (!objfile.eof()) {
      objfile >> ch >> x >> y >> z;
      if (ch == "v") {
        v.push_back(vertex(stof(x), stof(y), stof(z), 1));
      } else if (ch == "f") {
          split(x,a1,a2);
          split(y,b1,b2);
          split(z,c1,c2);
          //cout<<"ao: "<<a1<<" "<<a2<<endl;
          f.push_back(face(a1, a2, b1, b2, c1, c2));
      } else if(ch == "vn"){
          vn.push_back(vertexn(stof(x), stof(y), stof(z), 1));
      }
        else
        cout << "Fail to read obj line";
      }
    
    f.pop_back(); // one more last line
    objfile.close();
  } else {
    cout << "File not open.\n";
  }
  //cout << "Now read the obj file.\n";
//  for (vector<vertex>::const_iterator i = v.begin(); i !=v.end(); ++i)
//  {
//   cout << "v1 " << i->x << i->y << i->z <<"\n";
//  }
//  for (vector<face>::const_iterator i = f.begin(); i !=f.end(); ++i)
//  {
//   cout << "f1 " << i->a << i->b << i->c <<"\n";
//  }
}

GraphObj::GraphObj(){
  
}

bool GraphObj::readObj(char const* s) {
  ifstream objfile(s);
  // vector <string> fields;
  char ch;
  float x, y, z;

  if (objfile.is_open()) {
    while (!objfile.eof()) {
      // read in a line of .obj
      // cout << line <<"\n";
      // split(fields, line, " ");// split a line in .obj
      // print(fields);
      objfile >> ch >> x >> y >> z;
      if (ch == 'v') {
        v.push_back(vertex(x, y, z, 1));
        // cout << "v1 " << v.back().x << "," << v.back().y << "," << v.back().z
        // << "\n";
      } else if (ch == 'f') {
        //f.push_back(face((int)x, (int)y, (int)z));
        // cout << "f1 " << f.back().a << "," << f.back().b << "," << f.back().c
        // << "\n";
      } else {
        cout << "Fail to read obj line";
      }
    }
    f.pop_back(); // one more last line
    objfile.close();
  } else {
    cout << "File not open.\n";
    return false;
  }
  // cout << "Now read the obj file.\n";
  // for (vector<vertex>::const_iterator i = v.begin(); i !=v.end(); ++i)
  // {
  // 	cout << "v1 " << i->x << i->y << i->z <<"\n";
  // }
  // for (vector<face>::const_iterator i = f.begin(); i !=f.end(); ++i)
  // {
  // 	cout << "f1 " << i->a << i->b << i->c <<"\n";
  // }
  return true;
}

string GraphObj::getName(){
  return objname;

}

unsigned int GraphObj::getNumber(){
  return objnumber;

}

bool GraphObj::setName(string s){
  objname=s;
  return true;
}

bool GraphObj::setNumber(unsigned int n){
  objnumber=n;
  return true;
}

bool GraphObj::setPrintName(string s){
    printname=s;
    return true;
}

string GraphObj::getPrintName(){
    return printname;
}

int GraphObj::dupMore(){
    dupnumber++;
    return dupnumber;
}

int GraphObj::getDupNumber(){
    return dupnumber;
}

void split(string s, int &s1, int &s2){
    size_t pos=s.find("//");
    string sub1=s.substr(pos-1,1);
    string sub2=s.substr(pos+2,1);
    s1=atoi(sub1.c_str());
    s2=atoi(sub2.c_str());
    
}

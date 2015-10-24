//
//  main.cpp
//  hw0_3
//
//  Created by 徐海翔 on 15/10/10.
//  Copyright (c) 2015年 徐海翔. All rights reserved.
//

#include "readobj.h"
#include <iostream>
#include </usr/local/include/Eigen/Dense>
#include <sstream>
#include <fstream>

using namespace std;
using namespace Eigen;

int xres,yres;
GraphObj magic(GraphObj obj);
GraphObj magic2(GraphObj obj, Matrix4f a); // also used for cam2ndc
// GraphObj cam2NDC(GraphObj obj, Matrix4f a);<---identical to magic2


int First_Octant_Bresenham(int x0,int y0,int x1,int y1, int** grid);


int main(int argc, char** argv) {
    ifstream graphfile("scene_cube2.txt");
    xres=800;//atoi
    yres=800;
    
    
    vector<GraphObj> graph;//set of graph objs
    vector<light> lightsource;
    
    string line;
    string objname,objfilename;
    unsigned int flag=0;
    unsigned int number=0;
    float a[4];
    int i=0;
    string nametemp, stringtemp, command;
    int numtemp=0;
    Matrix4f m=Matrix4f::Identity(4,4);
    
    float cam_p[3]={0,0,0};
    float cam_o[4]={0,0,0,0};
    float n=0, f=0, l=0, r=0, t=0, b=0;
    
    light light1;
    
    if (graphfile.is_open()){
        while(getline(graphfile,line)){//read each line
            //read camera
            if (line.size() >0 && flag ==0){
                getline(graphfile, line);//camera:
                stringstream ss(line);
                getline(ss, command, ' ');
                for (int i=0;i<3;i++){
                    getline(ss, stringtemp, ' ');
                    stringstream(stringtemp)>>cam_p[i];
                }
                
                getline(graphfile, line);
                stringstream ss1(line);
                getline(ss1, command, ' ');
                for (int i=0;i<4;i++){
                    getline(ss1, stringtemp, ' ');
                    stringstream(stringtemp)>>cam_o[i];
                }
                
                getline(graphfile, command, ' ');
                getline(graphfile, stringtemp);
                stringstream(stringtemp)>>n;
                getline(graphfile, command, ' ');
                getline(graphfile, stringtemp);
                stringstream(stringtemp)>>f;
                getline(graphfile, command, ' ');
                getline(graphfile, stringtemp);
                stringstream(stringtemp)>>l;
                getline(graphfile, command, ' ');
                getline(graphfile, stringtemp);
                stringstream(stringtemp)>>r;
                getline(graphfile, command, ' ');
                getline(graphfile, stringtemp);
                stringstream(stringtemp)>>t;
                getline(graphfile, command, ' ');
                getline(graphfile, stringtemp);
                stringstream(stringtemp)>>b;
                
            }else if (line.size()==0 && flag==0){//read in the obj files
                flag=1;
                getline(graphfile,line);
            }
            
            // read in lights
            if(line.size()!=0 && flag==1){
                string lighttemp;
                stringstream ss_l(line);
                getline(ss_l, stringtemp, ' ');
                cout<<stringtemp<<"\n";
                
                getline(ss_l, stringtemp, ',');// , as separation. xyz
                cout<<stringtemp<<"\n";
                stringstream ss_l1(stringtemp);
                for (int i=0;i<3;i++){
                    getline(ss_l1, lighttemp, ' ');
                    cout<<lighttemp<<"\n";
                    stringstream(lighttemp)>>light1.pos[i];
                    cout<<light1.pos[i]<<"\n";
                }
                
                getline(ss_l, stringtemp, ',');// , as separation. rgb
                cout<<stringtemp<<"\n";
                stringstream ss_l2(stringtemp);
                getline(ss_l2, lighttemp, ' ');// avoid one more ' '
                for (int i=0;i<3;i++){
                    getline(ss_l2, lighttemp, ' ');
                    cout<<lighttemp<<"\n";
                    stringstream(lighttemp)>>light1.color[i];
                    cout<<light1.color[i]<<"\n";
                }
                
                getline(ss_l, stringtemp);// , as separation. k
                cout<<stringtemp<<"\n";
                stringstream ss_l3(stringtemp);
                ss_l3>>light1.k;
                cout<<light1.k<<"\n";
                
                lightsource.push_back(light1);
            }else if(line.size()==0 && flag==1){
                flag=2;
                getline(graphfile,line);
            }
            
            
            
            
            if(flag==2&&line.size()>0&&line!="objects:"){
                stringstream ss(line);
                if (getline(ss, objname,' ')){
                    getline(ss, objfilename);
                    
                    char* cstr = new char[objfilename.length()+1];
                    strcpy(cstr, objfilename.c_str());
                    
                    GraphObj obj(cstr);//define a object named 'obj'??????name???indexing!!!
                    delete cstr;
                    stringstream ss2;
                    ss2<<objname<<"_copy_1";
                    obj.setPrintName(ss2.str());
                    obj.setName(objname);
                    obj.setNumber(number);//total number of objs
                    number++;
                    graph.push_back(obj);
                }
            }else if (flag==2&&line.size()==0){
                flag=3;
            }
            
            if(flag==3&&line.size()==0){
                getline(graphfile,line);
                stringtemp=line;
                for (numtemp=0;numtemp<graph.size();numtemp++){
                    if (stringtemp==graph[numtemp].getName()){
                        graph[numtemp].dupMore();
                        if (graph[numtemp].getDupNumber()>1){
                            GraphObj newobj;
                            newobj=graph[numtemp];
                            stringstream ss1;
                            ss1<<graph[numtemp].getName()<<"_copy_"<<graph[numtemp].getDupNumber();
                            newobj.setPrintName(ss1.str());
                            newobj.setName(ss1.str());
                            newobj.setNumber(number);
                            numtemp=number;//IMPORTANT!!!!!!!!!
                            number++;
                            newobj.matrix=Matrix4f::Identity(4,4);
                            graph.push_back(newobj);
                        }
                        break;
                    }
                }//for
            }else if(flag==3&&line.size()>0){
                stringstream ss(line);
                getline(ss, stringtemp, ' ');
                //cout<<"stringtemp: "<<stringtemp<<endl;
                if (stringtemp=="t"){
                    for (i=0;i<3;i++){
                        getline(ss, stringtemp,' ');
                        stringstream(stringtemp)>>a[i];
                    }
                    m=graph[numtemp].translation(a[0],a[1],a[2]);
//                    cout<<a[0]<<a[1]<<a[2]<<"\n";
//                    cout<<m<<"\n";
                    graph[numtemp].matrix=m*graph[numtemp].matrix;
//                    cout<<graph[numtemp].matrix;
                    graph[numtemp].inv_matrix=graph[numtemp].matrix.inverse();
                
//                    cout<<graph[numtemp].matrix<<"\n";
//                    cout<<graph[numtemp].getName()<<endl;
                    
                }else if (stringtemp=="r"){
                    for (i=0;i<4;i++){
                        getline(ss, stringtemp,' ');
                        stringstream(stringtemp)>>a[i];
                    }
                    m=graph[numtemp].rotation(a[0],a[1],a[2],a[3]);
                    graph[numtemp].matrix=m*graph[numtemp].matrix;
//                    cout <<"roa "<<m<<endl<<numtemp<<endl;
//                    cout<<graph[numtemp].getName()<<endl;
                    graph[numtemp].inv_matrix=graph[numtemp].matrix.inverse();
                    
                }else if (stringtemp=="s"){
                    for (i=0;i<3;i++){
                        getline(ss, stringtemp, ' ');
                        stringstream(stringtemp)>>a[i];
                    }
                    m=graph[numtemp].scaling(a[0],a[1],a[2]);
                    graph[numtemp].matrix=m*graph[numtemp].matrix;
                    graph[numtemp].inv_matrix=graph[numtemp].matrix.inverse();
//                    cout<< "sca "<<m<<endl<<numtemp<<endl;
//                    cout<<graph[numtemp].getName()<<endl;
                    
                }else{
                    cout << "No such transform!\n";
                }//if flag=2 && not empty line
            }
            
        }
        //end of read
        

        //end of output obj vertex
        
        
    }else{
        cout <<"Graph file not open.\n";
        return 1;
    }//if open graph file
    
    //==================perform trasfroamtion======================
    vector<GraphObj> graph_after_trans;//set of graph objs
            for (numtemp=0;numtemp<graph.size();numtemp++){
//                cout<<graph[numtemp].getPrintName()<<"\n";
//                cout<<graph[numtemp].matrix<<"\n";
//                cout<<graph[numtemp].inv_matrix<<"\n";
                GraphObj objout;
                objout=magic(graph[numtemp]);
                //cout<<objout.getPrintName()<<"\n";
                vector<vertex> v1;
                v1=objout.getVertex();
//                for (i=0;i<v1.size();i++){
//                    cout<<v1[i].x/v1[i].w<<" "<<v1[i].y/v1[i].w<<" "<<v1[i].z/v1[i].w<<"\n";
//                    cout<<v1[i].w<<"\n";
//                }
                graph_after_trans.push_back(objout);
                graph_after_trans[numtemp].matrix=graph[numtemp].matrix;
                graph_after_trans[numtemp].inv_matrix=graph[numtemp].inv_matrix;
                graph_after_trans[numtemp].setPrintName(graph[numtemp].getPrintName());
            }
    
    //==================camera=====================================
    camera c1;
    Matrix4f temp=c1.setInvC(cam_p, cam_o);
//    cout<< "temp: "<<temp<<"\n";
    vector<GraphObj> graph_after_camera;//set of graph objs
    //output objs' vertex
    for (numtemp=0;numtemp<graph_after_trans.size();numtemp++){
        GraphObj objout;
        objout=magic2(graph_after_trans[numtemp],temp);
        vector<vertex> v1;
        v1=objout.getVertex();
//        cout<<"after camera trans\n";
//        for (i=0;i<v1.size();i++){
//            cout<<v1[i].x/v1[i].w<<" "<<v1[i].y/v1[i].w<<" "<<v1[i].z/v1[i].w<<"\n";
//            cout<<v1[i].w<<"\n";
//        }
        objout.setVertex(v1);// I FORGOT THIS ONE!!!
        graph_after_camera.push_back(objout);//AND THIS ONE!!!
        graph_after_camera[numtemp].matrix=graph[numtemp].matrix;
        graph_after_camera[numtemp].inv_matrix=graph[numtemp].inv_matrix;
        graph_after_camera[numtemp].setPrintName(graph[numtemp].getPrintName());
    }
    //end of output, Section 2 finished
    
    //==============cam2ndc=======================================
    Matrix4f P=Matrix4f::Zero(4, 4);//P: Camera Space to NDC;
    vector<GraphObj> graph_after_NDC;//set of graph objs
    P<<2*n/(r-l),0,(r+l)/(r-l),0,
    0,2*n/(t-b),(t+b)/(t-b),0,
    0,0,-(f+n)/(f-n),-2*f*n/(f-n),
    0,0,-1,0;
//    cout<<"P: \n"<<P<<"\n";
    //project to NDC
    for (numtemp=0;numtemp<graph_after_camera.size();numtemp++){
        GraphObj objout;
        objout=magic2(graph_after_camera[numtemp],P);
        //cout<<objout.getPrintName()<<"\n";
        vector<vertex> v1;
        v1=objout.getVertex();
//        cout<<"cam2NDC\n";
//        for (i=0;i<v1.size();i++){
//            cout<<v1[i].x/v1[i].w<<" "<<v1[i].y/v1[i].w<<" "<<v1[i].z/v1[i].w<<"\n";
//            cout<<v1[i].w<<"\n";
//        }
        objout.setVertex(v1);
        graph_after_NDC.push_back(objout);
        graph_after_NDC[numtemp].matrix=graph[numtemp].matrix;
        graph_after_NDC[numtemp].inv_matrix=graph[numtemp].inv_matrix;
        graph_after_NDC[numtemp].setPrintName(graph[numtemp].getPrintName());

    }
    //end of output NDC, Section 3 finished
    //not check whether inside the frustum yet
    
    //==============ndc2screen====================================

    int** grid;
    //grid matrix!----------------------------------
    
    grid=new int*[yres];
    for (int i=0;i<yres;i++){
            grid[i]=new int[xres];
    }
    
    
    //Proj to 2D
    for (int i=0;i<graph_after_NDC.size();i++){
        GraphObj objtemp=graph_after_NDC[i];
        vector<vertex> v1;
        vector<face> f1;
        vector <int> x;
        vector <int> y;
        
        v1=objtemp.getVertex();
        f1=objtemp.getFace();
        for (int j=0;j<v1.size();j++){
            int temp=0;
            temp=(int)xres/2*v1[j].x/v1[j].w;
            x.push_back(temp);
            //cout<<temp<<"\n";
            temp=(int)yres/2*v1[j].y/v1[j].w;
            y.push_back(temp);
            //cout<<temp<<"\n";
            
        }
        for (int j=0;j<f1.size();j++)
        {
            int a,b,c;
            a=f1[j].a-1;//#of vertex starts from 0;
            b=f1[j].b-1;
            c=f1[j].c-1;
//            cout<<"a "<<a<<" b "<<b<<" c "<<c<<"\n";
//            cout<<"xa "<<x[a]<<" xb "<<x[b]<<" xc "<<x[c]<<"\n";
//            cout<<"ya "<<y[a]<<" yb "<<y[b]<<" yc "<<y[c]<<"\n";
            First_Octant_Bresenham(x[a], y[a], x[b], y[b], grid);
            First_Octant_Bresenham(x[b], y[b], x[c], y[c], grid);
            First_Octant_Bresenham(x[a], y[a], x[c], y[c], grid);
            
        }
    }
    
    //Output Grid
    //write files=========================================
    ofstream outfile;
    outfile.open("/Users/Alex/Documents/CPPtrial/hw1/out_bunny_closeup.ppm");
    outfile<<"P3\n";
    outfile<<xres<<" "<<yres<<"\n";
    outfile<<"255\n";
    
    for (int i=yres-1;i>=0;i--){//!!!!!bug probably
        for (int j=0;j<xres;j++){
            cout<<grid[i][j]<<" ";
            if (grid[i][j]==1)
                outfile<< "255 255 255"<<"\n";
            else
                outfile<< "0 0 0"<<"\n";
        }
        cout<<"\n";
    }
    
    outfile.close();

    for (int i=0;i<yres;i++){
        delete[] grid[i];
    }
    delete[] grid;
    //-----------------------------------------------------------
    

    return 0;
}
//=================================================================

GraphObj magic(GraphObj obj){
    vector<vertex> v1,v2;
    v1=obj.getVertex();
    for (int i=0;i<v1.size();i++){
        Vector4f a(4);
        a(0)=v1[i].x;
        a(1)=v1[i].y;
        a(2)=v1[i].z;
        a(3)=v1[i].w;
        a=obj.matrix*a;
        //cout<<a;
        v2.push_back(vertex(a(0),a(1),a(2),a(3)));
    }
    obj.setVertex(v2);

    //cout << v2.size();
    return obj;
}

GraphObj magic2(GraphObj obj, Matrix4f a){//also used for cam2NDC
    vector<vertex> v1=obj.getVertex();
    vector<vertex> v2;
    Vector4f b(4);
    
    for (int i=0;i<v1.size();i++){
        b(0)=v1[i].x;
        b(1)=v1[i].y;
        b(2)=v1[i].z;
        b(3)=v1[i].w;
//        cout<<"b0: "<<b<<"\n";
//        cout<<"a: "<<a<<"\n";
        b=a*b;
        v2.push_back(vertex(b(0),b(1),b(2),b(3)));
//        cout<<"b: "<<b<<"\n";
    }
    obj.setVertex(v2);
    
    return obj;
    
}


int First_Octant_Bresenham(int x0,int y0,int x1,int y1, int **grid){
    int e=0;
    int dx,dy=0;
    if(x1<x0){
        int temp;
        temp=x1;x1=x0;x0=temp;
        temp=y1;y1=y0;y0=temp;
    }// swap, x1>=x0
    dx=x1-x0;
    //cout<<"x0: "<<x0<<"y0: "<<y0<<"x1: "<<x1<<"y1: "<<y1<<"\n";
    if (y1>=y0){
        dy=y1-y0;
        if (dy>dx){
            int x=x0;
            for (int y=y0;y<y1;y++){
                if ((abs(x)<xres/2)&&(abs(y)<yres/2)){
                    grid[y+yres/2][x+xres/2]=1;
                    if(2*e+dx<dy){
                        e=e+dx;
                    }else{
                        e=e+dx-dy;
                        x=x+1;
                    }
//                    cout<<x<<" "<<y<<"\n";
                }else
                    break;
            }
        }else{
            int y=y0;
            for (int x=x0;x<x1;x++){
                if ((abs(x)<xres/2)&&(abs(y)<yres/2)){
                    grid[y+yres/2][x+xres/2]=1;
                    if (2*(e+dy)<dx){
                        e=e+dy;
                    }else {
                        e=e+dy-dx;
                        y=y+1;
                    }
//                    cout<<x<<" "<<y<<"\n";
                }else
                    break;
            }
        }
    }else{
        dy=y0-y1;
        if (dy>dx){
            int x=x1;
            for (int y=y1;y<y0;y++){
                if((abs(x)<xres/2)&&(abs(y)<yres/2)){
                    grid[y+yres/2][x+xres/2]=1;
                    if (2*e+dx<dy){
                        e=e+dx;
                    }else{
                        e=e+dx-dy;
                        x=x-1;
//                        cout<<x<<" "<<y<<"\n";
                    }
                }else
                    break;
            }
        }else{
            int y=y0;
            for (int x=x0;x<x1;x++){
                if((abs(x)<xres/2)&&(abs(y)<yres/2)){
                    grid[y+yres/2][x+xres/2]=1;
                    if (2*(e+dy)<dx){
                        e=e+dy;
                    }else {
                        e=e+dy-dx;
                        y=y-1;
                    }
//                    cout<<x<<" "<<y<<"\n";
                }else
                    break;
            }
            
        }
        
    }
    
    return 0;
}













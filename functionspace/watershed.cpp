#include "../headerspace/WatershedAlg.h"
#include<vector>
#include<math.h>
#include<cmath>
#include <limits>
#include<list>
#include"./mergeRegion.cpp"
using namespace cv;
#define NUMSIZE 8
#define NSIZE 4




struct Pixel {
    int val;
    int x;
    int y;

    Pixel(int val, int x, int y) : val(val), x(x), y(y) {}


};


struct cmp1{

   bool operator()(Pixel &para1, Pixel &para2) {
        return para1.val < para2.val;
    }

};


struct nngNode{

int x;
int y;
int pixelval;
int pixelnum;
double ndist;
nngNode(int x, int y, int pixelval,int pixelnum,double ndist) : x(x), y(y), pixelval(pixelval),pixelnum(pixelnum),ndist(ndist) {}

};

struct cmp2{

   bool operator()(nngNode &para1, nngNode &para2) {
        return para1.ndist < para2.ndist;
    }

};


Mat WatershedAlg::watershed(Array2D<int> &image, Array2D<int>& markers,Mat &duplImage,int &rows,int &cols,Array2D<bool> &inprioq,Array2D<int>& markerMap,Array2D<int>&temp,Array2D<int>& nextSet, int &id_num,Array2D<int>&testMat){


       int msize=0;
  
       int dx[NUMSIZE]={-1, 1, 0, 0, -1, -1, 1, 1};
       int dy[NUMSIZE]={0, 0, -1, 1, -1,  1, 1, -1};

        // Put markers in priority queue
        int id = 1;
        Mat testDuplicate;
        duplImage.copyTo(testDuplicate);
        Mat dstImage(duplImage.rows,duplImage.cols,CV_8UC3,Scalar::all(0));


int tempcounter=0;
/*
   for(int y=0;y<rows;y++){

      for(int z=0;z<cols;z++){
 
           if(markers(y,z)==2){    
            markerMap(y,z) = id;
//	    image(y,z)=image(y,z)+25;
           // msize++;
             for(int i = 0; i < 4; i++) {

                int newX =y + dx[i];
                int newY =z + dy[i];
                if(newX < 0 || newY < 0 || newX >= rows || newY >= cols) {
                    continue;
                }

	       temp(tempcounter,0)=image(newX,newY);
               temp(tempcounter,1)=newX;
               temp(tempcounter,2)=newY;
              // prioq.push(temp(tempcounter));
	       tempcounter++;

                
             }

             id++;
           }
        }
   }
*/
//add priority queue
priority_queue<Pixel,vector<Pixel>,cmp1>markerpq;
int markercounter=0;

   for(int y=0;y<rows;y++){

      for(int z=0;z<cols;z++){
 
           if(markers(y,z)==2){
           //  markerMap(y,z) = id
             markerpq.push({image(y,z),y,z});
             markercounter++;
             //id++;
        }
   }
}

//int ** __restrict platmarker=new int*[rows];
/*
for(int i=0;i<rows;i++){
   platmarker[i]=new int[cols];
   for(int j=0;j<cols;j++){
   platmarker[i][j]=0;
   }
}
*/
/*
Pixel ** __restrict pixelmat=new Pixel*[markercounter];

for(int i=0;i<markercounter;i++){
   pixelmat[i]=new Pixel[markercounter];
   for(int j=0;j<markercounter;j++){
   pixelmat[i][j].val=-1;
   }
}

*/
Pixel **pixelmat=(Pixel**)malloc(markercounter*sizeof(Pixel*));
for(int h=0;h<markercounter;h++){
   pixelmat[h]=(Pixel*)malloc(markercounter*sizeof(Pixel));
   for(int k=0;k<markercounter;k++){
     pixelmat[h][k].val=-1;
   }

}



int tempval=markerpq.top().val;
int pixelcols=0;
int pixelmatid=1;

for(int i=0;i<markercounter;i++){
 
if(!markerpq.empty()){
   
     if(markerpq.top().val==tempval){
       pixelmat[pixelmatid][pixelcols].val=markerpq.top().val;
       pixelmat[pixelmatid][pixelcols].x=markerpq.top().x;
       pixelmat[pixelmatid][pixelcols].y=markerpq.top().y;
       markerMap(pixelmat[pixelmatid][pixelcols].x,pixelmat[pixelmatid][pixelcols].y)=pixelmatid;
       tempval=pixelmat[pixelmatid][pixelcols].val;
       pixelcols++;
       markerpq.pop();
   }
    else{
        pixelmatid++;
        pixelcols=0;
        pixelmat[pixelmatid][pixelcols].val=markerpq.top().val;
        pixelmat[pixelmatid][pixelcols].x=markerpq.top().x;
        pixelmat[pixelmatid][pixelcols].y=markerpq.top().y;
        markerMap(pixelmat[pixelmatid][pixelcols].x,pixelmat[pixelmatid][pixelcols].y)=pixelmatid;
        tempval=pixelmat[pixelmatid][pixelcols].val;
        pixelcols++;
        markerpq.pop();

    }
  }
}

cout<<"The pixelmat id is"<<pixelmatid<<endl;



pixelmatid=pixelmatid+1;



         id_num=id;
	 /////////////////////////////////////
       // Array2D<int> idArr(id,id,0);

Array2D<int> idArr(pixelmatid,pixelmatid,0);

//	 Array2D<double> neighDis(id,id,0);
//         int pixelSum[pixelmatid]={0};
//	int pixelNum[pixelmatid]={0};
       //  memset(pixelNum,-10,sizeof(pixelNum));

         



         cv::Vec3b colors[pixelmatid];
         #pragma omp parallel for
         for(int i = 0; i <= pixelmatid; i++) {
           Vec3b vecColor;
           vecColor[0] = rand()%255+0;
           vecColor[1] = rand()%255+1;
           vecColor[2] = rand()%255+2;          
           colors[i]=vecColor;
        }


int ptcounter=0;
Vec3b boundColor;
boundColor[0] = 0;
boundColor[1] = 0;
boundColor[2] = 0;



/*priority_queue<Pixel,vector<Pixel>,cmp1>temppq;


for(int i=0;i<tempcounter;i++){
  temppq.push({temp(i,0),temp(i,1),temp(i,2)});
 
}







for(int j=0;j<tempcounter;j++){

temp(j,0)=temppq.top().val;
temp(j,1)=temppq.top().x;
temp(j,2)=temppq.top().y;
temppq.pop();
}
*/






//#pragma omp parallel for reduction(+:pixelSum[:id],pixelNum[:id])

//#pragma omp parallel for
for(int minnum=1;minnum<pixelmatid;minnum++){
 //  Pixel origin = Pixel( temp(i,0),temp(i,1),temp(i,2));
  //int originx=;
  //int originy=;
  //int originval=;
  int tempminnum=minnum;
  priority_queue<Pixel,vector<Pixel>,cmp1>prioq;

   for(int s=0;s<markercounter;s++){
     
    if(pixelmat[tempminnum][s].val>-1){
      prioq.push({pixelmat[tempminnum][s].val,pixelmat[tempminnum][s].x,pixelmat[tempminnum][s].y});
    }
     else break;
  }
  while(!prioq.empty()){
    int CrtX=prioq.top().x;
    int CrtY=prioq.top().y;
	  
	  
    // int curpoint[3]={prioq.top()[0],prioq.top()[1],prioq.top()[2]};
    //int curpointx=temp(i,1);
    //int curpointx=temp(i,2);
    prioq.pop();
    //int CrtX=curpoint[1];
    //int CrtY=curpoint[2];
    bool canLabel = true;
    int neighboursLabel = tempminnum;
  //  #pragma omp critical
{
    for(int i = 0; i < 8; i++) {
       int nextX = CrtX + dx[i];
       int nextY = CrtY + dy[i];
       if(nextX < 0 || nextY < 0 || nextX >= rows || nextY >= cols) {
         continue;
       }



        if(markerMap(nextX,nextY) != 0 && image(nextX,nextY)!=0){
                   
                 if(markerMap(CrtX,CrtY) == 0) {

                    // if(neighboursLabel == 0) {
                        //neighboursLabel = markerMap(nextX,nextY);//using id value,all strats from their closest neighbour                  
                          markerMap(CrtX,CrtY)=markerMap(nextX,nextY);
                          neighboursLabel=markerMap(nextX,nextY);
                       
			//idArr(neighboursLabel,neighboursLabel)=neighboursLabel;
                    } else {//this part tells us that if there is two points at the boundary to see if they are in the same classification,
                    //two classification there is no merge
                        if(markerMap(nextX,nextY) != markerMap(CrtX,CrtY) ) {
                               canLabel = false;
			    duplImage.at<Vec3b>(CrtX,CrtY)=boundColor;
                            //   idArr(neighboursLabel-1,markerMap(nextX,nextY)-1)=1;
			    //idSum++;


                   //         Vec3b vecColor;
                     //       vecColor[0] = rand()%255+0;
                       //     vecColor[1] = rand()%255+1;
                         //   vecColor[2] = rand()%255+2;

                        }
                    }
                } 
	 else {
                if(inprioq(nextX,nextY) == false) {
                        inprioq(nextX,nextY) = true;//aviod duplicate point is chosen,point does not exist in marker or background
                        //Pixel next=Pixel(image(nextX,nextY),nextX,nextY);
		       // if(image(nextX,nextY)>60&&abs(image(nextX,nextY)-image(CrtX,CrtY)<3)){	
			prioq.push({image(nextX,nextY),nextX,nextY});

                        //the most important expending step,//only the point whose pixel value is 0 in Markermap
                       // }
		//	else continue;
		  }
                }
                

	    }//inner for


             if(canLabel&&image(CrtX,CrtY)!=0) {
                 markerMap(CrtX,CrtY) = neighboursLabel;//in this way it tells us that the points in same region share the same id or label
                // idArr[neighboursLabel-1][neighboursLabel-1][0][2]=idArr[neighboursLabel-1][neighboursLabel-1][0][2]+image(CrtX,CrtY);//sum of pixel value
		// idArr[neighboursLabel-1][neighboursLabel-1][0][1]=idArr[neighboursLabel-1][neighboursLabel-1][0][1]+1;//num
		 
		//pixelSum[neighboursLabel-1]=pixelSum[neighboursLabel-1]+image(CrtX,CrtY);
		//pixelNum[neighboursLabel-1]=pixelNum[neighboursLabel-1]+1;
                 duplImage.at<Vec3b>(CrtX,CrtY)=colors[ markerMap(CrtX,CrtY) ];
            }
        }   
    }//while
}//for




       cv::addWeighted(duplImage,0.25,testDuplicate,0.75,0,dstImage);

    
        duplImage.release();
        testDuplicate.release();
        return dstImage;
    }

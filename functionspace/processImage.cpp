#include "../headerspace/WatershedAlg.h"
#include<opencv2/ximgproc.hpp>
using namespace cv;
using namespace ximgproc;
void WatershedAlg::processImage(Mat &image,Mat &duplImage,Array2D<int> &threshmat0,Array2D<int> & markers,Array2D<bool> &visArr,Array1D &plotx,Array1D &ploty,Array2D<int>& plots,Array2D<bool> &inprioq,Array2D<int> &markerMap,Array2D<int> &temp,Array2D<int>& nextSet,int**** arr4D,int**** mat4D,Bool2D &visBool,int **platmarker,int **distance) {
       //Mat testImg;
//       image.copyTo(testImg);
//
//
//
//
//
//
    image = makeImageGrayScale(image);
    Array2D<int>testMat(image.rows,image.cols,0);	
	
    int morph_size = 2;
    Mat dialelement = getStructuringElement(cv::MORPH_RECT, Size(2 * morph_size + 1,2 * morph_size + 1),
    Point(morph_size, morph_size));

    Mat dill(image.rows,image.cols,CV_8UC1,Scalar::all(0));//

    dilate(image, dill, dialelement,Point(-1, -1), 1);//

   // imshow("result dilation",dill);
    //waitKey(0);

      
    Mat eroelement = getStructuringElement(cv::MORPH_RECT, Size(2 * morph_size + 1,2 * morph_size + 1),
    Point(morph_size, morph_size));
    Mat eroimg(image.rows,image.cols,CV_8UC1,Scalar::all(0));////
    cv::erode(dill,eroimg,eroelement);////
   // cv::imshow("erode image",eroimg);
   // cv::waitKey(0);


    //cv::Canny(eroimg,cannyImg,60,120);
   // cv::imshow("Canny result",cannyImg);
   // cv::waitKey(0);
	
   // image = makeImageGrayScale(image);

    Mat tarImg;

    dill.copyTo(tarImg);

    cv::Canny(dill,tarImg,10,24);
//    cv::imshow("Canny edge",tarImg);
  //  cv::waitKey(0);
//    #pragma omp parallel for
    for(int i=0;i<image.rows;i++){
     for(int j=0;j<image.cols;j++){
        threshmat0(i,j)=(int)tarImg.at<uchar>(i,j);
     }
    }
     
     //  image = makeImageGrayScale(image);


      // vector< vector<int> > erosionTargetThree{{1, 1, 1}, {1, 1, 1}, {1, 1, 1}, };
      // image = erosion(image, erosionTargetThree);

       // int** __restrict threshmat;
       
    //    threshmat0= thresholdMeasure(image,threshmat0);

       //int** __restrict antimat;
      threshmat0=antiInverseImage(threshmat0,image.rows,image.cols);


	//int** __restrict matimage;
      threshmat0=distanceTransform(threshmat0,markers,image.rows,image.cols,plots,visArr,plotx,ploty,arr4D,mat4D,visBool,platmarker,distance);
    
  


    

      Mat distImg(image.rows,image.cols,CV_8UC1,Scalar::all(0));
      Mat tdistImg(image.rows,image.cols,CV_8UC1,Scalar::all(0));
      // Mat threshImg(image.rows,image.cols,CV_8UC1,Scalar::all(0));
  //     #pragma omp parallel for
       for(int i=0;i<image.rows;i++){
         for(int j=0;j<image.cols;j++){

	    // if(threshmat0(i,j)>10){	   
	//	 threshmat0(i,j)=255;
	         distImg.at<uchar>(i,j)=threshmat0(i,j);
	  //  }
	    // else
	    // {
	//	     threshmat0(i,j)=0;
	//	     distImg.at<uchar>(i,j)=threshmat0(i,j);

	    // }
	   }
       } 
    //  cv::imshow("distance",distImg);
     // cv::waitKey(0);
//      cv::erode(distImg,distImg,eroelement);
 //     cv::imshow("distance 2",distImg);
   //   cv::waitKey(0);
     // dilate(distImg, distImg, dialelement,Point(-1, -1), 1);
      //threshold(distImg, distImg, 30, 255, 0);
      //cv::imshow("threshImg",distImg);
      //cv::waitKey(0);
 

    //   #pragma omp parallel for
       for(int i=0;i<image.rows;i++){
         for(int j=0;j<image.cols;j++){

             if(threshmat0(i,j)>65){     
              // threshmat0(i,j)=255;
               tdistImg.at<uchar>(i,j)=255;
            }
           else
           {
                 //  threshmat0(i,j)=0;
                   tdistImg.at<uchar>(i,j)=0;

             }
           }
       }

   

      
       //dilate(tdistImg, tdistImg, dialelement,Point(-1, -1), 1);
       //cv::imshow("distance to bin tdimage",tdistImg);
       // cv::waitKey(0);
       // cv::erode(tdistImg,tdistImg,eroelement);
       // cv::imshow("distance 2 bin tdimage",tdistImg);

       //image.rows,image.cols,CV_8UC1,Scalar::all(0));
     /*
       vector<vector<Point>>contours;
       findContours(tdistImg,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
       Mat tmarkers(image.rows,image.cols,CV_8UC1,Scalar::all(0));
      
       for(int i=0;i<contours.size();i++){
         drawContours(tmarkers,contours,static_cast<int>(i),Scalar(static_cast<int>(i)+1),-1);

       }

       circle(tmarkers,Point(5,5),3,Scalar(255),-1);
       Mat marker8u(image.rows,image.cols,CV_8UC1,Scalar::all(0));
       Mat marker3(image.rows,image.cols,CV_8UC1,Scalar::all(0));


       tmarkers.convertTo(marker8u,CV_8UC1,10);
       cv::erode(marker8u,marker8u,eroelement);
      // cv::imshow("marker and circle result",marker8u);
      // cv::waitKey(0);
      

       #pragma omp parallel for
       for(int i=0;i<image.rows;i++){
         for(int j=0;j<image.cols;j++){

             if(marker8u.at<uchar>(i,j)>0){
              // threshmat0(i,j)=255;
              marker8u.at<uchar>(i,j)=255;
            }
           else
           {
                 //  threshmat0(i,j)=0;
                   marker8u.at<uchar>(i,j)=0;

             }
           }
       }


      int nlabels=cv::connectedComponents(marker8u,marker3,4);
      cout<<"nlabels"<<nlabels<<endl;

       #pragma omp parallel for
       for(int i=0;i<image.rows;i++){
	       for(int j=0;j<image.cols;j++){
                testMat(i,j)=(int)marker3.at<uchar>(i,j);
	       }
       }





        string filename25="/opt/testmarker.txt";
        ofstream fout25(filename25);
        



        for(int i = 0; i < image.rows; i++) {
            for(int j = 0; j < image.cols; j++) {

             fout25<<testMat(i,j)<<",";
             

        }
         fout25<<endl;
     }
     fout25.close();
*/




       int id_num=0;
       image = watershed(threshmat0, markers,duplImage,image.rows,image.cols,inprioq,markerMap,temp,nextSet,id_num,testMat);
  //     cv::imshow("watershed result",image);
    //   cv::waitKey(0);
       // id_num=40;   
       //int avgPixels=(image.rows*image.cols)/id_num;
//        Mat matlabel;
//	Mat matmask;
       // Mat testlabel;
       //Mat testmask;
//       Ptr<SuperpixelLSC> SlicImg=createSuperpixelLSC(image,150,0.05);
      // Ptr<SuperpixelLSC> supertestImg=createSuperpixelLSC(testImg,80,0.07);

      //supertestImg->iterate(15);
      //supertestImg->getLabels(testlabel);
      //supertestImg->getLabelContourMask(testmask);

//       SlicImg->iterate(20);
//       SlicImg->getLabels(matlabel);
//       SlicImg->getLabelContourMask(matmask);
       //SlicImg=cv::ximgproc::createSuperpixelSLIC(image,SLIC,avgPixels,10);
       //
       //
      // testImg.setTo(Scalar(255,255,255),testmask);
  //     image.setTo(Scalar(255,255,255),matmask);

      // cv::imshow("merge testSLIC",testImg);
      // cv::waitKey(0);

    //   cv::imshow("merge SLIC",image);
    //   cv::waitKey(0);


    }

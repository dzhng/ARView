/**

Copyright 2011 Rafael Muñoz Salinas. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of
      conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list
      of conditions and the following disclaimer in the documentation and/or other materials
      provided with the distribution.

THIS SOFTWARE IS PROVIDED BY Rafael Muñoz Salinas ''AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Rafael Muñoz Salinas OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those of the
authors and should not be interpreted as representing official policies, either expressed
or implied, of Rafael Muñoz Salinas.

*/
#include "arucofidmarkers.h"
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;
namespace aruco {

/************************************
 *
 *
 *
 *
 ************************************/
/**
*/
Mat FiducidalMarkers::createMarkerImage(int id,int size) throw (cv::Exception)
{
    Mat marker(size,size, CV_8UC1);
    marker.setTo(Scalar(0));
    if (0<=id && id<1024) {
        //for each line, create
        int swidth=size/7;
        int ids[4]={0x10,0x17,0x09,0x0e};
        for (int y=0;y<5;y++) {
            int index=(id>>2*(4-y)) & 0x0003;
            int val=ids[index];
            for (int x=0;x<5;x++) {
                Mat roi=marker(Rect((x+1)* swidth,(y+1)* swidth,swidth,swidth));
                if ( ( val>>(4-x) ) & 0x0001 ) roi.setTo(Scalar(255));
                else roi.setTo(Scalar(0));
            }
        }
    }
    else  throw cv::Exception(9004,"id invalid","createMarker",__FILE__,__LINE__);

    return marker;
}

cv::Mat FiducidalMarkers::getMarkerMat(int id) throw (cv::Exception)
{
    Mat marker(5,5, CV_8UC1);
    marker.setTo(Scalar(0));
    if (0<=id && id<1024) {
        //for each line, create
        int ids[4]={0x10,0x17,0x09,0x0e};
        for (int y=0;y<5;y++) {
            int index=(id>>2*(4-y)) & 0x0003;
            int val=ids[index];
            for (int x=0;x<5;x++) {
                if ( ( val>>(4-x) ) & 0x0001 ) marker.at<uchar>(y,x)=1;
                else marker.at<uchar>(y,x)=0;
            }
        }
    }
    else throw cv::Exception (9189,"Invalid marker id","aruco::fiducidal::createMarkerMat",__FILE__,__LINE__);
    return marker;
}
/************************************
 *
 *
 *
 *
 ************************************/

cv::Mat  FiducidalMarkers::createBoardImage( Size gridSize,int MarkerSize,int MarkerDistance,unsigned int FirstMarkerID, BoardConfiguration& TInfo  ) throw (cv::Exception)
{

    vector<vector<int> > MarkersIds;
//     vector<vector<int> > *TheMarkersIds=NULL;

    srand(time(NULL));
    TInfo._markersId.create(gridSize,CV_32SC1);
    int nMarkers=gridSize.height*gridSize.width;
    if (FirstMarkerID<1023) {
        if (FirstMarkerID+nMarkers>=1024)
            throw cv::Exception(9189,"Creation of board impies a marker with an impossible id:","aruco::createBoard",__FILE__,__LINE__);
    }
//     else if(FirstMarkerID>=2000 && FirstMarkerID<2007){
//       if (FirstMarkerID+nMarkers>=2008)
// 	  throw cv::Exception(9189,"Creation of board impies a marker with an impossible id:","aruco::createBoard",__FILE__,__LINE__);
//     }
    else   throw cv::Exception(9189,"Creation of board impies a marker with an impossible id:","aruco::createBoard",__FILE__,__LINE__);
    unsigned int idp=0;
    for (  int i=0;i<gridSize.height;i++)
        for (  int j=0;j<gridSize.width;j++,idp++)
            TInfo._markersId.at<int>(i,j)=FirstMarkerID+idp; //number in the range [0,1023]




    TInfo._markerSizePix=MarkerSize;
    TInfo._markerDistancePix=MarkerDistance;

    int sizeY=gridSize.height*MarkerSize+(gridSize.height-1)*MarkerDistance;
    int sizeX=gridSize.width*MarkerSize+(gridSize.width-1)*MarkerDistance;

    Mat tableImage(sizeY,sizeX,CV_8UC1);
    tableImage.setTo(Scalar(255));
    for (int y=0;y<gridSize.height;y++)
        for (int x=0;x<gridSize.width;x++) {
            Mat subrect(tableImage,Rect( x*(MarkerDistance+MarkerSize),y*(MarkerDistance+MarkerSize),MarkerSize,MarkerSize));
            Mat marker=createMarkerImage( TInfo._markersId.at<int>(y,x),MarkerSize);
            marker.copyTo(subrect);
        }

    return tableImage;
}
/************************************
 *
 *
 *
 *
 ************************************/
Mat FiducidalMarkers::rotate(Mat  in)
{
    Mat out;
    in.copyTo(out);
    for (int i=0;i<in.rows;i++)
    {
        for (int j=0;j<in.cols;j++)
        {
            out.at<uchar>(i,j)=in.at<uchar>(in.cols-j-1,i);
        }
    }
    return out;
}


/************************************
 *
 *
 *
 *
 ************************************/
int FiducidalMarkers::hammDistMarker(Mat  bits)
{
    int ids[4][5]=
    {
        {
            1,0,0,0,0
        }
        ,
        {
            1,0,1,1,1
        }
        ,
        {
            0,1,0,0,1
        }
        ,
        {
            0, 1, 1, 1, 0
        }
    };
    int dist=0;

    for (int y=0;y<5;y++)
    {
        int minSum=1e5;
        //hamming distance to each possible word
        for (int p=0;p<4;p++)
        {
            int sum=0;
            //now, count
            for (int x=0;x<5;x++)
                sum+=  bits.at<uchar>(y,x) == ids[p][x]?0:1;
            if (minSum>sum) minSum=sum;
        }
        //do the and
        dist+=minSum;
    }

    return dist;
}

/************************************
 *
 *
 *
 *
 ************************************/
int FiducidalMarkers::analyzeMarkerImage(Mat &grey,int &nRotations)
{

    //Markers  are divided in 7x7 regions, of which the inner 5x5 belongs to marker info
    //the external border shoould be entirely black

    int swidth=grey.rows/7;
    for (int y=0;y<7;y++)
    {
        int inc=6;
        if (y==0 || y==6) inc=1;//for first and last row, check the whole border
        for (int x=0;x<7;x+=inc)
        {
            int Xstart=(x)*(swidth);
            int Ystart=(y)*(swidth);
            Mat square=grey(Rect(Xstart,Ystart,swidth,swidth));
            int nZ=countNonZero(square);
            if (nZ> (swidth*swidth) /2) {
// 		cout<<"neb"<<endl;
                return -1;//can not be a marker because the border element is not black!
            }
        }
    }

    //now,
    vector<int> markerInfo(5);
    Mat _bits=Mat::zeros(5,5,CV_8UC1);
    //get information(for each inner square, determine if it is  black or white)

    for (int y=0;y<5;y++)
    {

        for (int x=0;x<5;x++)
        {
            int Xstart=(x+1)*(swidth);
            int Ystart=(y+1)*(swidth);
            Mat square=grey(Rect(Xstart,Ystart,swidth,swidth));
            int nZ=countNonZero(square);
            if (nZ> (swidth*swidth) /2)  _bits.at<uchar>( y,x)=1;
        }
    }
// 		printMat<uchar>( _bits,"or mat");

    //checkl all possible rotations
    Mat _bitsFlip;
    Mat Rotations[4];
    Rotations[0]=_bits;
    int dists[4];
    dists[0]=hammDistMarker( Rotations[0]) ;
    pair<int,int> minDist( dists[0],0);
    for (int i=1;i<4;i++)
    {
        //rotate
        Rotations[i]=rotate(Rotations[i-1]);
        //get the hamming distance to the nearest possible word
        dists[i]=hammDistMarker( Rotations[i]) ;
        if (dists[i]<minDist.first)
        {
            minDist.first=  dists[i];
            minDist.second=i;
        }
    }
// 		        printMat<uchar>( Rotations [ minDist.second]);
// 		 	cout<<"MinDist="<<minDist.first<<" "<<minDist.second<<endl;

    nRotations=minDist.second;
    if (minDist.first!=0)	 //FUTURE WORK: correct if any error
        return -1;
    else {//Get id of the marker
        int MatID=0;
        cv::Mat bits=Rotations [ minDist.second];
        for (int y=0;y<5;y++)
        {
            MatID<<=1;
            if ( bits.at<uchar>(y,1)) MatID|=1;
            MatID<<=1;
            if ( bits.at<uchar>(y,3)) MatID|=1;
        }
        return MatID;
    }
}


/************************************
 *
 *
 *
 *
 ************************************/
bool FiducidalMarkers::correctHammMarker(Mat &bits)
{
    //detect this lines with errors
    bool errors[4];
    int ids[4][5]=
    {
        {
            0,0,0,0,0
        }
        ,
        {
            0,0,1,1,1
        }
        ,
        {
            1,1,0,0,1
        }
        ,
        {
            1, 1, 1, 1, 0
        }
    };

    for (int y=0;y<5;y++)
    {
        int minSum=1e5;
        //hamming distance to each possible word
        for (int p=0;p<4;p++)
        {
            int sum=0;
            //now, count
            for (int x=0;x<5;x++)
                sum+=  bits.at<uchar>(y,x) == ids[p][x]?0:1;
            if (minSum>sum) minSum=sum;
        }
        if (minSum!=0) errors[y]=true;
        else errors[y]=false;
    }

    return true;
}

/************************************
 *
 *
 *
 *
 ************************************/
int FiducidalMarkers::detect(const Mat &in,int &nRotations)
{
    assert(in.rows==in.cols);
    Mat grey;
    if ( in.type()==CV_8UC1) grey=in;
    else cv::cvtColor(in,grey,CV_BGR2GRAY);
    //threshold image
    threshold(grey, grey,125, 255, THRESH_BINARY|THRESH_OTSU);

    //now, analyze the interior in order to get the id
    //try first with the big ones

    return analyzeMarkerImage(grey,nRotations);;
    //too many false positives
    /*    int id=analyzeMarkerImage(grey,nRotations);
        if (id!=-1) return id;
        id=analyzeMarkerImage_type2(grey,nRotations);
        if (id!=-1) return id;
        return -1;*/
}

}


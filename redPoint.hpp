//	#include <opencv2/highgui.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
using namespace cv;

struct ss {
    int i; float a;
};

bool comp(const ss &a, const ss &b)
{
    return a.a>b.a;
}

bool compp(const int &a, const int &b){
    return a < b;
}

void swap(int &a, int &b) {
    if (a > b) {
        int tmp = a;
        a = b;
        b = tmp;
    }
}

template<class T> class Image
{
private:
    Mat imgp;
public:
    Image(Mat img=0) {imgp=img;}
    ~Image() {imgp.release();}
    inline T* operator[] (const int rowIndx)
    {
       // return ((T)imgp.imageData + rowIndx*imgp.widthStep);
    	Mat tmp = imgp.rowRange(rowIndx, rowIndx+1);
    	//cout << tmp.cols << endl;
    	T* ret = tmp.data;
    	/*for (int i=0; i<tmp.cols; i++)
    	{
    		ret[i] = tmp.colRange(i,i);
    	}*/
    	return ret; // (T*)imgp.rowRange(rowIndx, rowIndx);
    }
};
typedef struct {
    unsigned char b,g,r;
} RgbPixel;
typedef Image<RgbPixel>      RgbImage;
typedef Image<unsigned char> BwImage;

vector<double> detect_red_point(Mat img)
{
	int valve=30;
    int i,j,up,down,left,right;
    Mat img1;
    vector<double> res;
    double cX = img.cols / double(2);
    double cY = img.rows / double(2);
    double r = 0;
    static double x = 0;
    static double y = 0;

  //  cvNamedWindow("win1");
 //   CvCapture *cam=cvCreateCameraCapture(0);
 //   if (!cam) return 1;
 //   img=cvQueryFrame(cam);
    img1=Mat(img.rows, img.cols,8,3);	//cvCreateImage
    up=0;
    down=img.rows;
    left=0;
    right=img.cols;

   // img=cvQueryFrame(cam);
        //cvCopyImage(img,img1);
    img1 = img.clone();
    BwImage dat(img1);
    //cout << int(dat[down/2][right/2*3+2]) << ' ' << int(dat[down/2][right/2*3+1]) << ' ' << int(dat[down/2][right/2*3]) << endl; 
    for (i=up; i<down; i++)
        for (j=left; j<right; j++)
        {
            int j0=j*3;
            if (dat[i][j0+2]>valve && dat[i][j0+2]-dat[i][j0+1]>valve && dat[i][j0+2]-dat[i][j0]>valve/3)
                { dat[i][j0+2]=255; dat[i][j0+1]=255; dat[i][j0]=255; }
            else { dat[i][j0+2]=0; dat[i][j0+1]=0; dat[i][j0]=0; }
        }
   // imshow("win1",img);
    imshow("win2",img1);

    Mat singleImg[3];
    split(img1,singleImg);
    Mat edge = singleImg[0];

    GaussianBlur(edge, edge, Size(7, 7), 2, 2);
    vector<Vec3f> circles;
    //huofuyuan

    int erosion_size = 3;
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size(2 * erosion_size + 1, 2 * erosion_size + 1), Point(erosion_size, erosion_size));
    dilate(edge,edge,element);
    threshold(edge,edge,10,255,0);
    imshow("win4", edge);

    //HoughCircles(edge, circles, CV_HOUGH_GRADIENT, 1.5, 10, 200, 100, 0, 0);


    Canny(edge,edge,100,200,3);
    vector<Mat> contours(500);
    findContours(edge, contours,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    int len = contours.size();
    vector<ss> sss(len);
    vector<Moments> mu(len);
    for (int i = 0; i < len; i++)
    {
        mu[i] = moments(contours[i], false);
        ss tmp;
        tmp.a = contourArea(contours[i]);
        tmp.i = i;
        sss.push_back(tmp);
    }

    sort(sss.begin(), sss.end(), comp);
    vector<Point2f> mc(len);
    for (int i = 0; i < len; i++)
    {
        mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
    }
    if(len != 0 && sss[0].a > 500){
        vector<Point> frame = contours[sss[0].i];
        int num_points = frame.size();
        int side = frame[0].x;
        for (int n = 1; n < num_points; ++n) {
            side = min(side, frame[n].x);
        }
        
        r = abs(side - mc[sss[0].i].x);
        x = mc[sss[0].i].x-cX;
        y = mc[sss[0].i].y-cY;
        //cout << sss[0].a << endl;
        circle(img, mc[sss[0].i], 3, Scalar(0, 255, 0), -1, 8, 0);
        //circle(img, mc[sss[0].i], r, Scalar(155, 50, 255), 3, 8, 0);
    }


    /*
    const double z_val = 1;
    //static double z = 0;
    for (size_t i = 0; i < circles.size(); i++)
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        //center
        circle(img, center, 3, Scalar(0, 255, 0), -1, 8, 0);
        //contour
        circle(img, center, radius, Scalar(155, 50, 255), 3, 8, 0);

        if (radius > r)
        {
        	r = radius;
        	x = double(center.x-cX);
        	y = double(center.y-cY);
        	//if (fabs(x)+fabs(y) < threshold) z = z_val;
        }
       // cout<<center.x<<' '<<center.y<<' '<<radius<<endl;
    }

    //if (!circles.size()) z=0;*/
    imshow("win3",img);

    res.push_back(x);
    res.push_back(y);
    res.push_back(r);
    res.push_back(len);

    return res;
}
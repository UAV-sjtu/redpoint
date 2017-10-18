#include "redPoint.hpp"

using namespace std;

int main()
{
    Mat img;
    static double dx = 0;
    static double dy = 0;
    static double dz = 0;

    vector<double> res;

    //CvCapture *cam = cvCreateCameraCapture(1);
    //if (!cam) return 1;
    VideoCapture pCapture(1);
 //   img = cvQueryFrame(cam);

    while (1)
    {
    	//img = cvQueryFrame(cam);
    	pCapture >> img;
    	res = detect_red_point(img);

    	for (int i=0; i<res.size(); i++)
    	{
    		cout<< res[i]<<' ';
    	}
    	cout<<endl;

    	if (cvWaitKey(1) >= 0) break;
    }

 	return 0;
}
/*   int valve=60;// ºìÉ«µÄãÐÖµ
    int i,j,up,down,left,right;
    cvNamedWindow("win1");
    CvCapture *cam=cvCreateCameraCapture(0);
    if (!cam) return 1;
    img=cvQueryFrame(cam);
    img1=Mat(img.rows, img.cols,8,3);	//cvCreateImage
    up=0;
    down=img.rows;
    left=0;
    right=img.cols; 
    //cout << right << endl;
    while (1)
    {
        img=cvQueryFrame(cam);
        //cvCopyImage(img,img1);
        img1 = img.clone();
        BwImage dat(img1);
        for (i=up; i<down; i++)
            for (j=left; j<right; j++)
            {
                int j0=j*3;
                if (dat[i][j0+2]-dat[i][j0]>valve && dat[i][j0+2]-dat[i][j0+1]>valve)
                   { dat[i][j0+2]=255; dat[i][j0+1]=255; dat[i][j0]=255; }
                else { dat[i][j0+2]=0; dat[i][j0+1]=0; dat[i][j0]=0; }
            }
        imshow("win1",img);
        imshow("win2",img1);

        Mat singleImg[3];
        split(img1,singleImg);
        Mat edge = singleImg[0];

        GaussianBlur(edge, edge, Size(7, 7), 2, 2);
        vector<Vec3f> circles;
        //huofuyuan
        HoughCircles(edge, circles, CV_HOUGH_GRADIENT, 1.5, 10, 200, 100, 0, 0);
        for (size_t i = 0; i < circles.size(); i++)
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            //center
            circle(img, center, 3, Scalar(0, 255, 0), -1, 8, 0);
            //contour
            circle(img, center, radius, Scalar(155, 50, 255), 3, 8, 0);
            cout<<center.x<<' '<<center.y<<' '<<radius<<endl;
        }
        imshow("win3",img);

        x = double(center.x)/radius;
        y = double(center.y)/radius;
        if (x+y < 100000000000) z = 10000000;

        if (cvWaitKey(1)>=0) break; 
    } 
    return 0;
}
*/
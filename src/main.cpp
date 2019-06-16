#include <iostream>
#include <vector>
#include <string>
#include <fstream>
//#include <sstream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <sys/types.h>
#include <dirent.h>

bool loadImageFiles(std::string dirName, std::string ext, std::vector<std::string> &imageFileName)
{
    DIR* dp=opendir(dirName.c_str());

    std::cout << "aa" << std::endl;

    if (dp!=NULL) {

        struct dirent* dent;
        
        do {
            dent = readdir(dp);
            if (dent!=NULL) {
                std::cout << dent->d_name << std::endl;   
            }
        } while(dent!=NULL);
        
        closedir(dp);
    }

    return true;
}

int main( int argc, char *argv[] )
{
	using namespace cv;
    using namespace std;

    // 画像データのディレクトリ指定
    string dirName = "~/Pictures/timelapse_20190616/part1/";

    // 画像データのロード
    vector<string> imageFileName;
    if (!loadImageFiles(dirName, ".JPG", imageFileName)) return 0;

    // オリジナルの画像サイズ

    // 縮小する（明るい点を残すように）

    // 重ね合わせる

    // 表示してみる

    cv::Mat m = cv::imread("../data/RefMain00.pgm", 1);
    cv::namedWindow("sample", cv::WINDOW_AUTOSIZE);
    cv::imshow("sample", m);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;

}
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
#include <sys/stat.h>

#include "../include/ImageCV2.hpp"

using namespace cv;
using namespace std;


bool loadImageFiles(std::string dirName, std::string ext, std::vector<std::string> &imageFileName)
{
    int i, dirElements;
    string search_path;

    struct stat stat_buf;
    struct dirent **namelist=NULL;

    cout << dirName << endl;

    // dirElements にはディレクトリ内の要素数が入る
    dirElements = scandir(dirName.c_str(), &namelist, NULL, NULL);

    if(dirElements == -1) {
        cout << "ERROR" <<  endl;
    }
    else {
        //ディレクトリかファイルかを順番に識別
        for (i=0; i<dirElements; i+=1) {

            // "." と ".." を除く
            if( (strcmp(namelist[i]->d_name , ".\0") != 0) && (strcmp(namelist[i]->d_name , "..\0") != 0) ) {

                //search_pathには検索対象のフルパスを格納する
                search_path = dirName + string(namelist[i] -> d_name);

                // ファイル情報の取得の成功
                if (stat(search_path.c_str(), &stat_buf) == 0) {

                    // ディレクトリだった場合の処理
                    if ((stat_buf.st_mode & S_IFMT) == S_IFDIR){
                        // 再帰によりディレクトリ内を探索
                        loadImageFiles(dirName + string(namelist[i] -> d_name) + "/", ext, imageFileName);
                    }
                    else {
                        //ファイルだった場合の処理
                        cout << search_path << endl;
                        imageFileName.push_back(search_path);
                    }
                }
                else {
                    // ファイル情報の取得の失敗
                    cout << "ERROR" <<  endl << endl;
                }
            }
        }
    }

    free(namelist);
    
    return true;
}

bool getImageFileList(string path, vector<string> &imageFileName, string extension)
{
	string fname;

	// readdir() で返されるエントリーポイント
   dirent* entry;

    cout << "path = " << path << endl;

	// ディレクトリへのポインタ
	DIR *dp;
	dp = opendir(path.c_str());

    int eno = errno;
    cout << "errno = " << eno << endl;


	if (dp==NULL) return false;

    cout << "kita?" << endl;

	do {
		// エントリーポイント
		entry = readdir(dp);

		if (entry != NULL) {
			fname = entry->d_name;
			//cout << fname << endl;

			// ファイルの場合
			//int pos = fname.find("refImg.pgm");
			//int pos1 = fname.find(prefix);
			int pos2 = fname.find(extension);
			if (pos2 < 0) continue;
			//if (pos1 < 0 || pos2 < 0) continue;

            cout << path+fname << endl;
			imageFileName.push_back(path+"/"+fname);
		}

	} while (entry != NULL);

	closedir(dp);

	return true;
}

bool resizeImage( CImageCV2 &src, int ratio, CImageCV2 &dst)
{
    int imgW = src.GetImgW();
    int imgH = src.GetImgH();

    int w2 = imgW / ratio;
    int h2 = imgH / ratio;

    // グレーに変換
    CImageCV2 gray;
    src.ConvertRgb2Gray(&gray);

    if (!dst.Create24(w2, h2)) return false;

    int x, y, lx, ly;
    int max, maxX, maxY;
    int R, G, B;

    for (y=0; y<imgH; y+=ratio) {
        for (x=0; x<imgW; x+=ratio) {
     
            max = 0;
            maxX = maxY = 0;        

            for (ly=0; ly <ratio; ly++) {
                for (lx=0; lx < ratio; lx++) {
                    if (max < gray.GetImg(x+lx, y+ly)) {
                        max = gray.GetImg(x+lx, y+ly);
                        maxX = x+lx;
                        maxY = y+ly;
                    }
                }
            }

            // 最大値で縮小
            src.GetImg24(maxX, maxY, &R, &G, &B);
            dst.SetImg(maxX/ratio, maxY/ratio, R, G, B);
        }
    }

    return true;
}

bool compositeImage(CImageCV2 &src1, CImageCV2 &src2)
{
    CImageCV2 gray1, gray2;
    src1.ConvertRgb2Gray(&gray1);
    src2.ConvertRgb2Gray(&gray2);

    int imgW = src1.GetImgW();
    int imgH = src1.GetImgH();

    int x,y;
    int R,G,B;

    for (y=0; y<imgH; y++) {
        for (x=0; x<imgW; x++) {

            if (gray1.GetImg(x,y) < gray2.GetImg(x,y)) {
                src2.GetImg24(x, y, &R, &G, &B);
                src1.SetImg(  x, y,  R,  G,  B);
            }
        }
    }

    return true;
}

int main( int argc, char *argv[] )
{
    // 画像データのディレクトリ指定
    string path = "/home/kazuo/Pictures/timelapse_20190616/part2";
    //string path = "/home/kazuo/work/devCpp/testOpenCV/data/";

    // 画像データのロード
    vector<string> imageFileName;
    if (!getImageFileList(path, imageFileName, ".JPG")) return 0;

    // ソート
    sort(imageFileName.begin(), imageFileName.end());

    cout << "ss = " << imageFileName[0] << endl;

    // オリジナルの画像サイズ
    CImageCV2 src1, src2;
    if (!src1.Load24(imageFileName[0])) return false;
    int imgW = src1.GetImgW();
    int imgH = src1.GetImgH();

    cout << "ss" << endl;

    int resizeRatio = 8;

    CImageCV2 base, rs2;
    resizeImage(src1, resizeRatio, base);

    //base.Show("test");

    for (int i=1; i< imageFileName.size(); i++) {
        //cout << i << ", " << imageFileName[i] << endl;

        cout << i-1 << ": done." << endl;

        // ロード
        if (!src2.Load24(imageFileName[i])) continue;

        // 縮小する（明るい点を残すように）
        resizeImage(src2, resizeRatio, rs2);

        // 重ね合わせる
        compositeImage(base, rs2);
    }

    // これは、ヤリ過ぎだった
    //dilate(base.m_Img, base.m_Img, Mat(), Point(-1, -1), 1);

    // 表示してみる
    base.Show("result");
    base.Save(path+"/result.jpg");

    return 0;

    /* cv::Mat m = cv::imread("../data/RefMain00.pgm", 1);
    cv::namedWindow("sample", cv::WINDOW_AUTOSIZE);
    cv::imshow("sample", m);
    cv::waitKey(0);
    cv::destroyAllWindows();*/

    return 0;

}
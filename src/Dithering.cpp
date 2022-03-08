#include "Dithering.h"


using namespace std;

// @brief   找到新调色板中离旧颜色最近的颜色
// @param   oldpixel    
//          bits        位数
cv::Vec3b Dithering::FindClosestColor(const cv::Vec3b & oldpixel, int bits){
    cv::Vec3b newpixel;
    switch(bits){
        case 16:
        // BGR: 5 6 5
            newpixel[0] = oldpixel[0] & (0xFF<<(8-5));
            newpixel[1] = oldpixel[1] & (0xFF<<(8-6));
            newpixel[2] = oldpixel[2] & (0xFF<<(8-5));        
        case 8:
        // BGR: 2 3 3
            newpixel[0] = oldpixel[0] & (0xFF<<(8-2));
            newpixel[1] = oldpixel[1] & (0xFF<<(8-3));
            newpixel[2] = oldpixel[2] & (0xFF<<(8-3));
        break;
        case 4:
        // BGR: 1 2 1
            newpixel[0] = oldpixel[0] & (0xFF<<(8-1));
            // newpixel[0] = (oldpixel[0] < 127) ? 0 : 255;
            newpixel[1] = oldpixel[1] & (0xFF<<(8-2));
            newpixel[2] = oldpixel[2] & (0xFF<<(8-1));
            // newpixel[2] = (oldpixel[2] < 127) ? 0 : 255;
        break;
    }
    return newpixel;
}

// @brief  dithering的具体实现
// @param  img_input   输入cv::Mat图像(24位)
//         bits
// @ret:   dithering图像
/* pseudocode

for each y from top to bottom do
    for each x from left to right do
        oldpixel := pixels[x][y]
        newpixel := find_closest_palette_color(oldpixel)
        pixels[x][y] := newpixel
        quant_error := oldpixel - newpixel
        pixels[x + 1][y    ] := pixels[x + 1][y    ] + quant_error × 7 / 16
        pixels[x - 1][y + 1] := pixels[x - 1][y + 1] + quant_error × 3 / 16
        pixels[x    ][y + 1] := pixels[x    ][y + 1] + quant_error × 5 / 16
        pixels[x + 1][y + 1] := pixels[x + 1][y + 1] + quant_error × 1 / 16

*/
cv::Mat Dithering::FloydSteinbergDithering(const cv::Mat & img_input, int bits){
    cv::Mat img_output = img_input.clone();

    const int width = img_output.cols;
    const int height = img_output.rows;

    // cout << width << " " << height << endl;
    cv::Vec3b oldpixel;
    cv::Vec3b newpixel;
    cv::Vec3b err;
    for (int j = 0; j < height; j++){
        for (int i = 0; i < width; i++){

            oldpixel = img_output.at<cv::Vec3b>(j,i);
            newpixel = FindClosestColor(oldpixel, bits);
            img_output.at<cv::Vec3b>(j, i) = newpixel;
            // if(j==i){
            //     cout << oldpixel << "-----" << newpixel << endl;
            // }
            err = oldpixel - newpixel;
            // cout << err;
            // for (int c = 0; c < 3; c++){
            //     img_output.at<cv::Vec3b>(j, i + 1)[c] += (7/16) * err[c];
            //     img_output.at<cv::Vec3b>(j + 1, i - 1)[c] += (3/16) * err[c];
            //     img_output.at<cv::Vec3b>(j + 1, i)[c] += (5/16) * err[c];
            //     img_output.at<cv::Vec3b>(j + 1, i + 1)[c] += (1/16) * err[c];
            // }
            //  改成float的算法
            if(i + 1 < width)
                img_output.at<cv::Vec3b>(j, i + 1) += (7.0/16.0) * err;
                // img_output.at<cv::Vec3b>(j, i + 1) = PixelAdd((7.0/16.0) * err, img_output.at<cv::Vec3b>(j, i + 1));
            if((j + 1 < height) && (i > 0))
                img_output.at<cv::Vec3b>(j + 1, i - 1) += (3.0/16.0) * err;
            if(j + 1 < height)
                img_output.at<cv::Vec3b>(j + 1, i) += (5.0/16.0) * err;
            if((j + 1 < height) && (i + 1 < width))
                img_output.at<cv::Vec3b>(j + 1, i + 1) += (1.0/16.0) * err;
        }
    }

    return img_output;
}


cv::Mat Dithering::ColorReduction(const cv::Mat & img_input, int bits){
    cv::Mat img_output = img_input.clone();

    const int width = img_output.cols;
    const int height = img_output.rows;

    cv::Vec3b oldpixel;
    cv::Vec3b newpixel;
    for (int j = 0; j < height; j++){
        for (int i = 0; i < width; i++){
            oldpixel = img_output.at<cv::Vec3b>(j,i);
            newpixel = FindClosestColor(oldpixel, bits);
            img_output.at<cv::Vec3b>(j, i) = newpixel;
        }
    }

    return img_output;
}


cv::Mat Dithering::MinimizedAverageError(const cv::Mat & input_img, int bits){

}
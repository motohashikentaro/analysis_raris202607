#include "./../include/bm_hit_matching.h"

#include <iostream>


int main()
{
    HitMatch hm;


    // 入力ROOTファイル読み込み
    hm.BranchExtract();


    // 出力ROOTファイル作成
    hm.CreateMatchFile();


    // ヒット同期・マッチング
    hm.HitMatching();


    // 保存
    hm.SaveMatchFile();


    std::cout << "Hit matching finished." << std::endl;


    return 0;
}
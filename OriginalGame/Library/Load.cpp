#include "Load.h"

EvoLib::Load::DivGraphInfo EvoLib::Load::LoadDivGraph_EvoLib(const char* filePath, const int& div_x, const int& div_y)
{

    int wide = 0;       // グラフィックの横幅
    int height = 0;     // グラフィックの縦幅
    int graphic = -1;   // グラフィックの代入


    // プレイヤーのグラフィックサイズを取得
    {
        graphic = LoadGraph(filePath);
        GetGraphSize(graphic, &wide, &height);
    }


    // 分割数合計
    const int divNum = div_x * div_y;

    // グラフィック情報
    DivGraphInfo graphInfo;

    // スケールを代入
    graphInfo.scale.x = static_cast<float>(wide);
    graphInfo.scale.y = static_cast<float>(height);



    // 分割した画像をハンドルに入れる
    {
        // ハンドル
        int* handle = new int[divNum];

        // 分割した画像を入れる
        LoadDivGraph(filePath, divNum,
            div_x, div_y,
            wide / div_x, height / div_y, handle);

        for (int i = 0; i < divNum; i++)
        {
            // グラフィックを代入
            graphInfo.handle.push_back(handle[i]);
        }

        // メモリの開放
        delete[] handle;
    }

    return graphInfo;
}

std::vector<int> EvoLib::Load::LoadDivGraph_EvoLib_Revision(const char* filePath, const DivNum& div)
{
    int wide = 0;       // グラフィックの横幅
    int height = 0;     // グラフィックの縦幅
    int graphic = -1;   // グラフィックの代入


    // プレイヤーのグラフィックサイズを取得
    {
        graphic = LoadGraph(filePath);
        GetGraphSize(graphic, &wide, &height);
    }


    // 分割数合計
    const int divNum = div.x * div.y;

    // 分割されたグラフィックハンドル
    std::vector<int> graphHandle;

    // 分割した画像をハンドルに入れる
    {
        // ハンドル
        int* handle = new int[divNum];

        // 分割した画像を入れる
        LoadDivGraph(filePath, divNum,
            div.x, div.y,
            wide / div.x, height / div.y, handle);

        for (int i = 0; i < divNum; i++)
        {
            // グラフィックを代入
            graphHandle.push_back(handle[i]);
        }

        // メモリの開放
        delete[] handle;
    }

    // グラフィック情報
    return graphHandle;
}

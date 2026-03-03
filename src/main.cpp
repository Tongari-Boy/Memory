#include <SDL.h>
#include <iostream>
#include <cstdlib>	//rand()用
#include <ctime>	//time()用


//=============
//Enemy & Pool
//=============

struct Enemy {
    float x, y;    //位置
    float vx, vy;  //速度
    int   hp;
    bool  active;
};

class EnemyPool {
public:
    static const int MAX = 10;
    Enemy pool[MAX];

    int freeList[MAX]; //空きスロットの番号を管理
    int freeCount;     //現在の空き数

    int activeList[MAX];  //アクティブなインデックスを管理
    int activeCount_;     //アクティブ数

    //コンストラクタで全てfalseに初期化
    EnemyPool() {
        freeCount = MAX;
        activeCount_ = 0;

        for (int i = 0; i < MAX; i++) {
            pool[i].active = false;
            freeList[i] = i;    //最初は全部空
        }
    }

    //敵のスポーン
    Enemy* spawn(float x, float y, float vx, float vy, int hp) {
        std::cout << "spawn呼び出し freeCount=" << freeCount << std::endl;

        if (freeCount == 0) {
            std::cout << "[警告] スロットが満杯です！" << std::endl;
            return nullptr;
        }

        //末尾から番号を取り出す(探索しない)
        int index = freeList[--freeCount];
        pool[index] = { x, y, vx, vy, hp, true };

        //active版
        activeList[activeCount_++] = index;

        return &pool[index];
    }

    //敵のデスポーン
    void despawn(Enemy* e) {
        if (!e || !e->active) return;
        e->active = false;

        //番号をfreeListに返却
        int index = e - pool;   //ポインタ差分でインデックスを計算
        freeList[freeCount++] = index;

        //activeListから除去
        for (int i = 0; i < activeCount_; i++) {
            if (activeList[i] == index) {
                //末尾と入れ替えて削除（順番を保つ必要はない）
                activeList[i] = activeList[--activeCount_];
                break;
            }
        }
    }

    //全アクティブ敵を更新
    void update() {
        //activeListだけを見る
        for (int i = 0; i < activeCount_; i++) {
            Enemy& e = pool[activeList[i]];
            e.x += e.vx;
            e.y += e.vy;

            //画面外にでたらdespawn
            if (e.x < -20 || e.x > 820 ||
                e.y < -20 || e.y > 620) {
                despawn(&e);//despawn経由でfreeListに返却
                //despawnで末尾と入れ替えるので、iを進めずにもう一度同じiを確認する
            }else{
                i++;    //despawnでactiveListが縮むので調整
            }
        }
    }

    //全アクティブ敵を描画
    void render(SDL_Renderer* r) {
        SDL_SetRenderDrawColor(r, 255, 80, 80, 255);    //赤色を指定
        // activeListだけを見る
        for (int i = 0; i < activeCount_; i++) {
            Enemy& e = pool[activeList[i]];
            SDL_Rect rect = { (int)e.x, (int)e.y, 20, 20 };
            SDL_RenderFillRect(r, &rect);
        }
    }

    //デバッグ：使用中スロット数
    int activeCount() {
        return activeCount_;
    }
};

//=====================
//MAINメインmainめいん
//=====================

int main(int argc, char* argv[]) {
    srand((unsigned)time(nullptr));

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow(
        "EnemyPool Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, SDL_WINDOW_SHOWN
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED
    );

    EnemyPool pool;
    int  spawnTimer = 0;
    bool running = true;
    SDL_Event event;

    while (running) {
        //イベント
        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT) running = false;

        //一定間隔で敵をスポーン
        spawnTimer++;
        if (spawnTimer >= 30) { //約0.5秒ごと
            spawnTimer = 0;
            float vx = ((rand() % 5) + 1) * (rand() % 2 == 0 ? 1 : -1);
            float vy = ((rand() % 5) + 1) * (rand() % 2 == 0 ? 1 : -1);
            pool.spawn(400, 300, vx, vy, 100);
        }

        //更新
        pool.update();

        //描画
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);
        pool.render(renderer);
        SDL_RenderPresent(renderer);

        //スロット状況をコンソールに出力
        std::cout << "active: " << pool.activeCount()
            << " / " << EnemyPool::MAX << std::endl;

        SDL_Delay(16); //約60fps
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
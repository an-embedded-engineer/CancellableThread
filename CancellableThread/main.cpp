#include "TestFunctions.h"

int main()
{
    /* スレッドテスト1 : スレッド開始/停止切り替え */
    //ThreadTest1();

    /* スレッドテスト2 : スレッド遅延実行 */
    //ThreadTest2();

    /* スレッドテスト3 : スレッド遅延実行(相対時間タイムアウトあり) */
    //ThreadTest3();

    /* スレッドテスト4 : スレッド遅延実行(絶対時間タイムアウトあり) */
    //ThreadTest4();

    /* スレッドテスト5 : スレッドマネージャー  */
    ThreadTest5();

    return 0;
}

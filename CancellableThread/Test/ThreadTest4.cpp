#include "CancellableThread.h"
#include "StringFormat.h"
#include "Logger.h"

/* テストスレッド実行処理 */
static void ThreadFunction(CancellableThread& thread, CancellationPoint& cp);

/* スレッドテスト4 : スレッド遅延実行(絶対時間タイムアウトあり) */
void ThreadTest4()
{
    try
    {
        Logger::Info("[Main] Start");

        /* テストスレッドを生成 */
        CancellableThread test_thread(1, "Test Thread", ThreadFunction);

        Logger::Info("[Main] Create Thread [%s]", test_thread.GetName());

        try
        {
            /* テストスレッドを開始 */
            test_thread.Start();

            Logger::Info("[Main] Start Thread [%s]", test_thread.GetName());

            /* カウンタ初期化 */
            size_t i = 0;

            while (true)
            {
                /* カウンタ更新 */
                i++;

                Logger::Info("[Main] Execute Value=%d", i);

                /* 5sec後 */
                if (i == 5)
                {
                    Logger::Info("[Main] Request Ready Thread [%s]", test_thread.GetName());

                    /* テストスレッドのレディを要求 */
                    test_thread.NotifyReady();
                }
                /* 10sec後 */
                else if (i == 10)
                {
                    Logger::Info("[Main] Request Stop Thread [%s]", test_thread.GetName());

                    /* テストスレッドを停止 */
                    test_thread.Stop();
                }
                /* 15sec後 */
                else if (i == 15)
                {
                    Logger::Info("[Main] Start Thread [%s]", test_thread.GetName());

                    /* テストスレッドを開始 */
                    test_thread.Start();
                }
                /* 20sec後 */
                else if (i == 20)
                {
                    Logger::Info("[Main] Request Stop Thread [%s]", test_thread.GetName());

                    /* テストスレッドをレディ要求せずに停止 */
                    test_thread.Stop();
                }
                /* 25sec後 */
                else if (i == 25)
                {
                    Logger::Info("[Main] Start Thread [%s]", test_thread.GetName());

                    /* テストスレッドを開始 */
                    test_thread.Start();
                }
                /* 40sec後 */
                else if (i == 40)
                {
                    /* テストスレッドがタイムアウトするまでレディ要求しない */

                    /* メイン処理終了前にテストスレッド終了を待機 */
                    test_thread.Join();

                    Logger::Info("[Main] Execute End");

                    /* メイン処理終了 */
                    break;
                }

                /* 1sec待機 */
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
        catch (const std::exception& ex)
        {
            Logger::Error("Exception Occurred : %s", ex.what());
        }

        /* テストスレッドが実行中の場合(エラー発生 or スレッド停止せずにメイン処理が終了) */
        if (test_thread.IsRunning() == true)
        {
            Logger::Info("[Main] Request Stop Thread [%s]", test_thread.GetName());

            /* テストスレッドを停止 */
            test_thread.Stop();
        }

        Logger::Info("[Main] End");
    }
    catch (const std::exception& ex)
    {
        Logger::Error("Exception Occurred : %s", ex.what());
    }
}

/* テストスレッド実行処理 */
static void ThreadFunction(CancellableThread& thread, CancellationPoint& cp)
{
    Logger::Info("[%s] Thread Start", thread.GetName());

    /* カウンタ初期化 */
    size_t i = 0;

    try
    {
        Logger::Info("[%s] Wait Thread Ready", thread.GetName(), i);

        /* タイムアウト時間を算出(現在時刻から10秒後) */
        auto time_out_point = std::chrono::steady_clock::now() + std::chrono::seconds(10);

        /* スレッドのReady要求を待機(10秒でタイムアウト) */
        cp.WaitUntil(time_out_point);

        while (true)
        {
            /* カウンタ更新 */
            i++;

            Logger::Info("[%s] Thread Execute Value=%d", thread.GetName(), i);

            /* キャンセルリクエスト確認 */
            cp.CheckCancelRequested();

            /* 1sec待機 */
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    /* スレッドキャンセル発生 */
    catch (const CancelledException& ex)
    {
        Logger::Warn(ex.what());
        Logger::Warn("[%s] Thread Cancelled", thread.GetName());
    }
    /* それ以外の例外発生 */
    catch (const std::exception& ex)
    {
        Logger::Error("Exception Occurred : %s", ex.what());
    }

    Logger::Info("[%s] Thread End", thread.GetName());
}

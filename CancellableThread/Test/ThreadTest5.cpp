#include "ThreadManager.h"
#include "CancellableThread.h"
#include "StringFormat.h"
#include "Logger.h"

/* テストスレッド実行処理 */
static void ThreadFunction1(CancellableThread& thread, CancellationPoint& cp);
static void ThreadFunction2(CancellableThread& thread, CancellationPoint& cp);
static void ThreadFunction3(CancellableThread& thread, CancellationPoint& cp);


/* スレッドテスト5 : スレッドマネージャー */
void ThreadTest5()
{
    try
    {
        Logger::Info("[Main] Start");

        /* スレッドマネージャーシングルトンインスタンス取得 */
        ThreadManager& thread_manager = ThreadManager::GetInstance();

        /* テストスレッドを追加 */
        thread_id_t thread1_id = thread_manager.Add("Test Thread 1", ThreadFunction1);
        Logger::Info("Add Thread [%d:%s]", thread1_id, thread_manager.GetName(thread1_id));

        thread_id_t thread2_id = thread_manager.Add("Test Thread 2", ThreadFunction2);
        Logger::Info("Add Thread [%d:%s]", thread2_id, thread_manager.GetName(thread2_id));

        thread_id_t thread3_id = thread_manager.Add("Test Thread 3", ThreadFunction3);
        Logger::Info("Add Thread [%d:%s]", thread3_id, thread_manager.GetName(thread3_id));

        try
        {
            /* テストスレッドを全て開始 */
            thread_manager.StartAll();

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
                    Logger::Info("[Main] Request Stop Thread [%s]", thread_manager.GetName(thread1_id));

                    /* テストスレッド1を停止 */
                    thread_manager.Stop(thread1_id);

                    Logger::Info("[Main] Request Ready Thread [%s]", thread_manager.GetName(thread2_id));

                    /* テストスレッド2にレディ通知 */
                    thread_manager.NotifyReady(thread2_id);

                    Logger::Info("[Main] Request Ready Thread [%s]", thread_manager.GetName(thread3_id));

                    /* テストスレッド3にレディ通知 */
                    thread_manager.NotifyReady(thread3_id);
                }
                /* 10sec後 */
                else if (i == 10)
                {
                    Logger::Info("[Main] Request Stop Thread [%s]", thread_manager.GetName(thread2_id));

                    /* テストスレッド2を停止 */
                    thread_manager.Stop(thread2_id);
                }
                /* 15sec後 */
                else if (i == 15)
                {
                    Logger::Info("[Main] Request Stop Thread [%s]", thread_manager.GetName(thread3_id));

                    /* テストスレッド3を停止 */
                    thread_manager.Stop(thread3_id);

                    Logger::Info("[Main] Start Thread [%s]", thread_manager.GetName(thread1_id));

                    /* テストスレッド1を開始 */
                    thread_manager.Start(thread1_id);

                    Logger::Info("[Main] Start Thread [%s]", thread_manager.GetName(thread2_id));

                    /* テストスレッド2を開始 */
                    thread_manager.Start(thread2_id);

                    Logger::Info("[Main] Start Thread [%s]", thread_manager.GetName(thread3_id));

                    /* テストスレッド3を開始 */
                    thread_manager.Start(thread3_id);
                }
                /* 20sec後 */
                else if (i == 20)
                {
                    Logger::Info("[Main] Request Ready Thread [%s]", thread_manager.GetName(thread2_id));

                    /* テストスレッド2にレディ通知 */
                    thread_manager.NotifyReady(thread2_id);

                    /* テストスレッド3にはレディ通知しない */
                }
                /* 35sec後 */
                else if (i == 35)
                {
                    /* テストスレッドを停止しない */

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

        Logger::Info("[Main] Request Stop All Thread");

        /* 全てのスレッドを停止(エラー発生 or スレッド停止せずにメイン処理が終了) */
        thread_manager.StopAll();

        Logger::Info("[Main] End");
    }
    catch (const std::exception& ex)
    {
        Logger::Error("Exception Occurred : %s", ex.what());
    }
}

/* テストスレッド1実行処理 */
static void ThreadFunction1(CancellableThread& thread, CancellationPoint& cp)
{
    Logger::Info("[%s] Thread Start", thread.GetName());

    /* カウンタ初期化 */
    size_t i = 0;

    try
    {
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

/* テストスレッド2実行処理 */
static void ThreadFunction2(CancellableThread& thread, CancellationPoint& cp)
{
    Logger::Info("[%s] Thread Start", thread.GetName());

    /* カウンタ初期化 */
    size_t i = 0;

    try
    {
        Logger::Info("[%s] Wait Thread Ready", thread.GetName(), i);

        /* スレッドのReady要求を待機 */
        cp.WaitReady();

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

/* テストスレッド3実行処理 */
static void ThreadFunction3(CancellableThread& thread, CancellationPoint& cp)
{
    Logger::Info("[%s] Thread Start", thread.GetName());

    /* カウンタ初期化 */
    size_t i = 0;

    try
    {
        Logger::Info("[%s] Wait Thread Ready", thread.GetName(), i);

        /* スレッドのReady要求を待機(10秒でタイムアウト) */
        cp.WaitFor(std::chrono::seconds(10));

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

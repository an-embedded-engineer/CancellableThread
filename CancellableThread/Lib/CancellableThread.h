#pragma once
#include <string>
#include "ThreadTypes.h"
#include "CancellationPoint.h"

#include <functional>
#include <thread>

/* Cancellable Threadクラス */
class CancellableThread
{
public:
    /* コンストラクタ */
    CancellableThread();
    /* コンストラクタ */
    CancellableThread(const thread_id_t id, const std::string& name, ThreadFuncType thread_func);

    /* スレッド開始 */
    void Start();
    /* スレッド停止 */
    void Stop();
    /* スレッドレディ通知 */
    void NotifyReady();

    /* スレッド終了まで待機 */
    void Join();
    /* スレッドの管理を放棄 */
    void Detach();

    /* スレッドID取得 */
    const thread_id_t GetID();
    /* スレッド名取得 */
    const std::string& GetName();
    /* スレッド実行中確認 */
    bool IsRunning();

private:
    /* スレッドID */
    thread_id_t m_ID;
    /* スレッド名 */
    std::string m_Name;
    /* Threadオブジェクト */
    std::thread m_Thread;
    /* Cancellation Pointオブジェクト */
    CancellationPoint m_CancellationPoint;
    /* スレッド実行関数 */
    ThreadFuncType m_ThreadFunc;
    /* スレッド実行中フラグ */
    bool m_IsRunning;
};

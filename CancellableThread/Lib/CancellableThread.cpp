#include "CancellableThread.h"
#include "StringFormat.h"
#include "Logger.h"

#include <stdexcept>
#include <iostream>

/* コンストラクタ */
CancellableThread::CancellableThread()
    : m_ID(0)
    , m_Name("")
    , m_Thread()
    , m_CancellationPoint(*this)
    , m_ThreadFunc(nullptr)
    , m_IsRunning(false)
{
    /* Nothing to do */
}

/* コンストラクタ */
CancellableThread::CancellableThread(const thread_id_t id, const std::string& name, ThreadFuncType thread_func)
    : m_ID(id)
    , m_Name(name)
    , m_Thread()
    , m_CancellationPoint(*this)
    , m_ThreadFunc(thread_func)
    , m_IsRunning(false)
{
    /* Nothing to do */
}

/* スレッド開始 */
void CancellableThread::Start()
{
    /* スレッドが実行中でない */
    if (this->m_IsRunning == false)
    {
        /* スレッド実行関数を渡してスレッド実行開始 */
        this->m_Thread = std::thread([this]()
            {
                this->m_ThreadFunc(*this, this->m_CancellationPoint);
            });

        /* スレッド実行中フラグセット */
        this->m_IsRunning = true;
    }
    /* スレッドが既に実行中 */
    else
    {
        Logger::Error("Thread is already running : ID=%d Name=%s", this->m_ID, this->m_Name);
    }
}

/* スレッド停止 */
void CancellableThread::Stop()
{
    /* スレッドが実行中 */
    if (this->m_IsRunning == true)
    {
        /* スレッドキャンセルをリクエスト */
        this->m_CancellationPoint.RequestCancel();

        /* スレッド終了まで待機 */
        this->Join();
    }
    /* スレッドが既に停止 */
    else
    {
        Logger::Warn("Thread is already stopped : ID=%d Name=%s", this->m_ID, this->m_Name);
    }
}

/* スレッドレディ通知 */
void CancellableThread::NotifyReady()
{
    /* スレッドが実行中 */
    if (this->m_IsRunning == true)
    {
        /* スレッドレディをリクエスト */
        this->m_CancellationPoint.RequestReady();
    }
    /* スレッドが停止中 */
    else
    {
        Logger::Warn("Thread is not running : ID=%d Name=%s", this->m_ID, this->m_Name);
    }
}

/* スレッド終了まで待機 */
void CancellableThread::Join()
{
    /* スレッドが実行中 */
    if (this->m_IsRunning == true)
    {
        /* スレッドがThreadオブジェクトに関連付けられているかを確認 */
        if (this->m_Thread.joinable() == true)
        {
            /* スレッド終了まで待機 */
            this->m_Thread.join();

            /* スレッド実行中フラグクリア */
            this->m_IsRunning = false;
        }
    }
    /* スレッドが停止中 */
    else
    {
        Logger::Warn("Thread is already joined : ID=%d Name=%s", this->m_ID, this->m_Name);
    }
}

/* スレッドの管理を放棄 */
void CancellableThread::Detach()
{
    /* スレッドが実行中 */
    if (this->m_IsRunning == true)
    {
        /* スレッドがThreadオブジェクトに関連付けられているかを確認 */
        if (this->m_Thread.joinable() == true)
        {
            /* スレッドの管理を放棄 */
            this->m_Thread.detach();

            /* スレッド実行中フラグクリア */
            this->m_IsRunning = false;
        }
    }
    /* スレッドが停止中 */
    else
    {
        Logger::Warn("Thread is already joined : ID=%d Name=%s", this->m_ID, this->m_Name);
    }
}

/* スレッドID取得 */
const thread_id_t CancellableThread::GetID()
{
    return this->m_ID;
}

/* スレッド名取得 */
const std::string& CancellableThread::GetName()
{
    return this->m_Name;
}

/* スレッド実行中確認 */
bool CancellableThread::IsRunning()
{
    return this->m_IsRunning;
}

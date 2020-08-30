#include "CancellationPoint.h"
#include "CancellableThread.h"

/* コンストラクタ */
CancellationPoint::CancellationPoint(CancellableThread& thread)
    : m_Thread(thread)
    , m_ThreadName("")
    , m_IsCancelled(false)
    , m_IsReady(false)
    , m_Mutex()
    , m_ConditionVariable()
{
    /* スレッド名をセット */
    this->m_ThreadName = this->m_Thread.GetName();
}

/* スレッドキャンセルをリクエスト*/
void CancellationPoint::RequestCancel()
{
    /* Mutexによる排他処理 */
    std::unique_lock<std::mutex> lock(this->m_Mutex);

    Logger::Info("[%s] Cancel Request Accepted", this->m_ThreadName);

    /* キャンセルフラグセット */
    this->m_IsCancelled = true;

    /* 待機スレッドをすべて起床させる */
    this->m_ConditionVariable.notify_all();
}

/* キャンセルリクエスト確認 */
void CancellationPoint::CheckCancelRequested()
{
    /* Mutexによる排他処理 */
    std::unique_lock<std::mutex> lock(this->m_Mutex);

    Logger::Info("[%s] Check Cancel Request", this->m_ThreadName);

    /* キャンセルフラグ確認 */
    if (this->m_IsCancelled == true)
    {
        Logger::Info("[%s] Cancelled in Thread Operation", this->m_ThreadName);

        /* キャンセルを通知 */
        this->NotifyCancel();
    }
}

/* スレッドレディをリクエスト */
void CancellationPoint::RequestReady()
{
    /* Mutexによる排他処理 */
    std::unique_lock<std::mutex> lock(this->m_Mutex);

    Logger::Info("[%s] Read Request Accepted", this->m_ThreadName);

    /* レディフラグをセット */
    this->m_IsReady = true;

    /* 待機スレッドをすべて起床させる */
    this->m_ConditionVariable.notify_all();
}

/* スレッドレディリクエストを待機(キャンセル確認含む) */
void CancellationPoint::WaitReady()
{
    /* Mutexによる排他処理 */
    std::unique_lock<std::mutex> lock(this->m_Mutex);

    /* レディフラグが立つまで待機 */
    this->m_ConditionVariable.wait(lock,
        [this]
        {
            Logger::Info("[%s] Check Cancel Request in Wait Ready", this->m_ThreadName);

            /* キャンセルフラグ確認 */
            if (this->m_IsCancelled == true)
            {
                Logger::Info("[%s] Cancelled in Wait Ready", this->m_ThreadName);

                /* キャンセルを通知 */
                this->NotifyCancel();
            }

            return this->m_IsReady;
        });
}

/* スレッドキャンセル通知 */
void CancellationPoint::NotifyCancel()
{
    /* キャンセルフラグクリア */
    this->m_IsCancelled = false;

    /* レディフラグクリア */
    this->m_IsReady = false;

    Logger::Info("[%s] Throw Cancelld Exception", this->m_ThreadName);

    /* Cancelled Exceptionをthrow */
    throw CancelledException(this->m_Thread);
}

#pragma once
#include <mutex>
#include <condition_variable>

#include "CancelledException.h"
#include "Logger.h"


/* Cancellable Threadクラス前方宣言 */
class CancellableThread;

/* Cancellation Pointクラス宣言 */
class CancellationPoint
{
public:
    /* コンストラクタ */
    CancellationPoint(CancellableThread& thread);

    /* スレッドキャンセルをリクエスト*/
    void RequestCancel();

    /* キャンセルリクエスト確認 */
    void CheckCancelRequested();

    /* スレッドレディをリクエスト */
    void RequestReady();

    /* スレッドレディリクエストを待機(キャンセル確認含む) */
    void WaitReady();

    /* スレッドレディリクエストを相対時間タイムアウト付きで待機(キャンセル確認含む) */
    template <typename PeriodType>
    void WaitFor(const PeriodType& period)
    {
        /* Mutexによる排他処理 */
        std::unique_lock<std::mutex> lock(this->m_Mutex);

        /* 指定時間(呼び出し時間からの相対時刻)になるまで待機 */
        if (false == this->m_ConditionVariable.wait_for(lock, period, [this]
            {
                Logger::Info("[%s] Check Cancel Request in Wait For Ready", this->m_ThreadName);

                /* キャンセルフラグ確認 */
                if (this->m_IsCancelled == true)
                {
                    Logger::Info("[%s] Cancelled in Wait For Ready", this->m_ThreadName);

                    /* キャンセルを通知 */
                    this->NotifyCancel();
                }

                return this->m_IsReady;
            }))
        {
            /* タイムアウトするまでレディリクエストなし */
            Logger::Info("[%s] Cancelled by Timeout in Wait For Ready", this->m_ThreadName);

            /* キャンセルを通知 */
            this->NotifyCancel();
        }
        /* タイムアウトする前にレディリクエストあり */
        else
        {
            return;
        }
    }

    /* スレッドレディリクエストを相対時間タイムアウト付きで待機(キャンセル確認含む) */
    template <typename PeriodType>
    void WaitUntil(const PeriodType& period)
    {
        /* Mutexによる排他処理 */
        std::unique_lock<std::mutex> lock(this->m_Mutex);

        /* 指定時間(呼び出し時に指定した絶対時刻)になるまで待機 */
        if (false == this->m_ConditionVariable.wait_until(lock, period, [this]
            {
                Logger::Info("[%s] Check Cancel Request in Wait Until Ready", this->m_ThreadName);

                /* キャンセルフラグ確認 */
                if (this->m_IsCancelled == true)
                {
                    Logger::Info("[%s] Cancelled in Wait Until Ready", this->m_ThreadName);

                    /* キャンセルを通知 */
                    this->NotifyCancel();
                }

                return this->m_IsReady;
            }))
        {
            /* タイムアウトするまでレディリクエストなし */
            Logger::Info("[%s] Cancelled by Timeout in Wait Until Ready", this->m_ThreadName);

            /* キャンセルを通知 */
            this->NotifyCancel();
        }
            /* タイムアウトする前にレディリクエストあり */
        else
        {
            return;
        }
    }

private:
    /* スレッドキャンセル通知 */
    void NotifyCancel();

private:
    /* Cancellable Threadクラスインスタンス */
    CancellableThread& m_Thread;
    /* スレッド名 */
    std::string m_ThreadName;
    /* キャンセルフラグ */
    bool m_IsCancelled;
    /* レディフラグ */
    bool m_IsReady;
    /* Mutex */
    std::mutex m_Mutex;
    /* Condition Variable */
    std::condition_variable m_ConditionVariable;
};

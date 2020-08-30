#pragma once
#include "CancellableThread.h"

#include <map>

/* Thread Managerクラス宣言 */
class ThreadManager
{
public:
    /* シングルトンインスタンス取得 */
    static ThreadManager& GetInstance();

private:
    /* コンストラクタ */
    ThreadManager();
    /* デストラクタ */
    ~ThreadManager();

public:
    /* コピーコンストラクタ削除 */
    ThreadManager(const ThreadManager&) = delete;
    /* ムーブコンストラクタを削除 */
    ThreadManager(ThreadManager&&) = delete;
    /* コピー代入オペレータ削除 */
    ThreadManager& operator=(const ThreadManager&) = delete;
    /* ムーブ代入オペレータ削除 */
    ThreadManager& operator=(ThreadManager&&) = delete;

public:
    /* スレッド追加 */
    template<typename C, typename T>
    thread_id_t Add(const std::string& name, void(C::*fp)(T), T* obj)
    {
        return this->Add(name, std::bind(fp, obj, std::placeholders::_1, std::placeholders::_2));
    }

    /* スレッド追加 */
    template<typename T>
    thread_id_t Add(const std::string& name, void(*fp)(T))
    {
        return this->Add(name, std::bind(fp, std::placeholders::_1, std::placeholders::_2));
    }

public:
    /* スレッド追加 */
    thread_id_t Add(const std::string& name, ThreadFuncType thread_func);

    /* スレッド存在確認 */
    bool IsExists(const thread_id_t thread_id);

    /* スレッド名取得 */
    const std::string GetName(const thread_id_t thread_id);

    /* スレッド開始 */
    void Start(thread_id_t thread_id);
    /* スレッド停止 */
    void Stop(thread_id_t thread_id);
    /* スレッドレディ通知 */
    void NotifyReady(thread_id_t thread_id);

    /* スレッド終了まで待機 */
    void Join(thread_id_t thread_id);
    /* スレッドの管理を放棄 */
    void Detach(thread_id_t thread_id);

    /* 全スレッド開始 */
    void StartAll();
    /* 全スレッド停止 */
    void StopAll();
    /* 全スレッドレディ通知 */
    void NotifyReadyAll();

    /* 全スレッド終了まで待機 */
    void JoinAll();
    /* 全スレッドの管理を放棄 */
    void DetachAll();

private:
    /* 次のスレッドIDを取得 */
    thread_id_t GetNextID();

private:
    /* スレッド数 */
    thread_id_t m_ThreadCount;

    /* スレッドマップ */
    std::map<thread_id_t, CancellableThread> m_ThreadMap;

    /* スレッド名マップ */
    std::map<thread_id_t, std::string> m_ThreadNameMap;
};


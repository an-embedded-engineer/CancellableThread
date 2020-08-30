#include "ThreadManager.h"
#include "Logger.h"

/* シングルトンインスタンス取得 */
ThreadManager& ThreadManager::GetInstance()
{
    static ThreadManager instance;
    return instance;
}

/* コンストラクタ */
ThreadManager::ThreadManager()
    : m_ThreadCount(0)
    , m_ThreadMap()
    , m_ThreadNameMap()
{
    /* Nothing to do */
}

/* デストラクタ */
ThreadManager::~ThreadManager()
{
    /* Nothing to do */
}

/* スレッド追加 */
thread_id_t ThreadManager::Add(const std::string& name, ThreadFuncType thread_func)
{
    /* 次のスレッドIDを取得 */
    thread_id_t thread_id = this->GetNextID();

    /* スレッドマップにIDとスレッドを追加 */
    this->m_ThreadMap.emplace(std::piecewise_construct, std::forward_as_tuple(thread_id), std::forward_as_tuple(thread_id, name, thread_func));

    /* スレッド名マップにIDとスレッド名を追加 */
    this->m_ThreadNameMap.insert_or_assign(thread_id, name);

    return thread_id;
}

/* スレッド存在確認 */
bool ThreadManager::IsExists(const thread_id_t thread_id)
{
    /* スレッドIDが登録されているか確認 */
    if (this->m_ThreadMap.count(thread_id) > 0 && this->m_ThreadNameMap.count(thread_id) > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/* スレッド名取得 */
const std::string ThreadManager::GetName(const thread_id_t thread_id)
{
    /* スレッドが登録されている場合 */
    if (this->IsExists(thread_id) == true)
    {
        /* スレッド名取得 */
        return this->m_ThreadMap[thread_id].GetName();
    }
    else
    {
        Logger::Error("Thread is not registered : ID=%d", thread_id);

        return "";
    }
}

/* スレッド開始 */
void ThreadManager::Start(thread_id_t thread_id)
{
    /* スレッドが登録されている場合 */
    if (this->IsExists(thread_id) == true)
    {
        /* スレッド開始 */
        this->m_ThreadMap[thread_id].Start();
    }
    else
    {
        Logger::Error("Thread is not registered : ID=%d", thread_id);
    }
}

/* スレッド停止 */
void ThreadManager::Stop(thread_id_t thread_id)
{
    /* スレッドが登録されている場合 */
    if (this->IsExists(thread_id) == true)
    {
        /* スレッド停止 */
        this->m_ThreadMap[thread_id].Stop();
    }
    else
    {
        Logger::Error("Thread is not registered : ID=%d", thread_id);
    }
}

/* スレッドレディ通知 */
void ThreadManager::NotifyReady(thread_id_t thread_id)
{
    /* スレッドが登録されている場合 */
    if (this->IsExists(thread_id) == true)
    {
        /* スレッドレディ通知 */
        this->m_ThreadMap[thread_id].NotifyReady();
    }
    else
    {
        Logger::Error("Thread is not registered : ID=%d", thread_id);
    }
}


/* スレッド終了まで待機 */
void ThreadManager::Join(thread_id_t thread_id)
{
    /* スレッドが登録されている場合 */
    if (this->IsExists(thread_id) == true)
    {
        /* スレッド終了まで待機 */
        this->m_ThreadMap[thread_id].Join();
    }
    else
    {
        Logger::Error("Thread is not registered : ID=%d", thread_id);
    }
}

/* スレッドの管理を放棄 */
void ThreadManager::Detach(thread_id_t thread_id)
{
    /* スレッドが登録されている場合 */
    if (this->IsExists(thread_id) == true)
    {
        /* スレッドの管理を放棄 */
        this->m_ThreadMap[thread_id].Detach();
    }
    else
    {
        Logger::Error("Thread is not registered : ID=%d", thread_id);
    }
}


/* 全スレッド開始 */
void ThreadManager::StartAll()
{
    /* 全スレッドを走査 */
    for (const auto& item : this->m_ThreadNameMap)
    {
        /* スレッドIDを取得 */
        thread_id_t thread_id = item.first;

        /* スレッドを開始 */
        this->Start(thread_id);
    }
}

/* 全スレッド停止 */
void ThreadManager::StopAll()
{
    /* 全スレッドを走査 */
    for (const auto& item : this->m_ThreadNameMap)
    {
        /* スレッドIDを取得 */
        thread_id_t thread_id = item.first;

        /* スレッドを停止 */
        this->Stop(thread_id);
    }
}

/* 全スレッドレディ通知 */
void ThreadManager::NotifyReadyAll()
{
    /* 全スレッドを走査 */
    for (const auto& item : this->m_ThreadNameMap)
    {
        /* スレッドIDを取得 */
        thread_id_t thread_id = item.first;

        /* スレッドレディ通知 */
        this->NotifyReady(thread_id);
    }
}


/* 全スレッド終了まで待機 */
void ThreadManager::JoinAll()
{
    /* 全スレッドを走査 */
    for (const auto& item : this->m_ThreadNameMap)
    {
        /* スレッドIDを取得 */
        thread_id_t thread_id = item.first;

        /* レッド終了まで待機 */
        this->Join(thread_id);
    }
}

/* 全スレッドの管理を放棄 */
void ThreadManager::DetachAll()
{
    /* 全スレッドを走査 */
    for (const auto& item : this->m_ThreadNameMap)
    {
        /* スレッドIDを取得 */
        thread_id_t thread_id = item.first;

        /* スレッドの管理を放棄 */
        this->Join(thread_id);
    }
}

/* 次のスレッドIDを取得 */
thread_id_t ThreadManager::GetNextID()
{
    while (true)
    {
        /* スレッドIDインクリメント(1始まり) */
        this->m_ThreadCount++;

        /* スレッドIDが登録されていない */
        if (this->IsExists(this->m_ThreadCount) == false)
        {
            return this->m_ThreadCount;
        }
    }
}

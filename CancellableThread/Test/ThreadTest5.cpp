#include "ThreadManager.h"
#include "CancellableThread.h"
#include "StringFormat.h"
#include "Logger.h"

/* �e�X�g�X���b�h���s���� */
static void ThreadFunction1(CancellableThread& thread, CancellationPoint& cp);
static void ThreadFunction2(CancellableThread& thread, CancellationPoint& cp);
static void ThreadFunction3(CancellableThread& thread, CancellationPoint& cp);


/* �X���b�h�e�X�g5 : �X���b�h�}�l�[�W���[ */
void ThreadTest5()
{
    try
    {
        Logger::Info("[Main] Start");

        /* �X���b�h�}�l�[�W���[�V���O���g���C���X�^���X�擾 */
        ThreadManager& thread_manager = ThreadManager::GetInstance();

        /* �e�X�g�X���b�h��ǉ� */
        thread_id_t thread1_id = thread_manager.Add("Test Thread 1", ThreadFunction1);
        Logger::Info("Add Thread [%d:%s]", thread1_id, thread_manager.GetName(thread1_id));

        thread_id_t thread2_id = thread_manager.Add("Test Thread 2", ThreadFunction2);
        Logger::Info("Add Thread [%d:%s]", thread2_id, thread_manager.GetName(thread2_id));

        thread_id_t thread3_id = thread_manager.Add("Test Thread 3", ThreadFunction3);
        Logger::Info("Add Thread [%d:%s]", thread3_id, thread_manager.GetName(thread3_id));

        try
        {
            /* �e�X�g�X���b�h��S�ĊJ�n */
            thread_manager.StartAll();

            /* �J�E���^������ */
            size_t i = 0;

            while (true)
            {
                /* �J�E���^�X�V */
                i++;

                Logger::Info("[Main] Execute Value=%d", i);

                /* 5sec�� */
                if (i == 5)
                {
                    Logger::Info("[Main] Request Stop Thread [%s]", thread_manager.GetName(thread1_id));

                    /* �e�X�g�X���b�h1���~ */
                    thread_manager.Stop(thread1_id);

                    Logger::Info("[Main] Request Ready Thread [%s]", thread_manager.GetName(thread2_id));

                    /* �e�X�g�X���b�h2�Ƀ��f�B�ʒm */
                    thread_manager.NotifyReady(thread2_id);

                    Logger::Info("[Main] Request Ready Thread [%s]", thread_manager.GetName(thread3_id));

                    /* �e�X�g�X���b�h3�Ƀ��f�B�ʒm */
                    thread_manager.NotifyReady(thread3_id);
                }
                /* 10sec�� */
                else if (i == 10)
                {
                    Logger::Info("[Main] Request Stop Thread [%s]", thread_manager.GetName(thread2_id));

                    /* �e�X�g�X���b�h2���~ */
                    thread_manager.Stop(thread2_id);
                }
                /* 15sec�� */
                else if (i == 15)
                {
                    Logger::Info("[Main] Request Stop Thread [%s]", thread_manager.GetName(thread3_id));

                    /* �e�X�g�X���b�h3���~ */
                    thread_manager.Stop(thread3_id);

                    Logger::Info("[Main] Start Thread [%s]", thread_manager.GetName(thread1_id));

                    /* �e�X�g�X���b�h1���J�n */
                    thread_manager.Start(thread1_id);

                    Logger::Info("[Main] Start Thread [%s]", thread_manager.GetName(thread2_id));

                    /* �e�X�g�X���b�h2���J�n */
                    thread_manager.Start(thread2_id);

                    Logger::Info("[Main] Start Thread [%s]", thread_manager.GetName(thread3_id));

                    /* �e�X�g�X���b�h3���J�n */
                    thread_manager.Start(thread3_id);
                }
                /* 20sec�� */
                else if (i == 20)
                {
                    Logger::Info("[Main] Request Ready Thread [%s]", thread_manager.GetName(thread2_id));

                    /* �e�X�g�X���b�h2�Ƀ��f�B�ʒm */
                    thread_manager.NotifyReady(thread2_id);

                    /* �e�X�g�X���b�h3�ɂ̓��f�B�ʒm���Ȃ� */
                }
                /* 35sec�� */
                else if (i == 35)
                {
                    /* �e�X�g�X���b�h���~���Ȃ� */

                    Logger::Info("[Main] Execute End");

                    /* ���C�������I�� */
                    break;
                }

                /* 1sec�ҋ@ */
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
        catch (const std::exception& ex)
        {
            Logger::Error("Exception Occurred : %s", ex.what());
        }

        Logger::Info("[Main] Request Stop All Thread");

        /* �S�ẴX���b�h���~(�G���[���� or �X���b�h��~�����Ƀ��C���������I��) */
        thread_manager.StopAll();

        Logger::Info("[Main] End");
    }
    catch (const std::exception& ex)
    {
        Logger::Error("Exception Occurred : %s", ex.what());
    }
}

/* �e�X�g�X���b�h1���s���� */
static void ThreadFunction1(CancellableThread& thread, CancellationPoint& cp)
{
    Logger::Info("[%s] Thread Start", thread.GetName());

    /* �J�E���^������ */
    size_t i = 0;

    try
    {
        while (true)
        {
            /* �J�E���^�X�V */
            i++;

            Logger::Info("[%s] Thread Execute Value=%d", thread.GetName(), i);

            /* �L�����Z�����N�G�X�g�m�F */
            cp.CheckCancelRequested();

            /* 1sec�ҋ@ */
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    /* �X���b�h�L�����Z������ */
    catch (const CancelledException& ex)
    {
        Logger::Warn(ex.what());
        Logger::Warn("[%s] Thread Cancelled", thread.GetName());
    }
    /* ����ȊO�̗�O���� */
    catch (const std::exception& ex)
    {
        Logger::Error("Exception Occurred : %s", ex.what());
    }

    Logger::Info("[%s] Thread End", thread.GetName());
}

/* �e�X�g�X���b�h2���s���� */
static void ThreadFunction2(CancellableThread& thread, CancellationPoint& cp)
{
    Logger::Info("[%s] Thread Start", thread.GetName());

    /* �J�E���^������ */
    size_t i = 0;

    try
    {
        Logger::Info("[%s] Wait Thread Ready", thread.GetName(), i);

        /* �X���b�h��Ready�v����ҋ@ */
        cp.WaitReady();

        while (true)
        {
            /* �J�E���^�X�V */
            i++;

            Logger::Info("[%s] Thread Execute Value=%d", thread.GetName(), i);

            /* �L�����Z�����N�G�X�g�m�F */
            cp.CheckCancelRequested();

            /* 1sec�ҋ@ */
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    /* �X���b�h�L�����Z������ */
    catch (const CancelledException& ex)
    {
        Logger::Warn(ex.what());
        Logger::Warn("[%s] Thread Cancelled", thread.GetName());
    }
    /* ����ȊO�̗�O���� */
    catch (const std::exception& ex)
    {
        Logger::Error("Exception Occurred : %s", ex.what());
    }

    Logger::Info("[%s] Thread End", thread.GetName());
}

/* �e�X�g�X���b�h3���s���� */
static void ThreadFunction3(CancellableThread& thread, CancellationPoint& cp)
{
    Logger::Info("[%s] Thread Start", thread.GetName());

    /* �J�E���^������ */
    size_t i = 0;

    try
    {
        Logger::Info("[%s] Wait Thread Ready", thread.GetName(), i);

        /* �X���b�h��Ready�v����ҋ@(10�b�Ń^�C���A�E�g) */
        cp.WaitFor(std::chrono::seconds(10));

        while (true)
        {
            /* �J�E���^�X�V */
            i++;

            Logger::Info("[%s] Thread Execute Value=%d", thread.GetName(), i);

            /* �L�����Z�����N�G�X�g�m�F */
            cp.CheckCancelRequested();

            /* 1sec�ҋ@ */
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    /* �X���b�h�L�����Z������ */
    catch (const CancelledException& ex)
    {
        Logger::Warn(ex.what());
        Logger::Warn("[%s] Thread Cancelled", thread.GetName());
    }
    /* ����ȊO�̗�O���� */
    catch (const std::exception& ex)
    {
        Logger::Error("Exception Occurred : %s", ex.what());
    }

    Logger::Info("[%s] Thread End", thread.GetName());
}

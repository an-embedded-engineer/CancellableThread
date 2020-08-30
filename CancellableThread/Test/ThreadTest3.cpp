#include "CancellableThread.h"
#include "StringFormat.h"
#include "Logger.h"

/* �e�X�g�X���b�h���s���� */
static void ThreadFunction(CancellableThread& thread, CancellationPoint& cp);

/* �X���b�h�e�X�g3 : �X���b�h�x�����s(���Ύ��ԃ^�C���A�E�g����) */
void ThreadTest3()
{
    try
    {
        Logger::Info("[Main] Start");

        /* �e�X�g�X���b�h�𐶐� */
        CancellableThread test_thread(1, "Test Thread", ThreadFunction);

        Logger::Info("[Main] Create Thread [%s]", test_thread.GetName());

        try
        {
            /* �e�X�g�X���b�h���J�n */
            test_thread.Start();

            Logger::Info("[Main] Start Thread [%s]", test_thread.GetName());

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
                    Logger::Info("[Main] Request Ready Thread [%s]", test_thread.GetName());

                    /* �e�X�g�X���b�h�̃��f�B��v�� */
                    test_thread.NotifyReady();
                }
                /* 10sec�� */
                else if (i == 10)
                {
                    Logger::Info("[Main] Request Stop Thread [%s]", test_thread.GetName());

                    /* �e�X�g�X���b�h���~ */
                    test_thread.Stop();
                }
                /* 15sec�� */
                else if (i == 15)
                {
                    Logger::Info("[Main] Start Thread [%s]", test_thread.GetName());

                    /* �e�X�g�X���b�h���J�n */
                    test_thread.Start();
                }
                /* 20sec�� */
                else if (i == 20)
                {
                    Logger::Info("[Main] Request Stop Thread [%s]", test_thread.GetName());

                    /* �e�X�g�X���b�h�����f�B�v�������ɒ�~ */
                    test_thread.Stop();
                }
                /* 25sec�� */
                else if (i == 25)
                {
                    Logger::Info("[Main] Start Thread [%s]", test_thread.GetName());

                    /* �e�X�g�X���b�h���J�n */
                    test_thread.Start();
                }
                /* 40sec�� */
                else if (i == 40)
                {
                    /* �e�X�g�X���b�h���^�C���A�E�g����܂Ń��f�B�v�����Ȃ� */

                    /* ���C�������I���O�Ƀe�X�g�X���b�h�I����ҋ@ */
                    test_thread.Join();

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

        /* �e�X�g�X���b�h�����s���̏ꍇ(�G���[���� or �X���b�h��~�����Ƀ��C���������I��) */
        if (test_thread.IsRunning() == true)
        {
            Logger::Info("[Main] Request Stop Thread [%s]", test_thread.GetName());

            /* �e�X�g�X���b�h���~ */
            test_thread.Stop();
        }

        Logger::Info("[Main] End");
    }
    catch (const std::exception& ex)
    {
        Logger::Error("Exception Occurred : %s", ex.what());
    }
}

/* �e�X�g�X���b�h���s���� */
static void ThreadFunction(CancellableThread& thread, CancellationPoint& cp)
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

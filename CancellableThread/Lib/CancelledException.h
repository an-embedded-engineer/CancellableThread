#pragma once
#include <string>

/* Cancellable Thread�N���X�O���錾 */
class CancellableThread;

/* Cancelled Exception�N���X�錾 */
class CancelledException
{
public:
    /* �R���X�g���N�^ */
    CancelledException(CancellableThread& thread);
    CancelledException(CancellableThread& thread, const std::string& message);

    /* ���b�Z�[�W���e���擾 */
    const char* what() const noexcept;

private:
    /* �G���[���b�Z�[�W���� */
    std::string GenerateErrorMessage(CancellableThread& thread, const std::string& message);

private:
    /* �G���[���b�Z�[�W */
    std::string m_ErrorMessage;
};

#pragma once
#include <cstddef>
#include <functional>

/* クラス前方宣言 */
class CancellableThread;
class CancellationPoint;

/* スレッドID型 */
using thread_id_t = size_t;

/* スレッド実行関数型 */
using ThreadFuncType = std::function<void(CancellableThread& thread, CancellationPoint&)>;


#include "callback_scheduler.h"

CallbackScheduler::TaskId CallbackScheduler::Schedule(std::function<void()> callback, TimePoint when)
{
    // TODO(student): реализуйте планирование и выполнение callback в момент when.
    //
    // Минимальные требования:
    // - callback должен быть вызван не раньше when;
    // - необходимо поддерживать несколько запланированных callback;
    // - решение должно быть потокобезопасным;
    // - метод должен вернуть идентификатор задачи для последующей отмены.
    (void)callback;
    (void)when;
    return 0;
}

bool CallbackScheduler::Cancel(TaskId id)
{
    // TODO(student): отмените задачу по идентификатору id.
    // Возвращайте true, если задача была найдена и отменена, иначе false.
    (void)id;
    return false;
}

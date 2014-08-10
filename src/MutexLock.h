#ifndef MUTEXLOCK_H
#define MUTEXLOCK_H 

#include <pthread.h>
#include <assert.h>
#include "NonCopyable.h"

class MutexLock : NonCopyable
{
    friend class MutexLockGuard;
    public:
        MutexLock();
        ~MutexLock();

        //思考为什么不是const
        pthread_mutex_t *getMutexPtr() { return &mutex_;}  
        bool isLocked() const { return isLocked_; }
    private:
        //防止用户手工调用
        void lock();
        void unlock();

        pthread_mutex_t mutex_;
        bool isLocked_;
};

inline MutexLock::MutexLock()
    :isLocked_(false)
{
    pthread_mutex_init(&mutex_, NULL);
}

inline MutexLock::~MutexLock()
{
    //确保这里已经解锁
    assert(isLocked_ == false);
    pthread_mutex_destroy(&mutex_);
}

inline void MutexLock::lock()
{
    pthread_mutex_lock(&mutex_);
    isLocked_ = true;
}

inline void MutexLock::unlock()
{
    isLocked_ = false;
    pthread_mutex_unlock(&mutex_);
}


class MutexLockGuard
{
    public:
        MutexLockGuard(MutexLock &mutex);
        ~MutexLockGuard();
    private:
        MutexLock &mutex_;
};

inline MutexLockGuard::MutexLockGuard(MutexLock &mutex)
    :mutex_(mutex)
{
    mutex_.lock();
}

inline MutexLockGuard::~MutexLockGuard()
{
    mutex_.unlock();
}


//MutexLockGuard(mutex_);
//帮助在编译期间发现错误
#define MutexLockGuard(m) "ERROR"


#endif  /*MUTEXLOCK_H*/

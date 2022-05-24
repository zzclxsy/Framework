#include "TimerWheel.h"
#include "chrono"
#include <thread>

// 第1个轮占的位数
#define TVR_BITS 8
// 第1个轮的长度
#define TVR_SIZE (1 << TVR_BITS)

// 第n个轮占的位数，TVR_BITS不计算在n内，n从0开始
#define TVN_BITS 6
// 第n个轮的长度
#define TVN_SIZE (1 << TVN_BITS)

#define TVN_MASK (TVN_SIZE - 1)
#define TVR_MASK (TVR_SIZE - 1)
#define OFFSET(N) (TVR_SIZE + (N) *TVN_SIZE)
//V是期望时间毫秒搓，N是第几个轮，当N为0时
//先执行((V >> (TVR_BITS + (0) *TVN_BITS)) V先右移动8位，将第1个轮移除掉，在与上TVN_MASK
#define INDEX(V, N) ((V >> (TVR_BITS + (N) *TVN_BITS)) & TVN_MASK)

namespace TimerWheel{
Timer::Timer()
{

}

Timer::Timer(TimerManager* manager)
    : manager_(manager)
    , vecIndex_(-1)
{
}

Timer::~Timer()
{
    Stop();
}

void Timer::Start()
{
    Stop();
    expires_ = interval_ + TimerManager::GetCurrentMillisecs();
    manager_->AddTimer(this);
}

void Timer::Stop()
{
    if (vecIndex_ != -1)
    {
        manager_->RemoveTimer(this);
        vecIndex_ = -1;
    }
}

void Timer::setManager(TimerManager *manager)
{
    manager_ = manager;
    vecIndex_ = -1;
}

void Timer::OnTimer(unsigned long long now)
{
    if (timerType_ == Timer::CIRCLE)
    {
        expires_ = interval_ + now;
        manager_->AddTimer(this);
    }
    else
    {
        vecIndex_ = -1;
    }
    timerFun_();
}

// TimerManager
TimerManager *TimerManager::mp_instant = nullptr;
TimerManager::TimerManager()
{
    tvec_.resize(TVR_SIZE + 4 * TVN_SIZE);
    checkTime_ = GetCurrentMillisecs();
    mb_start = false;
}

void TimerManager::AddTimer(Timer* timer)
{
    unsigned long long expires = timer->expires_;
    unsigned long long idx = expires - checkTime_;


    //间隔时间小于255
    if (idx < TVR_SIZE)
    {
        //期望触发时间为当前时间加上间隔，因为间隔小于255ms，expires&0xff，将其结果放到时间轮上面，遍历时间轮的时候即可准时触发改定时器
        timer->vecIndex_ = expires & TVR_MASK;  //取出0-8位数据，将其放到对应块内
    }//触发时间256 -- 16383(ms)
    else if (idx < 1 << (TVR_BITS + TVN_BITS))
    {
        //TVR_SIZE占据了0-255，所以先偏移256，即OFFSET(0)
        //OFFSET(0) + INDEX(expires, 0)意思是移除当前轮的前面几个轮所占的数据空间，在&上TVN_MASK，获取结果
        timer->vecIndex_ = OFFSET(0) + INDEX(expires, 0);//取出9-14位数据，将其放到对应块内
    }
    else if (idx < 1 << (TVR_BITS + 2 * TVN_BITS))
    {
        timer->vecIndex_ = OFFSET(1) + INDEX(expires, 1);
    }
    else if (idx < 1 << (TVR_BITS + 3 * TVN_BITS))
    {
        timer->vecIndex_ = OFFSET(2) + INDEX(expires, 2);
    }
    else if ((long long) idx < 0)
    {
        timer->vecIndex_ = checkTime_ & TVR_MASK;
    }
    else
    {
        if (idx > 0xffffffffUL)
        {
            idx = 0xffffffffUL;
            expires = idx + checkTime_;
        }
        timer->vecIndex_ = OFFSET(3) + INDEX(expires, 3);
    }

    TimeList& tlist = tvec_[timer->vecIndex_];
    tlist.push_back(timer);
    timer->itr_ = tlist.end();
    --(timer->itr_);//指向当前TimeList的最后一项
}

void TimerManager::RemoveTimer(Timer* timer)
{
    TimeList& tlist = tvec_[timer->vecIndex_];
    tlist.erase(timer->itr_);
}

void TimerManager::DetectTimers()
{
    unsigned long long now = GetCurrentMillisecs();
    while (checkTime_ <= now)
    {
        //遇上0xff，相当于255是一个轮回，时间涨到255后，又开始重0计算，相当于255进制
        int index = checkTime_ & TVR_MASK;
        if (!index &&
                !Cascade(OFFSET(0), INDEX(checkTime_, 0)) &&
                !Cascade(OFFSET(1), INDEX(checkTime_, 1)) &&
                !Cascade(OFFSET(2), INDEX(checkTime_, 2)))
        {
            Cascade(OFFSET(3), INDEX(checkTime_, 3));
        }
        ++checkTime_;

        TimeList& tlist = tvec_[index];
        TimeList temp;
        temp.splice(temp.end(), tlist);

        for (TimeList::iterator itr = temp.begin(); itr != temp.end(); ++itr)
        {
            (*itr)->OnTimer(now);
        }
    }
}

TimerManager *TimerManager::instant()
{
    if (mp_instant)
        return mp_instant;

    mp_instant = new TimerManager();
    return mp_instant;
}

void TimerManager::run()
{
    mb_start = true;
    std::thread th(std::bind(&TimerManager::workPrc,this));
    th.detach();
}

int TimerManager::Cascade(int offset, int index)
{
    //判断是否到达轮子时间，到达则将其移动到更上层的轮里面
    TimeList& tlist = tvec_[offset + index];
    TimeList temp;
    temp.splice(temp.end(), tlist);

    for (TimeList::iterator itr = temp.begin(); itr != temp.end(); ++itr)
    {
        AddTimer(*itr);
    }
    return index;
}

void TimerManager::workPrc()
{
    while (mb_start)
    {
        DetectTimers();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

unsigned long long TimerManager::GetCurrentMillisecs()
{
    std::chrono::system_clock clock;
    unsigned long long msec = std::chrono::duration_cast<std::chrono::milliseconds>(clock.now().time_since_epoch()).count();
    return msec;
}

}
